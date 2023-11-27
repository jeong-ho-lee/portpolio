#!/usr/bin/env python
# coding: utf-8

# In[ ]:


# %%
import random
import numpy as np
import cv2
import os
import torch
import torch.utils.data as data
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader
from torchvision.transforms import ToTensor, Normalize, Compose
from os.path import join
from os import listdir
from torchsummary import summary
import time
import zipfile
from os.path import splitext
import torchvision.transforms as transforms
import torch.nn.functional as F
from torchvision.datasets import ImageFolder
import csv
import basicsr
from torch.optim.lr_scheduler import CosineAnnealingLR
import math
from PIL import Image
from torch.utils.data import Subset
from torch.optim.lr_scheduler import ReduceLROnPlateau
import torch.nn.init as init
import mmap




#스케쥴러를 expotential로 바꿈. 
#모델 구조는 동일
#epoch는 70 for 학습률이 0.0001보다 작어졌을 때 더 좋은지 알아보려고





# 랜덤 시드 고정
random_seed = 42
torch.manual_seed(random_seed)
torch.cuda.manual_seed(random_seed)
torch.cuda.manual_seed_all(random_seed)
torch.backends.cudnn.deterministic = True
torch.backends.cudnn.benchmark = False
np.random.seed(random_seed)
random.seed(random_seed)


# 시작 시간 기록
start_time = time.time()


# 하이퍼파라미터 설정
num_epochs = 30
batch_size = 32
learning_rate = 0.001
channel = 3

# -----------------------------define--------------------------------

# 이미지 로드 함수 정의
def load_img(filepath):
    img = cv2.imread(filepath)
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    return img

class KBAFunction(torch.autograd.Function):

    @staticmethod
    def forward(ctx, x, att, selfk, selfg, selfb, selfw):
        B, nset, H, W = att.shape
        KK = selfk ** 2
        selfc = x.shape[1]

        att = att.reshape(B, nset, H * W).transpose(-2, -1)

        ctx.selfk, ctx.selfg, ctx.selfc, ctx.KK, ctx.nset = selfk, selfg, selfc, KK, nset
        ctx.x, ctx.att, ctx.selfb, ctx.selfw = x, att, selfb, selfw

        bias = att @ selfb
        attk = att @ selfw

        uf = torch.nn.functional.unfold(x, kernel_size=selfk, padding=selfk // 2)

        # for unfold att / less memory cost
        uf = uf.reshape(B, selfg, selfc // selfg * KK, H * W).permute(0, 3, 1, 2)
        attk = attk.reshape(B, H * W, selfg, selfc // selfg, selfc // selfg * KK)

        x = attk @ uf.unsqueeze(-1)  #
        del attk, uf
        x = x.squeeze(-1).reshape(B, H * W, selfc) + bias
        x = x.transpose(-1, -2).reshape(B, selfc, H, W)
        return x

    @staticmethod
    def backward(ctx, grad_output):
        x, att, selfb, selfw = ctx.x, ctx.att, ctx.selfb, ctx.selfw
        selfk, selfg, selfc, KK, nset = ctx.selfk, ctx.selfg, ctx.selfc, ctx.KK, ctx.nset

        B, selfc, H, W = grad_output.size()

        dbias = grad_output.reshape(B, selfc, H * W).transpose(-1, -2)

        dselfb = att.transpose(-2, -1) @ dbias
        datt = dbias @ selfb.transpose(-2, -1)

        attk = att @ selfw
        uf = F.unfold(x, kernel_size=selfk, padding=selfk // 2)
        # for unfold att / less memory cost
        uf = uf.reshape(B, selfg, selfc // selfg * KK, H * W).permute(0, 3, 1, 2)
        attk = attk.reshape(B, H * W, selfg, selfc // selfg, selfc // selfg * KK)

        dx = dbias.view(B, H * W, selfg, selfc // selfg, 1)

        dattk = dx @ uf.view(B, H * W, selfg, 1, selfc // selfg * KK)
        duf = attk.transpose(-2, -1) @ dx
        del attk, uf

        dattk = dattk.view(B, H * W, -1)
        datt += dattk @ selfw.transpose(-2, -1)
        dselfw = att.transpose(-2, -1) @ dattk

        duf = duf.permute(0, 2, 3, 4, 1).view(B, -1, H * W)
        dx = F.fold(duf, output_size=(H, W), kernel_size=selfk, padding=selfk // 2)

        datt = datt.transpose(-1, -2).view(B, nset, H, W)

        return dx, datt, None, None, dselfb, dselfw

class LayerNormFunction(torch.autograd.Function):

    @staticmethod
    def forward(ctx, x, weight, bias, eps):
        ctx.eps = eps
        N, C, H, W = x.size()
        mu = x.mean(1, keepdim=True)
        var = (x - mu).pow(2).mean(1, keepdim=True)
        y = (x - mu) / (var + eps).sqrt()
        ctx.save_for_backward(y, var, weight)
        y = weight.view(1, C, 1, 1) * y + bias.view(1, C, 1, 1)
        return y

    @staticmethod
    def backward(ctx, grad_output):
        eps = ctx.eps

        N, C, H, W = grad_output.size()
        y, var, weight = ctx.saved_tensors
        g = grad_output * weight.view(1, C, 1, 1)
        mean_g = g.mean(dim=1, keepdim=True)

        mean_gy = (g * y).mean(dim=1, keepdim=True)
        gx = 1. / torch.sqrt(var + eps) * (g - y * mean_gy - mean_g)
        return gx, (grad_output * y).sum(dim=3).sum(dim=2).sum(dim=0), grad_output.sum(dim=3).sum(dim=2).sum(
            dim=0), None

class LayerNorm2d(nn.Module):

    def __init__(self, channels, eps=1e-6):
        super(LayerNorm2d, self).__init__()
        self.register_parameter('weight', nn.Parameter(torch.ones(channels)))
        self.register_parameter('bias', nn.Parameter(torch.zeros(channels)))
        self.eps = eps

    def forward(self, x):
        return LayerNormFunction.apply(x, self.weight, self.bias, self.eps)




class SimpleGate(nn.Module):
    def forward(self, x):
        x1, x2 = x.chunk(2, dim=1)
        return x1 * x2

class KBBlock_s(nn.Module):
    def __init__(self, c, DW_Expand=2, FFN_Expand=2, nset=32, k=3, gc=4, lightweight=False):
        super(KBBlock_s, self).__init__()
        self.k, self.c = k, c
        self.nset = nset
        dw_ch = int(c * DW_Expand)
        ffn_ch = int(FFN_Expand * c)

        self.g = c // gc
        self.w = nn.Parameter(torch.zeros(1, nset, c * c // self.g * self.k ** 2))
        self.b = nn.Parameter(torch.zeros(1, nset, c))
        self.init_p(self.w, self.b)

        self.norm1 = LayerNorm2d(c)
        self.norm2 = LayerNorm2d(c)

        self.sca = nn.Sequential(
            nn.AdaptiveAvgPool2d(1),
            nn.Conv2d(in_channels=c, out_channels=c, kernel_size=1, padding=0, stride=1,
                      groups=1, bias=True),
        )

        if not lightweight:
            self.conv11 = nn.Sequential(
                nn.Conv2d(in_channels=c, out_channels=c, kernel_size=1, padding=0, stride=1, groups=1,
                          bias=True),
                nn.Conv2d(in_channels=c, out_channels=c, kernel_size=5, padding=2, stride=1, groups=c // 4,
                          bias=True),
            )
        else:
            self.conv11 = nn.Sequential(
                nn.Conv2d(in_channels=c, out_channels=c, kernel_size=1, padding=0, stride=1, groups=1,
                          bias=True),
                nn.Conv2d(in_channels=c, out_channels=c, kernel_size=3, padding=1, stride=1, groups=c,
                          bias=True),
            )

        self.conv1 = nn.Conv2d(in_channels=c, out_channels=c, kernel_size=1, padding=0, stride=1, groups=1,
                               bias=True)
        self.conv21 = nn.Conv2d(in_channels=c, out_channels=c, kernel_size=3, padding=1, stride=1, groups=c,
                                bias=True)

        interc = min(c, 32)
        self.conv2 = nn.Sequential(
            nn.Conv2d(in_channels=c, out_channels=interc, kernel_size=3, padding=1, stride=1, groups=interc,
                      bias=True),
            SimpleGate(),
            nn.Conv2d(interc // 2, self.nset, 1, padding=0, stride=1),
        )

        self.conv211 = nn.Conv2d(in_channels=c, out_channels=self.nset, kernel_size=1)

        self.conv3 = nn.Conv2d(in_channels=dw_ch // 2, out_channels=c, kernel_size=1, padding=0, stride=1,
                               groups=1, bias=True)

        self.conv4 = nn.Conv2d(in_channels=c, out_channels=ffn_ch, kernel_size=1, padding=0, stride=1, groups=1,
                               bias=True)
        self.conv5 = nn.Conv2d(in_channels=ffn_ch // 2, out_channels=c, kernel_size=1, padding=0, stride=1,
                               groups=1, bias=True)

        self.dropout1 = nn.Identity()
        self.dropout2 = nn.Identity()

        self.ga1 = nn.Parameter(torch.zeros((1, c, 1, 1)) + 1e-2, requires_grad=True)
        self.attgamma = nn.Parameter(torch.zeros((1, self.nset, 1, 1)) + 1e-2, requires_grad=True)
        self.sg = SimpleGate()

        self.beta = nn.Parameter(torch.zeros((1, c, 1, 1)) + 1e-2, requires_grad=True)
        self.gamma = nn.Parameter(torch.zeros((1, c, 1, 1)) + 1e-2, requires_grad=True)

    def init_p(self, weight, bias=None):
        init.kaiming_uniform_(weight, a=math.sqrt(5))
        if bias is not None:
            fan_in, _ = init._calculate_fan_in_and_fan_out(weight)
            bound = 1 / math.sqrt(fan_in)
            init.uniform_(bias, -bound, bound)

    def KBA(self, x, att, selfk, selfg, selfb, selfw):
        return KBAFunction.apply(x, att, selfk, selfg, selfb, selfw)

    def forward(self, inp):
        x = inp

        x = self.norm1(x)
        sca = self.sca(x)
        x1 = self.conv11(x)

        # KBA module
        att = self.conv2(x) * self.attgamma + self.conv211(x)
        uf = self.conv21(self.conv1(x))
        x = self.KBA(uf, att, self.k, self.g, self.b, self.w) * self.ga1 + uf
        x = x * x1 * sca

        x = self.conv3(x)
        x = self.dropout1(x)
        y = inp + x * self.beta

        # FFN
        x = self.norm2(y)
        x = self.conv4(x)
        x = self.sg(x)
        x = self.conv5(x)

        x = self.dropout2(x)        
        return y + x * self.gamma


class KBNet_s(nn.Module):
    def __init__(self, img_channel=3, width=16, middle_blk_num=4, enc_blk_nums=[2,2,2],
                 dec_blk_nums=[4,3,2], basicblock='KBBlock_s', lightweight=False, ffn_scale=2):
        super().__init__()
        basicblock = eval(basicblock)
        self.intro = nn.Conv2d(in_channels=img_channel, out_channels=width, kernel_size=3, padding=1, stride=1,
                               groups=1, bias=True)

        self.encoders = nn.ModuleList()
        self.middle_blks = nn.ModuleList()
        self.decoders = nn.ModuleList()

        self.ending = nn.Conv2d(in_channels=width, out_channels=img_channel, kernel_size=3, padding=1, stride=1,
                                groups=1, bias=True)

        self.ups = nn.ModuleList()
        self.downs = nn.ModuleList()

        chan = width
        for num in enc_blk_nums:
            self.encoders.append(
                nn.Sequential(
                    *[basicblock(chan, FFN_Expand=ffn_scale, lightweight=lightweight) for _ in range(num)]
                )
            )
            self.downs.append(
                nn.Conv2d(chan, 2 * chan, 2, 2)
            )
            chan = chan * 2

        self.middle_blks = \
            nn.Sequential(
                *[basicblock(chan, FFN_Expand=ffn_scale, lightweight=lightweight) for _ in range(middle_blk_num)]
            )

        for num in dec_blk_nums:
            self.ups.append(
                nn.Sequential(
                    nn.Conv2d(chan, chan * 2, 1, bias=False),
                    nn.PixelShuffle(2)
                )
            )
            chan = chan // 2
            self.decoders.append(
                nn.Sequential(
                    *[basicblock(chan, FFN_Expand=ffn_scale, lightweight=lightweight) for _ in range(num)]
                )
            )

        self.padder_size = 2 ** len(self.encoders)

    def forward(self, inp):
        B, C, H, W = inp.shape
        inp = self.check_image_size(inp)
        x = self.intro(inp)

        encs = []

        for encoder, down in zip(self.encoders, self.downs):
            x = encoder(x)
            encs.append(x)
            x = down(x)

        x = self.middle_blks(x)

        for decoder, up, enc_skip in zip(self.decoders, self.ups, encs[::-1]):
            x = up(x)
            x = x + enc_skip
            x = decoder(x)

        x = self.ending(x)
        x = x + inp

        return x[:, :, :H, :W]

    def check_image_size(self, x):
        _, _, h, w = x.size()
        mod_pad_h = (self.padder_size - h % self.padder_size) % self.padder_size
        mod_pad_w = (self.padder_size - w % self.padder_size) % self.padder_size
        x = F.pad(x, (0, mod_pad_w, 0, mod_pad_h))
        return x



# 커스텀 데이터셋 클래스 정의
# class CustomDataset(data.Dataset):
#     def __init__(self, noisy_image_paths, clean_image_paths, patch_size = 128, transform=None):
#         self.clean_image_paths = [join(clean_image_paths, x) for x in listdir(clean_image_paths)]
#         self.noisy_image_paths = [join(noisy_image_paths, x) for x in listdir(noisy_image_paths)]
#         self.transform = transform
#         self.patch_size = patch_size

#     def __len__(self):
#         return len(self.noisy_image_paths)

#     def __getitem__(self, index):
#         # 이미지 불러오기
#         noisy_image = load_img(self.noisy_image_paths[index])
#         clean_image = load_img(self.clean_image_paths[index])

#         H, W, _ = clean_image.shape

#         # 이미지 랜덤 크롭
#         rnd_h = random.randint(0, max(0, H - self.patch_size))
#         rnd_w = random.randint(0, max(0, W - self.patch_size))
#         noisy_image = noisy_image[rnd_h:rnd_h + self.patch_size, rnd_w:rnd_w + self.patch_size, :]
#         clean_image = clean_image[rnd_h:rnd_h + self.patch_size, rnd_w:rnd_w + self.patch_size, :]
        
#         # transform 적용
#         if self.transform:
#             noisy_image = self.transform(noisy_image)
#             clean_image = self.transform(clean_image)
        
#         return noisy_image, clean_image


class CustomDataset(data.Dataset):
    def __init__(self, scan_dir, clean_dir, patch_size = 128, transform=None):
        self.scan_dir = scan_dir
        self.clean_dir = clean_dir
        self.patch_size = patch_size
        self.transform = transform

        self.scan_paths = sorted(os.listdir(scan_dir))
        self.clean_paths = sorted(os.listdir(clean_dir))

    def __len__(self):
        return len(self.scan_paths)

    def __getitem__(self, index):
        scan_path = os.path.join(self.scan_dir, self.scan_paths[index])
        clean_path = os.path.join(self.clean_dir, self.clean_paths[index])

        # Load the clean and scan images using memory-mapped files
        scan_img = self.load_image(scan_path)
        clean_img = self.load_image(clean_path)

        # Apply data augmentation
        scan_img, clean_img = self.augment_image(scan_img, clean_img)

        # Convert Y channel images to PIL Images
        scan_img = Image.fromarray(scan_img)
        clean_img = Image.fromarray(clean_img)

        # Apply data transformation if provided
        if self.transform is not None:
            scan_img = self.transform(scan_img)
            clean_img = self.transform(clean_img)

        return scan_img, clean_img

    def load_image(self, image_path):
        # Open the image file using memory-mapped files
        with open(image_path, "rb") as f:
            mmapped_file = mmap.mmap(f.fileno(), length=0, access=mmap.ACCESS_READ)
            img_data = mmapped_file.read()

        # Read the image data using OpenCV
        img_array = np.frombuffer(img_data, dtype=np.uint8)
        img = cv2.imdecode(img_array, cv2.IMREAD_COLOR)
        
        # Convert from BGR to RGB
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        
        # Y Channel
        if channel == 1:
            # Convert RGB image to YUV color space
            img_yuv = cv2.cvtColor(img, cv2.COLOR_RGB2YUV)

            # Extract the Y channel
            img_y = img_yuv[:, :, 0]

            return img_y
        
        # RGB
        elif channel == 3:
            
            return img

    def augment_image(self, scan_img, clean_img):
        # Random crop
        # Y Channel
        if channel == 1:
            rows, cols = clean_img.shape
            x = np.random.randint(0, cols - self.patch_size)
            y = np.random.randint(0, rows - self.patch_size)
            scan_img = scan_img[y:y+self.patch_size, x:x+self.patch_size]
            clean_img = clean_img[y:y+self.patch_size, x:x+self.patch_size]
            rows, cols = clean_img.shape
        # RGB    
        elif channel == 3:
            rows, cols, _ = clean_img.shape
            x = np.random.randint(0, cols - self.patch_size)
            y = np.random.randint(0, rows - self.patch_size)
            scan_img = scan_img[y:y+self.patch_size, x:x+self.patch_size, :]
            clean_img = clean_img[y:y+self.patch_size, x:x+self.patch_size, :]
            rows, cols, _ = clean_img.shape
    
        # Random rotation
        angles = [0, 90, 180, 270, 360]
        angle = random.choice(angles)
        rotation_matrix = cv2.getRotationMatrix2D((cols / 2, rows / 2), angle, 1)
        scan_img = cv2.warpAffine(scan_img, rotation_matrix, (cols, rows))
        clean_img = cv2.warpAffine(clean_img, rotation_matrix, (cols, rows))

        # Random horizontal flip
        if np.random.rand() < 0.5:
            scan_img = np.fliplr(scan_img)
            clean_img = np.fliplr(clean_img)

        # Random vertical flip
        if np.random.rand() < 0.5:
            scan_img = np.flipud(scan_img)
            clean_img = np.flipud(clean_img)

        return scan_img, clean_img


# -----------------------------datasets----------------------------------


# 데이터셋 경로
noisy_image_paths = 'dataset/train/scan'
clean_image_paths = 'dataset/train/clean'

# 데이터셋 로드 및 전처리
train_transform = Compose([
    ToTensor()
])



# 커스텀 데이터셋 인스턴스 생성
train_dataset = CustomDataset(noisy_image_paths, clean_image_paths, transform=train_transform)

# 데이터 로더 설정
train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)

# valid set 설정
split_ratio = 0.8
dataset_size = len(train_dataset)
train_size = int(dataset_size * split_ratio)
validation_size = dataset_size - train_size

train_dataset, validation_dataset = torch.utils.data.random_split(train_dataset, [train_size, validation_size])

train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
valid_loader = DataLoader(validation_dataset, batch_size=batch_size, shuffle=False)


# --------------------------------train---------------------------------

# GPU 사용 여부 확인
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

# DnCNN 모델 인스턴스 생성 및 GPU로 이동
model = KBNet_s().to(device)
print(summary(model, (3, 128, 128)))

# 손실 함수와 최적화 알고리즘 설정
criterion = nn.MSELoss()
optimizer = optim.Adam(model.parameters(), lr=learning_rate)

# Set the desired final learning rate
final_lr = 0.0001

# Calculate the decay factor based on the desired final learning rate
gamma = (final_lr / 0.001) ** (1.0 / 50)
# Create the ExponentialLR scheduler
scheduler = torch.optim.lr_scheduler.ExponentialLR(optimizer, gamma=gamma)



# 모델 학습
best_loss = 9999.0
early_stopping_counter = 0 
patience = 50
valid_losses=[]
for epoch in range(num_epochs):
    model.train()
    running_loss = 0.0
    for noisy_images, clean_images in train_loader:
        noisy_images = noisy_images.to(device)
        clean_images = clean_images.to(device)
        optimizer.zero_grad()
        outputs = model(noisy_images)
        loss = criterion(outputs, noisy_images-clean_images)
        loss.backward()
        optimizer.step()
        running_loss += loss.item() * noisy_images.size(0)
    epoch_loss = running_loss / len(train_dataset)
    print(f'Epoch {epoch+1}/{num_epochs}, Loss: {epoch_loss:.4f}')

    model.eval()
    valid_loss = 0.0
    processed = 0.0
    for noisy_images, clean_images in valid_loader:
        noisy_images, clean_images = noisy_images.to(device), clean_images.to(device)
        output=model(noisy_images)
        loss = criterion(output, noisy_images-clean_images)
        valid_loss += loss.item() * noisy_images.size(0)
    valid_loss/=len(validation_dataset)
    valid_losses.append(valid_loss)
    print('\nEpoch {}/{}, Valid set : Average loss: {:.4f}\n'.format(epoch+1, num_epochs, valid_loss))
# 현재 val loss가 최소 loss보다 작으면 모델 갱신
    scheduler.step()

    if valid_loss < best_loss:
        best_loss = valid_loss
        torch.save(model.state_dict(), 'best_dncnn_model.pth')
        early_stopping_counter = 0  # 개선이 있으므로 카운터 초기화
        print(f"Saved model checkpoint at epoch {epoch+1}")
    else:
        early_stopping_counter += 1
    
    if early_stopping_counter >= patience:
        print(f"No improvement for {patience} epochs. Early stopping...")
        break



# -------------------------------time-------------------------

# 종료 시간 기록
end_time = time.time()

# 소요 시간 계산
training_time = end_time - start_time

# 시, 분, 초로 변환
minutes = int(training_time // 60)
seconds = int(training_time % 60)
hours = int(minutes // 60)
minutes = int(minutes % 60)

# 결과 출력
print(f"훈련 소요 시간: {hours}시간 {minutes}분 {seconds}초")

#---------------------------test-----------------------------



model = KBNet_s()
model.load_state_dict(torch.load('best_dncnn_model.pth'))
model.eval()
model.to(device)


# 데이터셋 경로
noisy_data_path = 'dataset/test/clean'
output_path = 'output'

if not os.path.exists(output_path):
    os.makedirs(output_path)

class CustomDatasetTest(data.Dataset):
    def __init__(self, noisy_image_paths, transform=None):
        self.noisy_image_paths = [join(noisy_image_paths, x) for x in listdir(noisy_image_paths)]
        self.transform = transform

    def __len__(self):
        return len(self.noisy_image_paths)

    def __getitem__(self, index):
        
        noisy_image_path = self.noisy_image_paths[index]
        noisy_image = load_img(self.noisy_image_paths[index])
        
        if self.transform:
            noisy_image = self.transform(noisy_image)

        return noisy_image, noisy_image_path

test_transform = Compose([
    ToTensor(),
])

# 데이터셋 로드 및 전처리
noisy_dataset = CustomDatasetTest(noisy_data_path, transform=test_transform)

# 데이터 로더 설정
noisy_loader = DataLoader(noisy_dataset, batch_size=1, shuffle=False)

# 이미지 denoising 및 저장
for noisy_image, noisy_image_path in noisy_loader:
    noisy_image = noisy_image.to(device)
    noise = model(noisy_image)

    denoised_image = noisy_image - noise
    
    # denoised_image를 CPU로 이동하여 이미지 저장
    denoised_image = denoised_image.cpu().squeeze(0)
    denoised_image = torch.clamp(denoised_image, 0, 1)  # 이미지 값을 0과 1 사이로 클램핑
    denoised_image = transforms.ToPILImage()(denoised_image)

    # Save denoised image
    output_filename = noisy_image_path[0]
    denoised_filename = output_path + '/' + output_filename.split('/')[-1][:-4] + '.png'
    denoised_image.save(denoised_filename) 
    
#    print(f'Saved denoised image: {denoised_filename}')




# -------------------------csv-------------------------------

folder_path = 'output'
output_file = 'output.csv'

# 폴더 내 이미지 파일 이름 목록을 가져오기
file_names = os.listdir(folder_path)
file_names.sort()

# CSV 파일을 작성하기 위해 오픈
with open(output_file, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['Image File', 'Y Channel Value'])

    for file_name in file_names:
        # 이미지 로드
        image_path = os.path.join(folder_path, file_name)
        image = cv2.imread(image_path)

        # 이미지를 YUV 색 공간으로 변환
        image_yuv = cv2.cvtColor(image, cv2.COLOR_BGR2YUV)

        # Y 채널 추출
        y_channel = image_yuv[:, :, 0]

        # Y 채널을 1차원 배열로 변환
        y_values = np.mean(y_channel.flatten())


        # 파일 이름과 Y 채널 값을 CSV 파일에 작성
        writer.writerow([file_name[:-4], y_values])

print('CSV file created successfully.')



