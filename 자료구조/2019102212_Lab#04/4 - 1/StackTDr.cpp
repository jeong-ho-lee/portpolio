// Test driver
#include <iostream>
#include <fstream>

#include "StackTType.hpp"

using namespace std;

StackType<int> stackD(StackType<int> stackA) {
    StackType<int> stackB;
    StackType<int> stackC;

    while (!stackA.IsEmpty()) {
        int temp = stackA.Top();
        stackC.Push(temp);
        stackA.Pop();
    }

    while (!stackC.IsEmpty()) {
        int temp = stackC.Top();
        stackB.Push(temp);
        stackC.Pop();
    }

    return stackB;
}

class DoubleStack {
private:
    int top_small = -1;
    int top_big = MAX_ITEMS;
    int items[MAX_ITEMS];
public:
    void Push(int item) {
        if (!isFull()) {
            if (item <= 1000) {
                top_small++;
                items[top_small] = item;
            }
            else {
                top_big--;
                items[top_big] = item;
            }
        }
    }

    void Print() {
        for (int i = top_small; i > -1; i--) {
            cout << items[i] << endl;
        }

        cout << "\n";

        for (int j = top_big; j < MAX_ITEMS; j++) {
            cout << items[j] << endl;
        }
    }

    bool isFull() {
        return (top_small + 1) == top_big;
    }
};

int main()
{

    /* ifstream inFile;       // file containing operations
    ofstream outFile;      // file containing output
    string inFileName;     // input file external name
    string outFileName;    // output file external name
    string outputLabel;
    string command;        // operation to be executed

    int item;
    StackType<int> stack;
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
      try
      {
        if (command == "Push")
        {
          inFile >> item;
          stack.Push(item);
        }
        else if (command == "Pop")
          stack.Pop();
        else if (command == "Top")
        {
          item = stack.Top();
          outFile<< "Top element is " << item << endl;
        }
        else if (command == "IsEmpty")
          if (stack.IsEmpty())
            outFile << "Stack is empty." << endl;
          else
            outFile << "Stack is not empty." << endl;

        else if (command == "IsFull")
          if (stack.IsFull())
            outFile << "Stack is full." << endl;
          else outFile << "Stack is not full."  << endl;
      }
      catch (FullStack)
      {
        outFile << "FullStack exception thrown." << endl;
      }

      catch (EmptyStack)
      {
        outFile << "EmtpyStack exception thrown." << endl;
      }
      numCommands++;
      cout <<  " Command number " << numCommands << " completed."
           << endl;
      inFile >> command;

    };

    cout << "Testing completed."  << endl;
    inFile.close();
    outFile.close(); */

    //#1
    StackType<int> stack;

    stack.Push(1);
    stack.Push(3);
    stack.Push(4);
    stack.Push(5);
    stack.Push(6);

    while (!stack.IsEmpty()) {
        int result = stack.Top();
        stack.Pop();
        cout << result << endl;

    }

    cout << "\n";

    //#2
    StackType<int> stackA;
    StackType<int> stackB;
    
    stackA.Push(9);
    stackA.Push(8);
    stackA.Push(4);
    stackA.Push(7);
    stackA.Push(5);
    stackA.Push(3);

    stackB = stackD(stackA);

    while (!stackA.IsEmpty()) {
        int temp = stackA.Top();
        stackA.Pop();
        cout << temp << endl;
    }

    cout << "\n";

    while (!stackB.IsEmpty()) {
        int temp = stackB.Top();
        stackB.Pop();
        cout << temp << endl;
    }

    cout << "\n";

    //#3
    DoubleStack ds;

    ds.Push(1);
    ds.Push(2);
    ds.Push(3);
    ds.Push(4);
    ds.Push(1003);
    ds.Push(1004);
    ds.Push(1005);

    ds.Print();

    return 0;
}


