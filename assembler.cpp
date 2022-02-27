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
#include <limits.h>
#include <cctype>

using namespace std;

int lastByte;
int currentByte = 0; 
// Table of all known command and reg values in bitstring
unordered_map<string, string> conversionTable{
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
    {"im", "00"},
    {"reg", "01"},
    {"dir", "10"},
    {"ind", "11"}, // there is a structre to instructions so you know you will need to look at operand to get mode,
    // meaning that the first non var thing is instruction then operand (where we can get  the type of mode from)
};

/* contains the intail zero value of table and then actual value all in string form */
unordered_map<string, string> labelTable;

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
            cout << line << '\n'; // debugComment
        }
        file.close();
        return 0;
    }
    else
    {
        return 1;
    }
}

string numToBinary(int input)
{
    string binaryRep(32, '0');
    unsigned int conversionToUI = (UINT_MAX & input);
    unsigned int compareOne = 1; /// this will give us a 32 bit mask with a 1 in the leftmost place which we will move to each index to check for value of bit

    int i;
    for (i = 0; i < 32; i++)
    {
        // printf("The compareONE is currently %u\n", (compareOne << (31 - i)));
        if ((conversionToUI & (compareOne << (31 - i))) == 0) // checking each digit and then shifting left 31 - i  times so that i input the array in the correct order with msb rightmost
        {
            binaryRep[i] = '0';
        }
        else
        { // if the AND bitwise operator is anything but zero it means there is a 1 in that location or index in the bit string/array
            binaryRep[i] = '1';
        }
    }
    // cout << "binaryRep: " << binaryRep << '\n'; //debugComment
    // cout << "binary#10: 00000000000000000000000000001010" <<   '\n'; //debugComment
    // cout << "Size of binary rep: " << binaryRep.size() << '\n'; //debugComment
    return binaryRep;
}

string convertOper(string command){
    string commandBitStringWithModeCode;
    string value;
    /* Immediate */
    if(command.at(0) == '$'){
        commandBitStringWithModeCode = conversionTable["im"];
        command = command.substr(1);
        if(isalpha(command[0])){
            value = labelTable[command];
            commandBitStringWithModeCode += value;
        }else{
            value = numToBinary(stoi(command));
            commandBitStringWithModeCode += value;
        }
    }else if(command == "R1" || command == "R2" || command == "R3" || command == "R4" || command == "R5" || command == "R6" || command == "RS" || command == "RB"){
        commandBitStringWithModeCode = conversionTable["reg"];
        value = conversionTable[command];
        commandBitStringWithModeCode += value;
    }else if(isalpha(command[0])){
        commandBitStringWithModeCode = conversionTable["dir"];
        value = labelTable[command];
        commandBitStringWithModeCode += value;
    }else{
        commandBitStringWithModeCode = conversionTable["ind"];
        size_t pos = 0;
        string token;
        pos = command.find("(");
        token = command.substr(0, pos);
        value = numToBinary(stoi(token));
        command.erase(0, pos + 1);

        pos = command.find(")");
        token = command.substr(0, pos);
        value += conversionTable[token];

        commandBitStringWithModeCode += value;
       

    }


    return commandBitStringWithModeCode;
}
string convertToMachine(string inputString)
{
    string bitString;
    inputString.erase(0, 4);
    // cout << "String without whitespace:" << inputString <<  '\n'; //debugComment
    if (inputString.at(0) == 'r')
    {
        bitString = conversionTable["ret"];
    }
    else if (inputString.at(0) == 'n')
    {
        bitString = conversionTable["nop"];
    }
    else
    {
        size_t pos = 0;
        vector<string> command;
        string token;
        while ((pos = inputString.find(" ")) != string::npos)
        {
            token = inputString.substr(0, pos);
            command.push_back(token);
            inputString.erase(0, pos + 1);
        }
        command.push_back(inputString);
        // this is  for all other instucts so here we need to tokenize instruction or var
        if (command.at(0) == "var")
        {
            bitString = conversionTable["var"];
            int var = stoi(command.at(1));
            string oparand1 = numToBinary(var);
            bitString = bitString + oparand1;
        }
        else
        {
            if (command.size() == 2)
            {
                string oper1 = convertOper(command.at(1));

                if (command.at(0) == "push")
                {
                    bitString = conversionTable["push"] + oper1;
                }
                else if (command.at(0) == "pop")
                {
                    bitString = conversionTable["pop"] + oper1;
                }
                else if (command.at(0) == "call")
                {
                    bitString = conversionTable["call"] + oper1;
                }
                else if (command.at(0) == "je")
                {
                    bitString = conversionTable["je"] + oper1;
                }
                else if (command.at(0) == "jge")
                {
                    bitString = conversionTable["jge"] + oper1;
                }
                else if (command.at(0) == "jl")
                {
                    bitString = conversionTable["jl"] + oper1;
                }
                else if (command.at(0) == "j")
                {
                    bitString = conversionTable["j"] + oper1;
                }
            }
            else
            {
                string oper1 = convertOper(command.at(1));
                string oper2 = convertOper(command.at(2));

                if (command.at(0) == "mov")
                {
                    bitString = conversionTable["mov"] + oper1 + oper2;
                }
                else if (command.at(0) == "add")
                {
                    bitString = conversionTable["add"] + oper1 + oper2;
                }
                else if (command.at(0) == "cmp")
                {
                    bitString = conversionTable["cmp"] + oper1 + oper2;
                }
            }
        }
    }

    int remainingZeros;
    remainingZeros = (int)bitString.size() % 8;
    remainingZeros = 8 - remainingZeros;
    string trailingZeros(remainingZeros, '0');

    bitString = bitString + trailingZeros;
    return bitString;
}
string stylizedString(string fullBitString){
    string formatedBytes = "";

    int parts = fullBitString.size()/8;
    int i;
    for(i = 0; i < parts; i++){
        formatedBytes += "Bytes"
    }
/* FIXME:
   -all you need to do is just add a bytes formated with setw that reads
   in the currentbyte then you do a loop (might need to reverse the oder of the loopshere
   but in that loop you copy 8 chars then copy newline
   then you add 1 to currentbyt after newline is added)
   -for the last thing for the labels all you got to do is just make the labels currentbyte+1 or just current byte 
   (i dont think you need lastbyte variable)
   -lastly you just have to reacall the entire proccess function but this time
   without touching the labelTabel and using it to write in the values instead of x's */
    return formatedBytes;
}
int parseProccess(vector<string> fileContents)
{
    /*-------Here we do validation of double label-----*/
    /* holds only the label string elements. Will be used for double label check*/
    vector<string> tempLabelHolder;
    /* this holds the vector of strings minus all the lables */
    vector<string> lablessStringVector;
    int i;
    for (i = 0; i < (int)fileContents.size(); i++) // main loop of proccesing
    {
        string currentInputString = fileContents[i];
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
            string labelVal = numToBinary((currentByte));//cant do +1 since the first adress is always zero and 0 + 1 is not zero 
            labelTable[currentInputString] = labelVal;
        }
        else
        {
            lablessStringVector.push_back(currentInputString);
            //cout << '\n';
            //cout << "Machine rep:" << convertToMachine(currentInputString) << '\n'; //debugComment 
            currentInputString = convertToMachine(currentInputString);
            currentInputString = stylizeString(currentInputString);
        }

        cout << currentInputString <<  '\n'; //debugComment    
    }

    // int iter1;
    // int iter2;
    // cout << "Lables:" <<  '\n'; //debugComment
    // for(iter1 = 0; iter1 < (int)tempLabelHolder.size(); iter1++){
    //     cout << tempLabelHolder[iter1] << '\n'; //debugComment
    // }
    // cout << "NON-Labels:" <<  '\n'; //debugComment
    // for(iter2 = 0; iter2 < (int)lablessStringVector.size(); iter2++){
    //     cout << lablessStringVector[iter2] << '\n'; //debugComment
    // }
    /*-----Now that double label check we can do first pass*/
    /* for (every string){
        bitPostion = 0
    }
    ) */
    
    return 0;
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

    numToBinary(INT32_MIN); // this works since we will not be given invalid sizes of ints so we can just use that same thing and input ints even if they are unsigneds

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
