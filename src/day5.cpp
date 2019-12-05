#include "doctest.h"

#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

static int runProgram(string str, const int input)
{
    vector<int> prg;
    stringstream ss(str);
    string instrStr;
    int value;

    int output = 0;

    while(getline(ss,instrStr,','))
    {
        value = atoi(instrStr.c_str());
        prg.push_back(value);
    }
    unsigned long pc = 0;
    int i = 0;
    while(i++<100000)
    {
        int instr = prg[pc];
        int opcode = instr%100;
        int imm1 = (instr/100)%10;
        int imm2 = (instr/1000)%10;
        int imm3 = (instr/1000)%10;

//        cout << pc << " " << instr << " " << opcode << endl;

        if(opcode == 99)
        {
            return output;
        }
        else if(opcode == 1)
        {
            unsigned long param1 = imm1 ? pc+1:static_cast<unsigned long>(prg[pc+1]);
            unsigned long param2 = imm2 ? pc+2:static_cast<unsigned long>(prg[pc+2]);
            unsigned long param3 = static_cast<unsigned long>(prg[pc+3]);
            prg[param3] = prg[param1] + prg[param2];
            pc += 4;
        }
        else if(opcode == 2)
        {
            unsigned long param1 = imm1 ? pc+1:static_cast<unsigned long>(prg[pc+1]);
            unsigned long param2 = imm2 ? pc+2:static_cast<unsigned long>(prg[pc+2]);
            unsigned long param3 = static_cast<unsigned long>(prg[pc+3]);
            prg[param3] = prg[param1] * prg[param2];
            pc += 4;
        }
        else if(opcode == 3)
        {
            unsigned long param1 = imm3 ? pc+1:static_cast<unsigned long>(prg[pc+1]);
            prg[param1] = input;
            pc += 2;
        }
        else if(opcode == 4)
        {
            unsigned long param1 = imm1 ? pc+1:static_cast<unsigned long>(prg[pc+1]);
            output = prg[param1];
            pc += 2;
        }
        else if(opcode == 5)
        {
            unsigned long param1 = imm1 ? pc+1:static_cast<unsigned long>(prg[pc+1]);
            unsigned long param2 = imm2 ? pc+2:static_cast<unsigned long>(prg[pc+2]);
            if(prg[param1] != 0)
                pc = static_cast<unsigned long>(prg[param2]);
            else
            {
                pc += 3;
            }
        }
        else if(opcode == 6)
        {
            unsigned long param1 = imm1 ? pc+1:static_cast<unsigned long>(prg[pc+1]);
            unsigned long param2 = imm2 ? pc+2:static_cast<unsigned long>(prg[pc+2]);
            if(prg[param1] == 0)
                pc = static_cast<unsigned long>(prg[param2]);
            else
            {
                pc += 3;
            }
        }
        else if(opcode == 7)
        {
            unsigned long param1 = imm1 ? pc+1:static_cast<unsigned long>(prg[pc+1]);
            unsigned long param2 = imm2 ? pc+2:static_cast<unsigned long>(prg[pc+2]);
            unsigned long param3 = static_cast<unsigned long>(prg[pc+3]);
            if(prg[param1] < prg[param2])
                prg[param3] = 1;
            else
                prg[param3] = 0;
            pc += 4;
        }
        else if(opcode == 8)
        {
            unsigned long param1 = imm1 ? pc+1:static_cast<unsigned long>(prg[pc+1]);
            unsigned long param2 = imm2 ? pc+2:static_cast<unsigned long>(prg[pc+2]);
            unsigned long param3 = static_cast<unsigned long>(prg[pc+3]);
            if(prg[param1] == prg[param2])
                prg[param3] = 1;
            else
                prg[param3] = 0;
            pc += 4;
        }
    }
    return 0;
}

static int runFile(string filename, const int input)
{
    ifstream file;
    file.open(filename);
    string line;
    while(getline(file, line))
    {
        return(runProgram(line, input));
    }
    return 0;
}

TEST_CASE("test")
{
    CHECK(runFile("../tests/day5.txt", 1) == 16225258);
    CHECK(runFile("../tests/day5.txt", 5) == 2808771);
}
