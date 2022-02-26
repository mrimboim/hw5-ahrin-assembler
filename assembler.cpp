#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <stack>
#include <vector>
#include <string.h>


unordered_map<string, int> umap;
 
    // inserting values by using [] operator
    umap["GeeksforGeeks"] = 10;
    umap["Practice"] = 20;
   umap["Contribute"] = 30;


using namespace std;

// struct labels{
//     int pos;
//     string value;
// }
// struct variable{
//     int pos;

// }
// struct instruction{

// }
// struct data{
//     string type;
//     labels label;
//     variable var;
//     instruction ins;
// }

int openFile(string fileName, vector<string> &buffer) // later add pass by ref for data struct
{
    string line;
    ifstream file(fileName);

    if (file.is_open()) // if statments to check size of each line and number of lines and close file at the end
    {
        while (getline(file, line))
        {

            buffer.push_back(line);
        }
        file.close();
        return 0;
    }
    else
    {
        return 1;
    }
}

int parseProccess(vector<string> inputString)
{
    // check for labels
    vector<string> tempLabel; // while parsing check to see that the current label is not contained in the other ones
    int i;
    for (i = 0; i < (int)inputString.size(); i++)//main loop of proccesing 
    {
        string currentString = inputString[i];
        // three casses where one is label one is var and one is instruction
        if (currentString.at(0) != ' ') // label clause
        {
            if (tempLabel.empty())
            {
                tempLabel.push_back(currentString);
            }
            else
            {
                int j;
                for (j = 0; j < (int)tempLabel.size(); j++)
                {
                      string compareLabel = tempLabel[j];
                      if(compareLabel == currentString)
                      {
                          return 1;
                      }
                    }
                    tempLabel.push_back(currentString);
            }
            //here we do the actual proccesing of figuring out the value of the label by just looking at the adress of the next thing
            //the problem i see here is we don't really know the byte adress
        }
   

    }
    return 0;
}

// MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN
int main(int argc, char *argv[])
{
    // intial command line validation
    if (argc < 2)
    {
        cout << "Wrong number of arguments." << '\n';
        return 1;
    }
    else if (argc > 2)
    {
        cout << "Wrong number of arguments." << '\n';
        return 1;
    }

    /*                       OPEN FILE
    ----------------------------------------------------------------*/
    string fileName(argv[1]);                             // save filename to a string var, run it through the file function
    vector<string> inputString;                           // main buffer of program
    int openFileStatus = openFile(fileName, inputString); // returns 0 for good and 1, 2, 3 for each type of failure

    if (openFileStatus == 1)
    {
        cout << "Failed to open file." << '\n';
        return 2;
    }

    /*                      PROCCESING
    --------------------------------------------------------------*/
    int parseStatus = parseProccess(inputString);

    if (parseStatus == 1)
    {
        cout << "At least one duplicate label found." << '\n';
        return 3;
    }

    /*                          PRINTING
    --------------------------------------------------------------*/

    /*                          EXIT
    --------------------------------------------------------------*/
    return 0;
}

/* To DO:
- make the open file be able to check if the file has any duplicate labels
- it can work by making the instructions a vector of strings and then making the main thing a vector of vectors of strings
- make a proccesing function that takes each line of the vector of string and puts it into  a structre of string, type of overarching instrution,
spefici instruction, and possibly operands. */