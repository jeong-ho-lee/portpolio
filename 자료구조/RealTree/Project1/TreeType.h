#include "QueType.h"
#include <string>
#include <iostream>

typedef char ItemType;
struct TreeNode;
enum OrderType { PRE_ORDER, IN_ORDER, POST_ORDER };

class TreeType
{
public:
    TreeType();
    ~TreeType();
    TreeType(const TreeType& originalTree);
    void operator=(const TreeType& originalTree);
    void MakeEmpty();
    bool IsEmpty() const;
    bool IsFull() const;
    int LengthIs() const;
    void RetrieveItem(ItemType& item, bool& found);
    void InsertItem(ItemType item);
    void DeleteItem(ItemType item);
    void ResetTree(OrderType order);
    void GetNextItem(ItemType& item, OrderType order, bool& finished);
    void Print() const;
    TreeNode* PtrToSuccessor1(TreeNode*& tree);
    TreeNode* PtrToSuccessor2(TreeNode*& tree);
    bool IsBst();
    int LeafCount();
    int SingleParentCount();
private:
    TreeNode* root;
    QueType preQue;
    QueType inQue;
    QueType postQue;
};