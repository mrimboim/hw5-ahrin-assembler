/* TODO:
    -
    - */
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
// #include <stack>
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

// Opens file given and puts the lines into a vector of strings 
int openFile(string fileName, vector<string> &buffer) 
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
/*-------Here we do validation of double label-----*/
    /* holds only the label string elements. Will be used for double label check*/
    vector<string> tempLabelHolder; 
    int i;
    for (i = 0; i < (int)inputString.size(); i++) // main loop of proccesing
    {
        string currentInputString = inputString[i];
        // three casses where one is label one is var and one is instruction
        if (currentInputString.at(0) != ' ') // checks if its a label clause
        {
            if (tempLabelHolder.empty())
            {
                tempLabelHolder.push_back(currentInputString);
            }
            else
            {
                int j;
                for (j = 0; j < (int)tempLabelHolder.size(); j++)
                {
                    string knownLabel = tempLabelHolder[j];
                    if (knownLabel == currentInputString)
                    {
                        return 1;
                    }
                }
                tempLabelHolder.push_back(currentInputString); /* this is a comment */
            }
            
        }
    }
/*-----Now that double label check we can do first pass*/    
    return 0;
}

void numToBinary(unsigned int input)
{
    string binaryRep (32, '0');
    unsigned int compareOne = 1;/// this will give us a 32 bit mask with a 1 in the leftmost place which we will move to each index to check for value of bit 
    
    int i;
    for (i = 0; i < 32; i++)
    {
        //printf("The compareONE is currently %u\n", (compareOne << (31 - i)));
        if((input & (compareOne << (31 - i) )) == 0)//checking each digit and then shifting left 31 - i  times so that i input the array in the correct order with msb rightmost
        {
            binaryRep[i] = '0';
        }else{//if the AND bitwise operator is anything but zero it means there is a 1 in that location or index in the bit string/array 
            binaryRep[i] = '1';
        }
    }
    // cout << "binaryRep: " << binaryRep << '\n'; //debugComment 
    // cout << "binary#10: 00000000000000000000000000001010" <<   '\n'; //debugComment 
    // cout << "Size of binary rep: " << binaryRep.size() << '\n'; //debugComment   


}

/* Runs open file and parse proccesor  */
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
