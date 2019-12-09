#include "doctest.h"

#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

#if !defined(__SIZEOF_INT128__)
    #error No int128
#else
typedef __int128 mem_t;
#endif

static mem_t& getParam(map<mem_t, mem_t>& prg, mem_t pc, mem_t mode, mem_t relative_base)
{
    if(mode == 2)
    {
        return prg[relative_base+prg[pc]];
    }
    else if(mode == 1)
    {
        return prg[pc];
    }
    else
    {
        return prg[prg[pc]];
    }
}

static void runProgram(string str, vector<mem_t>& input, vector<mem_t>& output)
{
//    vector<mem_t> prg;
    map<mem_t, mem_t> prg;
    stringstream ss(str);
    string instrStr;
    mem_t value;
    unsigned long input_index = 0;
    unsigned long step = 0;

    while(getline(ss,instrStr,','))
    {
        value = atoll(instrStr.c_str());
        prg[step++] = value;
    }
    mem_t pc = 0;
    mem_t rb = 0;
    //int i = 0;

    while(true)
    {
        mem_t instr = prg[pc];
        mem_t opcode = instr%100;
        mem_t imm1 = (instr/100)%10;
        mem_t imm2 = (instr/1000)%10;
        mem_t imm3 = (instr/10000)%10;

//        cout << pc << " " << instr << " " << opcode << endl;

        if(opcode == 99)
        {
            return;
        }
        else if(opcode == 1)
        {
            if(imm3 != 1)
                getParam(prg, pc+3, imm3, rb) = getParam(prg, pc+1, imm1, rb) + getParam(prg, pc+2, imm2, rb);
            pc += 4;
        }
        else if(opcode == 2)
        {
            if(imm3 != 1)
                getParam(prg, pc+3, imm3, rb) = getParam(prg, pc+1, imm1, rb) * getParam(prg, pc+2, imm2, rb);
            pc += 4;
        }
        else if(opcode == 3)
        {
            if(imm1 != 1)
                getParam(prg, pc+1, imm1, rb) = input[input_index++];
            pc += 2;
        }
        else if(opcode == 4)
        {
            output.push_back(getParam(prg, pc+1, imm1, rb));
            pc += 2;
        }
        else if(opcode == 5)
        {
            if(getParam(prg, pc+1, imm1, rb) != 0)
                pc = (getParam(prg, pc+2, imm2, rb));
            else
            {
                pc += 3;
            }
        }
        else if(opcode == 6)
        {
            if(getParam(prg, pc+1, imm1, rb) == 0)
                pc = getParam(prg, pc+2, imm2, rb);
            else
            {
                pc += 3;
            }
        }
        else if(opcode == 7)
        {
            if(imm3 != 1)
            {
                if(getParam(prg, pc+1, imm1, rb) < getParam(prg, pc+2, imm2, rb))
                        getParam(prg, pc+3, imm3, rb) = 1;
                else
                        getParam(prg, pc+3, imm3, rb) = 0;
            }
            pc += 4;
        }
        else if(opcode == 8)
        {
            if(imm3 != 1)
            {
                if(getParam(prg, pc+1, imm1, rb) == getParam(prg, pc+2, imm2, rb))
                    getParam(prg, pc+3, imm3, rb) = 1;
                else
                    getParam(prg, pc+3, imm3, rb) = 0;
            }
            pc += 4;
        }
        else if(opcode == 9)
        {
            rb += getParam(prg, pc+1, imm1, rb);
            pc += 2;
        }
    }
}

static long runFile(string filename, vector<mem_t>& input, vector<mem_t>& output)
{
    ifstream file;
    file.open(filename);
    string line;
    while(getline(file, line))
    {
        runProgram(line, input, output);
    }
    return 0;
}


TEST_CASE("test")
{
    vector<mem_t> input;
    vector<mem_t> output;

    input.clear();
    output.clear();
    mem_t test_ary[] = {109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99};
    runProgram("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99", input, output);
    for(unsigned long i=0;i<16;i++)
        CHECK( static_cast<long>(output[i]) == static_cast<long>(test_ary[i]));

    input.clear();
    output.clear();
    runProgram("1102,34915192,34915192,7,4,7,99,0", input, output);
    CHECK( static_cast<long>(output[0]) == 1219070632396864);
    input.clear();
    output.clear();
    runProgram("104,1125899906842624,99", input, output);
    CHECK( static_cast<long>(output[0]) == 1125899906842624);
    input.clear();
    output.clear();
    input.push_back(1);
    runFile("../tests/day9.txt", input, output);
    CHECK( output.size() == 1);
/*
    for(unsigned long i=0;i<output.size();i++)
    {
        cout << "Failure:" << static_cast<long>(output[i]) << endl;
    }
*/
    CHECK(static_cast<long>(output[0]) == 3380552333);

    input.clear();
    output.clear();
    input.push_back(2);
    runFile("../tests/day9.txt", input, output);
    CHECK( output.size() == 1);
    CHECK(static_cast<long>(output[0]) == 78831);

}
