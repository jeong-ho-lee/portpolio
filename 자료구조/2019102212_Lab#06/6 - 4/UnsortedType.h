#pragma once

template<typename ItemType>
struct NodeType {
	ItemType info;
	NodeType* next;
};

template<typename ItemType>
class UnsortedType {
public:
	UnsortedType();
	~UnsortedType();
	void MakeEmpty();
	bool IsFull() const;
	int LengthIs() const;
	void RetrieveItem(ItemType& item, bool& found);
	void InsertItem(ItemType item);
	void DeleteItem(ItemType item);
	void ResetList();
	void GetNextItem(ItemType& item);
	void MergeLists(UnsortedType<ItemType>& other, UnsortedType<ItemType>& result);
private:
	NodeType<ItemType>* listData;
	int length;
	NodeType<ItemType>* currentPos;
};