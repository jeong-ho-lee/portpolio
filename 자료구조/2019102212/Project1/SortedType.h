#pragma once
template <class ItemType>
struct NodeType;

template <class ItemType>
class SortedType
{
public:
    SortedType();
    ~SortedType();
    bool IsFull() const;
    int  LengthIs() const;
    void MakeEmpty();
    void RetrieveItem(ItemType& item, bool& found);
    void InsertItem(ItemType item);
    void DeleteItem(ItemType item);
    void ResetList();
    void GetNextItem(ItemType&);
    void MergeLists(SortedType<ItemType>& other, SortedType<ItemType>& result);
private:
    NodeType<ItemType>* listData;
    int length;
    NodeType<ItemType>* currentPos;
};
template<class ItemType>
struct NodeType
{
    ItemType info;
    NodeType* next;
};
template <class ItemType>
SortedType<ItemType>::SortedType()
{
    length = 0;
    listData = NULL;
}
template<class ItemType>
bool SortedType<ItemType>::IsFull() const
{
    NodeType<ItemType>* location;
    /*try
    {*/
    location = new NodeType<ItemType>;
    delete location;
    return false;
    /*}
    catch(bad_alloc exception)
    {
      return true;
    }*/
}
template <class ItemType>
int SortedType<ItemType>::LengthIs() const
{
    return length;
}
template <class ItemType>
void SortedType<ItemType>::MakeEmpty()
{
    NodeType<ItemType>* tempPtr;

    while (listData != NULL)
    {
        tempPtr = listData;
        listData = listData->next;
        delete tempPtr;
    }
    length = 0;
}
template <class ItemType>
void SortedType<ItemType>::RetrieveItem(ItemType& item,
    bool& found)
{
    bool moreToSearch;
    NodeType<ItemType>* location;

    location = listData;
    found = false;
    moreToSearch = (location != NULL);

    while (moreToSearch && !found)
    {
        if (location->info < item)
        {
            location = location->next;
            moreToSearch = (location != NULL);
        }
        else if (item == location->info)
        {
            found = true;
            item = location->info;
        }
        else
            moreToSearch = false;
    }
}

template <class ItemType>
void SortedType<ItemType>::InsertItem(ItemType item)
{
    NodeType<ItemType>* newNode;
    NodeType<ItemType>* predLoc;
    NodeType<ItemType>* location;
    bool moreToSearch;

    location = listData;
    predLoc = NULL;
    moreToSearch = (location != NULL);

    while (moreToSearch)
    {
        if (location->info < item)
        {
            predLoc = location;
            location = location->next;
            moreToSearch = (location != NULL);
        }
        else
            moreToSearch = false;
    }

    newNode = new NodeType<ItemType>;
    newNode->info = item;
    if (predLoc == NULL)
    {
        newNode->next = listData;
        listData = newNode;
    }
    else
    {
        newNode->next = location;
        predLoc->next = newNode;
    }
    length++;
}

template <class ItemType>
void SortedType<ItemType>::DeleteItem(ItemType item) {
    bool moreToSearch;
    RetrieveItem(item, moreToSearch);

    while (moreToSearch) {
        NodeType<ItemType>* location = listData;
        NodeType<ItemType>* tempLocation;

        if (item == listData->info)
        {
            tempLocation = location;
            listData = listData->next;
        }
        else
        {
            while (!(item == (location->next)->info))
                location = location->next;

            tempLocation = location->next;
            location->next = (location->next)->next;
        }
        delete tempLocation;
        length--;
        RetrieveItem(item, moreToSearch);
    }
}
template <class ItemType>

void SortedType<ItemType>::ResetList()
{
    currentPos = NULL;
}

template <class ItemType>
void SortedType<ItemType>::GetNextItem(ItemType& item)
{
    if (currentPos == NULL)
        currentPos = listData;
    item = currentPos->info;
    currentPos = currentPos->next;

}

template <class ItemType>
SortedType<ItemType>::~SortedType()
{
    NodeType<ItemType>* tempPtr;

    while (listData != NULL)
    {
        tempPtr = listData;
        listData = listData->next;
        delete tempPtr;
    }
}

template <typename ItemType>
void SortedType<ItemType>::MergeLists(SortedType<ItemType>& other, SortedType<ItemType>& result) {
    NodeType<ItemType>* ptr1 = listData;
    NodeType<ItemType>* ptr2 = other.listData;

    while (!ptr1 == NULL && !ptr2 == NULL) {
        if (ptr1->info >= ptr2->info) {
            result.InsertItem(ptr1->info);
            ptr1 = ptr1->next;
        }
        else {
            result.InsertItem(ptr2->info);
            ptr2 = ptr2->next;
        }
    }

    if (ptr1 == NULL) {
        while (!ptr2 == NULL) {
            result.InsertItem(ptr2->info);
            ptr2 = ptr2->next;
        }
    }
    else {
        while (!ptr1 == NULL) {
            result.InsertItem(ptr1->info);
            ptr1 = ptr1->next;
        }
    }
}