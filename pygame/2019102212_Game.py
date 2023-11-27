import pygame as P
from numpy import random as R

from time import sleep

def counting(count, count_type, count_text_y):
    global screen
    
    count_font = P.font.SysFont(None, 30)
    count_text = count_font.render(count_type + str(count), True, (255, 0, 0))
    screen.blit(count_text, (15, count_text_y))

def gameover(text_contents, text_y, image):
    global screen

    text_type = P.font.Font('freesansbold.ttf', 50)
    text_over = text_type.render(text_contents, True, (255, 0, 0))
    text_location = text_type.render(text_contents, True, (255, 0, 0)).get_rect()
    text_location.center = ((960/2),text_y)
    screen.blit(image, (0, 0))
    screen.blit(text_over, text_location)
    P.display.update()
    sleep(2)
    
background_width = 960
background_height = 540

aircraft_width = 100
aircraft_height = 38

helicopter_width = 90
helicopter_height = 52

missile_width = 63
missile_height = 44

def runGame():
    global screen, clock
    global background_image_1, background_image_2, aircraft_image, helicopter_image_1, helicopter_image_2, missile_image, bullet_image
    
    background_image_1_x = 0
    
    background_image_2_x = background_width

    aircraft_x = background_width * 0.05
    aircraft_y = background_height * 0.3
    
    aircraft_x_move = 0
    aircraft_y_move = 0
    
    aircraft_x_acceleration = 0
    aircraft_y_acceleration = 0

    helicopter_1_x = background_width
    helicopter_1_y = R.uniform(0, background_height * 0.3)
    
    helicopter_1_move = 3
    
    helicopter_2_x = background_width
    helicopter_2_y = R.uniform(0, background_height * 0.3)

    helicopter_2_move = 5

    missile_x = background_width
    missile_y = R.uniform(0, background_height * 0.3)
    
    missile_move = 3

    bullet_x = 0
    bullet_y = 0
    bullet_remove = False
    
    bullet_list = []

    shoot_helicopter_1 = False
    shoot_helicopter_1_count = 0
    
    shoot_helicopter_2 = False
    shoot_helicopter_2_count = 0

    score_count = 0

    level_count = 1

    item_count = 1

    combo = 0

    crashed = False
    
    while not crashed:
        for event in P.event.get():
            if event.type == P.QUIT:
                crashed = True

            if event.type == P.KEYDOWN:
                if event.key == P.K_LEFT:
                    aircraft_x_acceleration = -.5
                elif event.key == P.K_RIGHT:
                    aircraft_x_acceleration = .5
                elif event.key == P.K_UP:
                    aircraft_y_acceleration = -.5
                elif event.key == P.K_DOWN:
                    aircraft_y_acceleration = .5
                elif event.key == P.K_LCTRL:
                    bullet_x = aircraft_x + 100
                    bullet_y = aircraft_y + 20
                    bullet_list.append([bullet_x, bullet_y, bullet_remove])

            if event.type == P.KEYUP:
                if event.key == P.K_LEFT or event.key == P.K_RIGHT:
                    aircraft_x_acceleration = 0
                elif event.key == P.K_UP or event.key == P.K_DOWN:
                    aircraft_y_acceleration = 0

        screen.fill((255, 255, 255))

        background_image_1_x -= 5
        background_image_2_x -= 5

        if background_image_1_x == -background_width:
            background_image_1_x = background_width
            
        if background_image_2_x == -background_width:
            background_image_2_x = background_width

        aircraft_x_move += aircraft_x_acceleration
        aircraft_y_move += aircraft_y_acceleration

        if abs(aircraft_x_move) >= 7:
            aircraft_x_move = aircraft_x_move/abs(aircraft_x_move) * 7
        if abs(aircraft_y_move) >= 7:
            aircraft_y_move = aircraft_y_move/abs(aircraft_y_move) * 7

        if aircraft_x_acceleration == 0:
            aircraft_x_move *= 0.9
        if aircraft_y_acceleration == 0:
            aircraft_y_move *= 0.9
            
        if aircraft_x < 0:
            aircraft_x = 0
        elif aircraft_x > background_width * 0.9:
            aircraft_x = background_width * 0.9

        if aircraft_y < 0:
            aircraft_y = 0
        elif aircraft_y > background_height * 0.7:
            aircraft_y = background_height * 0.7

        aircraft_x += aircraft_x_move
        aircraft_y += aircraft_y_move

        helicopter_1_move += 0.1
        helicopter_1_x -= helicopter_1_move
        
        if helicopter_1_x <= -50:
            helicopter_1_x = background_width
            helicopter_1_y = R.uniform(0, background_height * 0.7)
            helicopter_1_move = 3
            combo = 0
            
        if score_count >= 10000:
            level_count = 2
            helicopter_2_move += 0.2
            helicopter_2_x -= helicopter_2_move
        
            if helicopter_2_x <= -50:
                helicopter_2_x = background_width
                helicopter_2_y = R.uniform(0, background_height * 0.7)
                helicopter_2_move = 5
                combo = 0

        if score_count >= 20000:
            item_count = 2

        if score_count >= 30000:
            level_count = 3
            missile_move += 0.5
            missile_x -= missile_move

        if score_count >= 50000:
            item_count = 3
        
            if missile_x <= -5000:
                missile_x = background_width
                missile_y = R.uniform(0, background_height * 0.7)
                missile_move = 3

        if len(bullet_list) != 0:
            for i, bullet in enumerate(bullet_list):
                bullet[0] += 30
                bullet_list[i][0] = bullet[0]
                
                if bullet[0] > helicopter_1_x:
                    if item_count == 1:
                        if bullet[1] > helicopter_1_y and bullet[1] < helicopter_1_y + helicopter_height:
                            score_count += 500 * (combo + 1)
                            bullet_list[i][2] = True
                            shoot_helicopter_1 = True
                            combo += 1

                    elif item_count == 2:
                        if bullet[1] > helicopter_1_y and bullet[1] < helicopter_1_y + helicopter_height or\
                           bullet[1] + 30 > helicopter_1_y and bullet[1] + 30 < helicopter_1_y + helicopter_height:
                            score_count += 500 * (combo + 1)
                            bullet_list[i][2] = True
                            shoot_helicopter_1 = True
                            combo += 1

                    elif item_count == 3:
                        if bullet[1] > helicopter_1_y and bullet[1] < helicopter_1_y + helicopter_height or\
                           bullet[1] + 30 > helicopter_1_y and bullet[1] + 30 < helicopter_1_y + helicopter_height or\
                           bullet[1] - 30 > helicopter_1_y and bullet[1] - 30 < helicopter_1_y + helicopter_height:
                            score_count += 500 * (combo + 1)
                            bullet_list[i][2] = True
                            shoot_helicopter_1 = True
                            combo += 1

                if bullet[0] > helicopter_2_x:
                    if item_count == 1:
                        if bullet[1] > helicopter_2_y and bullet[1] < helicopter_2_y + helicopter_height:
                            score_count += 500 * (combo + 1)
                            bullet_list[i][2] = True
                            shoot_helicopter_2 = True
                            combo += 1

                    elif item_count == 2:
                        if bullet[1] > helicopter_2_y and bullet[1] < helicopter_2_y + helicopter_height or\
                           bullet[1] + 30 > helicopter_2_y and bullet[1] + 30 < helicopter_2_y + helicopter_height:
                            score_count += 500 * (combo + 1)
                            bullet_list[i][2] = True
                            shoot_helicopter_2 = True
                            combo += 1

                    elif item_count == 3:
                        if bullet[1] > helicopter_2_y and bullet[1] < helicopter_2_y + helicopter_height or\
                           bullet[1] + 30 > helicopter_2_y and bullet[1] + 30 < helicopter_2_y + helicopter_height or\
                           bullet[1] - 30 > helicopter_2_y and bullet[1] - 30 < helicopter_2_y + helicopter_height:
                            score_count += 500 * (combo + 1)
                            bullet_list[i][2] = True
                            shoot_helicopter_2 = True
                            combo += 1

                if bullet[0] >= background_width:
                    try:
                        bullet_list[i][2] = True
                    except:
                        pass

            for bullet in bullet_list:
                if bullet[2]:
                    bullet_list.remove(bullet)
                    
        if aircraft_x + aircraft_width > helicopter_1_x and aircraft_x < helicopter_1_x + helicopter_width:
            if (aircraft_y > helicopter_1_y and aircraft_y < helicopter_1_y + helicopter_height) or\
               (aircraft_y + aircraft_height > helicopter_1_y and aircraft_y + aircraft_height < helicopter_1_y + helicopter_height):
                gameover('SCORE : ' + str(score_count), 480, gameover_image)
                gameover('GAME START!', 200, background_image_1)
                score_count = 0
                runGame()

        if aircraft_x + aircraft_width > helicopter_2_x and aircraft_x < helicopter_2_x + helicopter_width:
            if (aircraft_y > helicopter_2_y and aircraft_y < helicopter_2_y + helicopter_height) or\
               (aircraft_y + aircraft_height > helicopter_2_y and aircraft_y + aircraft_height < helicopter_2_y + helicopter_height):
                gameover('SCORE : ' + str(score_count), 480, gameover_image)
                gameover('GAME START!', 200, background_image_1)
                score_count = 0
                runGame()

        if aircraft_x + aircraft_width > missile_x and aircraft_x < missile_x + missile_width:
            if (aircraft_y > missile_y and aircraft_y < missile_y + missile_height) or\
               (aircraft_y + aircraft_height > missile_y and aircraft_y + aircraft_height < missile_y + missile_height):
                gameover('SCORE : ' + str(score_count), 480, gameover_image)
                gameover('GAME START!', 200, background_image_1)
                score_count = 0
                runGame()

        screen.blit(background_image_1, (background_image_1_x, 0))
        screen.blit(background_image_2, (background_image_2_x, 0))
        screen.blit(aircraft_image, (aircraft_x, aircraft_y))
        screen.blit(missile_image, (missile_x, missile_y))

        if len(bullet_list) != 0:
            for bullet_x, bullet_y, _ in bullet_list:
                if item_count == 1:
                    screen.blit(bullet_image, (bullet_x, bullet_y))
                elif item_count == 2:
                    screen.blit(bullet_image, (bullet_x, bullet_y))
                    screen.blit(bullet_image, (bullet_x, bullet_y + 30))
                elif item_count == 3:
                    screen.blit(bullet_image, (bullet_x, bullet_y))
                    screen.blit(bullet_image, (bullet_x, bullet_y + 30))
                    screen.blit(bullet_image, (bullet_x, bullet_y - 30))

        counting(score_count, 'SCORE : ', 15)
        counting(level_count, 'LEVEL : ', 40)
        counting(item_count, 'ITEM LEVEL : ', 65)
        counting(combo, 'COMBO : ', 90)

        if not shoot_helicopter_1:
            screen.blit(helicopter_image_1, (helicopter_1_x, helicopter_1_y))
        else:
            screen.blit(shoot_image, (helicopter_1_x, helicopter_1_y))
            shoot_helicopter_1_count += 1
            if shoot_helicopter_1_count > 5:
                shoot_helicopter_1_count = 0
                helicopter_1_x = background_width
                helicopter_1_y = R.uniform(0, background_height * 0.3)
                shoot_helicopter_1 = False

        if not shoot_helicopter_2:
            screen.blit(helicopter_image_2, (helicopter_2_x, helicopter_2_y))
        else:
            screen.blit(shoot_image, (helicopter_2_x, helicopter_2_y))
            shoot_helicopter_2_count += 1
            if shoot_helicopter_2_count > 5:
                shoot_helicopter_2_count = 0
                helicopter_2_x = background_width
                helicopter_2_y = R.uniform(0, background_height * 0.3)
                shoot_helicopter_2 = False
            
        P.display.update()
        clock.tick(60)

    P.quit()
    
def initGame():
    global screen, clock
    global background_image_1, background_image_2, aircraft_image, helicopter_image_1, helicopter_image_2, missile_image, bullet_image, shoot_image, gameover_image

    P.init()
    P.display.set_caption('2019102212_Game')
    screen = P.display.set_mode((background_width, background_height))
    background_image_1 = P.image.load('resources\\background.jpg')
    background_image_2 = background_image_1.copy()
    aircraft_image = P.image.load('resources\\aircraft.png')
    helicopter_image_1 = P.image.load('resources\\helicopter_1.png')
    helicopter_image_2 = P.image.load('resources\\helicopter_2.png')
    missile_image = P.image.load('resources\\missile.png')
    bullet_image = P.image.load('resources\\bullet.png')
    shoot_image = P.image.load('resources\\shoot.png')
    gameover_image = P.image.load('resources\\gameover.png')

    clock = P.time.Clock()
    gameover('GAME START!', 200, background_image_1)
    runGame()

initGame()
