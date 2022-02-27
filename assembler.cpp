/* TODO:
    -make a table with all code and reg values in binary string
    -bring in the binary converty for nums
    - */
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <stack>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

// Table of all known command and reg values in bitstring
unordered_map<string, string> umap{
    /* registers */
    {"R1", "000"},
    {"R2", "001"},
    {"R3", "010"},
    {"R4", "011"},
    {"R5", "100"},
    {"R6", "101"},
    {"RS", "110"},
    {"RB", "111"},
    /* instructions  */
    {"mov", "0000"},
    {"add", "0001"},
    {"cmp", "0010"},
    {"push", "0011"},
    {"pop", "0100"},
    {"call", "0101"},
    {"je", "0110"},
    {"jge", "0111"},
    {"jl", "1000"},
    {"j", "1001"},
    {"ret", "1010"},
    {"nop", "1011"},
    /* addresing modes  */
    {"immediate", "00"},
    {"register", "01"},
    {"direct", "10"},
    {"indexed", "11"},
    

};

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
    for (i = 0; i < (int)inputString.size(); i++) // main loop of proccesing
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
                    if (compareLabel == currentString)
                    {
                        return 1;
                    }
                }
                tempLabel.push_back(currentString); /* this is a comment */
            }
            // here we do the actual proccesing of figuring out the value of the label by just looking at the adress of the next thing
            // the problem i see here is we don't really know the byte adress
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
