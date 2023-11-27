// Test driver
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstring>

#include "SortedType.h"

using namespace std;
void PrintList(ofstream& outFile, SortedType<int>& list);
//3 - A
template <typename ItemType>
void MergeLists(SortedType<ItemType>& l_a, SortedType<ItemType>& l_b, SortedType<ItemType>& result) {
    l_a.ResetList();
    for (int i = l_a.LengthIs(); i; i--) {
        ItemType item;
        l_a.GetNextItem(item);
        result.InsertItem(item);
    }

    l_b.ResetList();
    for (int i = l_b.LengthIs(); i; i--) {
        ItemType item;
        l_b.GetNextItem(item);
        result.InsertItem(item);
    }
}

int main()
{

  /*ifstream inFile;       // file containing operations
  ofstream outFile;      // file containing output
  string inFileName;     // input file external name
  string outFileName;    // output file external name
  string outputLabel;     
  string command;        // operation to be executed
  
  int item;
  SortedType<int> list;
  bool found;
  int numCommands;


  // Prompt for file names, read file names, and prepare files
  cout << "Enter name of input command file; press return." << endl;
  cin  >> inFileName;
  inFile.open(inFileName.c_str());

  cout << "Enter name of output file; press return." << endl;
  cin  >> outFileName;
  outFile.open(outFileName.c_str());

  cout << "Enter name of test run; press return." << endl;
  cin  >> outputLabel;
  outFile << outputLabel << endl;

  inFile >> command;

  numCommands = 0;
  while (command != "Quit")
  { 
    if (command == "InsertItem")
    {
      inFile >> item; 
      list.InsertItem(item);
      outFile << item;
      outFile << " is inserted" << endl;
    }
    else if (command == "DeleteItem")
    {
      inFile >> item;
      list.DeleteItem(item);
      outFile << item;
      outFile << " is deleted" << endl;
    }
    else if (command == "RetrieveItem")
    {
      inFile >> item;

      list.RetrieveItem(item, found);
      if (found)
        outFile << item << " found in list." << endl;
      else outFile << item  << " not in list."  << endl;  
    } 
    else if (command == "LengthIs")  
      outFile << "Length is " << list.LengthIs() << endl;
    else if (command == "IsFull")
      if (list.IsFull())
        outFile << "List is full." << endl;
      else outFile << "List is not full."  << endl;  
  
    else PrintList(outFile, list);
    numCommands++;
    cout <<  " Command " << command << " completed." 
         << endl;
    inFile >> command;
  }
 
  cout << "Testing completed."  << endl;
  inFile.close();
  outFile.close();*/
    SortedType<int> l_a;
    SortedType<int> l_b;
    SortedType<int> result;

    l_a.InsertItem(1);
    l_a.InsertItem(3);
    l_a.InsertItem(5);
    l_a.InsertItem(7);
    l_a.InsertItem(9);

    l_b.InsertItem(2);
    l_b.InsertItem(4);
    l_b.InsertItem(6);
    l_b.InsertItem(8);
    l_b.InsertItem(10);

    MergeLists(l_a, l_b, result);
    result.ResetList();

    for (int i = result.LengthIs(); i; i--) {
        int item;
        result.GetNextItem(item);
        cout << item << "\n";
    }

    cout << "\n";

    result.MakeEmpty();
    l_a.MergeLists(l_b, result);

    for (int i = result.LengthIs(); i; i--) {
        int item;
        result.GetNextItem(item);
        cout << item << "\n";
    }

    cout << "\n";

    result.InsertItem(1);
    result.DeleteItem(1);
    result.ResetList();

    for (int i = result.LengthIs(); i; i--) {
        int item;
        result.GetNextItem(item);
        cout << item << "\n";
    }

  return 0;
}


void PrintList(ofstream& dataFile, SortedType<int>& list)
// Pre:  list has been initialized.      
//       dataFile is open for writing.   
// Post: Each component in list has been written to dataFile.
//       dataFile is still open.         
{
  int length;
  int item;
  list.ResetList();
  length = list.LengthIs();
  dataFile << "List Items: " << endl;
  for (int counter = 1; counter <= length; counter++)
  {
    list.GetNextItem(item);
    dataFile << item << endl;
  }
  dataFile << endl;
}


