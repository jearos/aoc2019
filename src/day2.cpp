#include "doctest.h"

#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

static int runProgram(string str, int noun, int verb)
{
    vector<int> prg;
    stringstream ss(str);
    string opcode;
    int value;

    while(getline(ss,opcode,','))
    {
        value = atoi(opcode.c_str());
        prg.push_back(value);
    }
    prg[1] = noun;
    prg[2] = verb;
    unsigned long pc = 0;
    while(1)
    {
        if(prg[pc] == 99)
        {
            return prg[0];
        }
        else if(prg[pc] == 1)
        {
            prg[static_cast<unsigned long>(prg[pc+3])] = prg[static_cast<unsigned long>(prg[pc+1])] + prg[static_cast<unsigned long>(prg[pc+2])];
        }
        else if(prg[pc] == 2)
        {
            prg[static_cast<unsigned long>(prg[pc+3])] = prg[static_cast<unsigned long>(prg[pc+1])] * prg[static_cast<unsigned long>(prg[pc+2])];
        }
        pc += 4;
    }
}

static int runFile(string filename)
{
    ifstream file;
    file.open(filename);
    string line;
    while(getline(file, line))
    {
        return(runProgram(line, 12, 2));
    }
    return 0;
}

static int runFile2(string filename)
{
    ifstream file;
    file.open(filename);
    string line;
    int noun = 0;
    int verb = 0;
    getline(file, line);

    for(noun=0;noun<=99;noun++)
    {
        for(verb=0;verb<=99;verb++)
        {
            if( runProgram(line, noun, verb) == 19690720)
            {
                return 100 * noun + verb;
            }
        }
    }
    return 0;
}


TEST_CASE("test")
{
    CHECK(runProgram("1,0,0,0,99",0,0) == 2);
    CHECK(runProgram("1,1,1,4,99,5,6,0,99", 1, 1) == 30);
    CHECK(runFile("../tests/day2.txt") == 5290681);
    CHECK(runFile2("../tests/day2.txt") == 5741);
}
