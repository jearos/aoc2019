#include "doctest.h"

#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

static int& getParam(vector<int>& prg, unsigned long pc, int mode)
{
    unsigned long val = mode ? pc:static_cast<unsigned long>(prg[pc]);
    return prg[val];
}
static int runProgram(string str, const int input[])
{
    vector<int> prg;
    stringstream ss(str);
    string instrStr;
    int value;
    int input_index = 0;
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
            getParam(prg, pc+3, 0) = getParam(prg, pc+1, imm1) + getParam(prg, pc+2, imm2);
            pc += 4;
        }
        else if(opcode == 2)
        {
            getParam(prg, pc+3, 0) = getParam(prg, pc+1, imm1) * getParam(prg, pc+2, imm2);
            pc += 4;
        }
        else if(opcode == 3)
        {
            getParam(prg, pc+1, imm3) = input[input_index++];
            pc += 2;
        }
        else if(opcode == 4)
        {
            output = getParam(prg, pc+1, imm1);
            pc += 2;
        }
        else if(opcode == 5)
        {
            if(getParam(prg, pc+1, imm1) != 0)
                pc = static_cast<unsigned long>(getParam(prg, pc+2, imm2));
            else
            {
                pc += 3;
            }
        }
        else if(opcode == 6)
        {
            if(getParam(prg, pc+1, imm1) == 0)
                pc = static_cast<unsigned long>(getParam(prg, pc+2, imm2));
            else
            {
                pc += 3;
            }
        }
        else if(opcode == 7)
        {
            if(getParam(prg, pc+1, imm1) < getParam(prg, pc+2, imm2))
                getParam(prg, pc+3, 0) = 1;
            else
                getParam(prg, pc+3, 0) = 0;
            pc += 4;
        }
        else if(opcode == 8)
        {
            if(getParam(prg, pc+1, imm1) == getParam(prg, pc+2, imm2))
                getParam(prg, pc+3, 0) = 1;
            else
                getParam(prg, pc+3, 0) = 0;
            pc += 4;
        }
    }
    return 0;
}

static int runFile(string filename, int input)
{
    int input_array[2];
    input_array[1] = 0;
    for(int i=0;i<5;i++)
    {
        int phase = input % 10;
        input = input / 10;
        input_array[0] = phase;

 //       cout << input << endl;
//        cout << phase << endl;

        ifstream file;
        file.open(filename);
        string line;
        while(getline(file, line))
        {
            input_array[1] = runProgram(line, input_array);
        }
    }
    return input_array[1];
}

static bool execute(std::vector<int>& prg, unsigned long& pc, vector<int> input, int& output)
{
    int i = 0;
    while(i++<100000)
    {
        int instr = prg[pc];
        int opcode = instr%100;
        int imm1 = (instr/100)%10;
        int imm2 = (instr/1000)%10;
        int imm3 = (instr/1000)%10;

        if(opcode == 99)
        {
            return true;
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
            if(!input.empty())
            {
                prg[param1] = input.back();
                input.pop_back();
                pc += 2;
            }
            else
            {
                return false;
            }
        }
        else if(opcode == 4)
        {
            unsigned long param1 = imm1 ? pc+1:static_cast<unsigned long>(prg[pc+1]);
            output = prg[param1];
            pc += 2;
            return false;
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
    return false;
}
static int runProgram2(string str, int input, int &output)
{
    vector<int> input_array;

    input_array.push_back(0);

    vector<int> prg_[5];
    stringstream ss(str);
    string instrStr;
    int value;

    while(getline(ss,instrStr,','))
    {
        value = atoi(instrStr.c_str());
        prg_[0].push_back(value);
        prg_[1].push_back(value);
        prg_[2].push_back(value);
        prg_[3].push_back(value);
        prg_[4].push_back(value);
    }
    unsigned long pc_[5] = {0};
    int i = 0;
    bool firstTime = true;
    input_array[1] = 0;
    while(1)
    {
        int phase = input;
        int stopCount = 0;
        for(i=0;i<5;i++)
        {
            if(firstTime)
            {
                int digit = phase % 10;
                phase = phase / 10;
                input_array.push_back(digit);
            }
            if(execute(prg_[i], pc_[i], input_array, output))
            {
                stopCount++;
                //cout << stopCount;
                if(stopCount == 5)
                {
                    return output;
                }
            }
            if(firstTime)
            {
                input_array.push_back(output);
            }
            else
            {
                input_array.push_back(output);
            }
        }
        if(firstTime) firstTime = false;

    }
}

static int runFile2(string filename, int input)
{
    int output = 0;
    ifstream file;
    file.open(filename);
    string line;
    while(getline(file, line))
    {
        return(runProgram2(line, input, output));
    }
    return 0;
}

static int testCombinations(string filename)
{
    int maxOutput = INT_MIN;
    for(int i=0;i<=4;i++)
    {
        for(int j=0;j<=4;j++)
        {
            for(int k=0;k<=4;k++)
            {
                for(int l=0;l<=4;l++)
                {
                    for(int m=0;m<=4;m++)
                    {
                        if( (i!=j&&i!=k&&i!=l&&i!=m) &&
                            (j!=i&&j!=k&&j!=l&&j!=m) &&
                            (k!=i&&k!=j&&k!=l&&k!=m) &&
                            (l!=i&&l!=j&&l!=k&&l!=m) &&
                            (m!=i&&m!=j&&m!=k&&m!=l) )

                        {
                            int input = 10000*i+1000*j+100*k+10*l+m;
                            int output = runFile2(filename, input);
                            if(output > maxOutput)
                            {
//                                cout << input << endl;
                                maxOutput = output;
                            }
                        }
                    }
                }
            }
        }
    }
    return maxOutput;
}

static int testCombinations2(string filename)
{
    int maxOutput = INT_MIN;
    for(int i=5;i<=9;i++)
    {
        for(int j=5;j<=9;j++)
        {
            for(int k=5;k<=9;k++)
            {
                for(int l=5;l<=9;l++)
                {
                    for(int m=5;m<=9;m++)
                    {
                        if( (i!=j&&i!=k&&i!=l&&i!=m) &&
                            (j!=i&&j!=k&&j!=l&&j!=m) &&
                            (k!=i&&k!=j&&k!=l&&k!=m) &&
                            (l!=i&&l!=j&&l!=k&&l!=m) &&
                            (m!=i&&m!=j&&m!=k&&m!=l) )

                        {
                            int input = 10000*i+1000*j+100*k+10*l+m;
                            int output = runFile2(filename, input);
                            if(output > maxOutput)
                            {
//                                cout << input << endl;
                                maxOutput = output;
                            }
                        }
                    }
                }
            }
        }
    }
    return maxOutput;
}
TEST_CASE("test")
{
    CHECK(runFile("../tests/day7_ut1.txt", 1234) == 43210);
    CHECK(runFile("../tests/day7_ut2.txt", 43210) == 54321);
    CHECK(runFile("../tests/day7_ut3.txt", 23401) == 65210);
    CHECK(testCombinations("../tests/day7.txt") == 422858);
}
TEST_CASE("test2")
{
    CHECK(runFile2("../tests/day7_ut4.txt", 56789) == 139629729);
    CHECK(runFile2("../tests/day7_ut5.txt", 65879) == 18216);
    CHECK(testCombinations2("../tests/day7.txt") == 14897241);
}
