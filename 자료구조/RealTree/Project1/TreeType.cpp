#include "TreeType.h"

struct TreeNode {
    ItemType info;
    TreeNode* left;
    TreeNode* right;
};

bool TreeType::IsFull() const {
    TreeNode* location;
    try {
        location = new TreeNode;
        delete location;

        return false;
    } catch (std::bad_alloc exception) {
        return true;
    }
}

bool TreeType::IsEmpty() const {
    return root == NULL;
}

int CountNodes(TreeNode* tree);
int TreeType::LengthIs() const {
    return CountNodes(root);
}
int CountNodes(TreeNode* tree) {
    if (tree == NULL) return 0;
    else return CountNodes(tree->left) + CountNodes(tree->right) + 1;
}

void Retrieve(TreeNode* tree, ItemType& item, bool& found);
void TreeType::RetrieveItem(ItemType& item, bool& found) {
    Retrieve(root, item, found);
}
void Retrieve(TreeNode* tree, ItemType& item, bool& found) {
    if (tree == NULL) found = false;
    else if (item < tree->info) Retrieve(tree->left, item, found);
    else if (item > tree->info) Retrieve(tree->right, item, found);
    else {
        item = tree->info;
        found = true;
    }
}

void Insert(TreeNode*& tree, ItemType item);
void TreeType::InsertItem(ItemType item) {
    Insert(root, item);
}
void Insert(TreeNode*& tree, ItemType item) {
    if (tree == NULL) {
        tree = new TreeNode;
        tree->right = NULL;
        tree->left = NULL;
        tree->info = item;
    } else if (item < tree->info) Insert(tree->left, item);
    else Insert(tree->right, item);
}

void DeleteNode(TreeNode*& tree);
void Delete(TreeNode*& tree, ItemType item);
void TreeType::DeleteItem(ItemType item) {
    Delete(root, item);
}
void Delete(TreeNode*& tree, ItemType item) {
    if (item < tree->info) Delete(tree->left, item);
    else if (item > tree->info) Delete(tree->right, item);
    else DeleteNode(tree);
}
void GetPredecessor(TreeNode* tree, ItemType& data);
void PtrToSuccessor(TreeNode*& tree, ItemType& data);
void DeleteNode(TreeNode*& tree) {
    ItemType data;
    TreeNode* tempPtr;

    tempPtr = tree;
    if (tree->left == NULL) {
        tree = tree->right;
        delete tempPtr;
    } else if (tree->right == NULL) {
        tree = tree->left;
        delete tempPtr;
    } else {
        PtrToSuccessor(tree->right, data);
        tree->info = data;
        Delete(tree->right, data);
    }
}

void GetPredecessor(TreeNode* tree, ItemType& data) {
    while (tree->right != NULL) tree = tree->right;
    data = tree->info;
}

void PrintTree(TreeNode* tree) {
    if (tree != NULL) {
        PrintTree(tree->left);
        std::cout << tree->info;
        PrintTree(tree->right);
    }
}

void TreeType::Print() const {
    PrintTree(root);
}

TreeType::TreeType() {
    root = NULL;
}

void Destroy(TreeNode*& tree);
TreeType::~TreeType() {
    Destroy(root);
}
void Destroy(TreeNode*& tree) {
    if (tree != NULL) {
        Destroy(tree->left);
        Destroy(tree->right);
        delete tree;
    }
}

void TreeType::MakeEmpty() {
    Destroy(root);
    root = NULL;
}

void CopyTree(TreeNode*& copy, const TreeNode* originalTree);
TreeType::TreeType(const TreeType& originalTree) {
    CopyTree(root, originalTree.root);
}
void TreeType::operator=(const TreeType& originalTree) {
    if (&originalTree == this) return;
    Destroy(root);
    CopyTree(root, originalTree.root);
}

void CopyTree(TreeNode*& copy, const TreeNode* originalTree) {
    if (originalTree == NULL) copy = NULL;
    else {
        copy = new TreeNode;
        copy->info = originalTree->info;
        CopyTree(copy->left, originalTree->left);
        CopyTree(copy->right, originalTree->right);
    }
}

void PreOrder(TreeNode*, QueType&);
void InOrder(TreeNode*, QueType&);
void PostOrder(TreeNode*, QueType&);

void TreeType::ResetTree(OrderType order) {
    switch (order) {
    case PRE_ORDER:
        PreOrder(root, preQue);
        break;
    case IN_ORDER:
        InOrder(root, inQue);
        break;
    case POST_ORDER:
        PostOrder(root, postQue);
        break;
    }
}
void PreOrder(TreeNode* tree, QueType& preQue) {
    if (tree != NULL) {
        preQue.Enqueue(tree->info);
        PreOrder(tree->left, preQue);
        PreOrder(tree->right, preQue);
    }
}
void InOrder(TreeNode* tree, QueType& inQue) {
    if (tree != NULL) {
        InOrder(tree->left, inQue);
        inQue.Enqueue(tree->info);
        InOrder(tree->right, inQue);
    }
}
void PostOrder(TreeNode* tree, QueType& postQue) {
    if (tree != NULL) {
        PostOrder(tree->left, postQue);
        PostOrder(tree->right, postQue);
        postQue.Enqueue(tree->info);
    }
}
void TreeType::GetNextItem(ItemType& item, OrderType order, bool& finished) {
    finished = false;

    switch (order) {
    case PRE_ORDER:
        preQue.Dequeue(item);
        if (preQue.IsEmpty()) finished = true;
        break;
    case IN_ORDER:
        inQue.Dequeue(item);
        if (inQue.IsEmpty()) finished = true;
        break;
    case  POST_ORDER:
        postQue.Dequeue(item);
        if (postQue.IsEmpty()) finished = true;
        break;
    }
}

TreeNode* TreeType::PtrToSuccessor1(TreeNode*& tree) {
    if (tree->left != NULL) return PtrToSuccessor1(tree->left);
    else {
        TreeNode* tempPtr = tree;

        tree = tree->right;

        return tempPtr;
    }
}
TreeNode* TreeType::PtrToSuccessor2(TreeNode*& tree) {
    while (tree->left != NULL) tree = tree->left;

    TreeNode* tempPtr = tree;

    tree = tree->right;

    return tempPtr;
}
void PtrToSuccessor(TreeNode*& tree, ItemType& data) {
    while (tree->left != NULL) tree = tree->left;

    data = tree->info;
}

bool Imp_IsBst(TreeNode* tree, ItemType& min, ItemType& max);
bool TreeType::IsBst() {
    ItemType min, max;

    return Imp_IsBst(root, min, max);
}

bool Imp_IsBst(TreeNode* tree, ItemType& min, ItemType& max) {
    bool isBst;
    ItemType left_min, left_max;
    ItemType right_min, right_max;

    PtrToSuccessor(tree->left, left_min);
    GetPredecessor(tree->left, left_max);
    PtrToSuccessor(tree->right, right_min);
    GetPredecessor(tree->right, right_max);

    if (tree == NULL) return true;
    if (tree->left != NULL) {
        isBst = Imp_IsBst(tree->left, left_min, left_max);

        if (!isBst || tree->info <= tree->left->info) return false;
    }
    if (tree->right != NULL) {
        isBst = Imp_IsBst(tree->right, right_min, right_max);
        if (!isBst || tree->info >= tree->right->info) return false;
    }

    return true;
}

int Imp_LeafCount(TreeNode* tree);
int TreeType::LeafCount() {
    return Imp_LeafCount(root);
}
int Imp_LeafCount(TreeNode* tree) {
    if (tree == NULL) return 0;
    else if (tree->left == NULL && tree->right == NULL) return 1;
    else return Imp_LeafCount(tree->left) + Imp_LeafCount(tree->right);
}

int Imp_SingleParentCount(TreeNode* tree);
int TreeType::SingleParentCount() {
    return Imp_SingleParentCount(root);
}

int Imp_SingleParentCount(TreeNode* tree) {
    if (tree == NULL) return 0;
    else if (tree->left == NULL && tree->right != NULL) return Imp_SingleParentCount(tree->right) + 1;
    else if (tree->right == NULL && tree->left != NULL) return Imp_SingleParentCount(tree->left) + 1;
    else return Imp_SingleParentCount(tree->left) + Imp_SingleParentCount(tree->right);
}