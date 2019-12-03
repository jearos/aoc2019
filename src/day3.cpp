#include "doctest.h"

#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

static void runWire(string str, std::map<string, int>& wireMap, int wire)
{
    stringstream ss(str);
    string opcode;
    int value;
    int x = 0;
    int y = 0;
    while(getline(ss,opcode,','))
    {
        int x_step = 0;
        int y_step = 0;
        value = stoi(&opcode[1]);
        if(opcode[0] == 'U')
        {
            y_step = 1;
        }
        else if(opcode[0] == 'D')
        {
            y_step = -1;
        }
        else if(opcode[0] == 'R')
        {
            x_step = 1;
        }
        else if(opcode[0] == 'L')
        {
            x_step = -1;
        }
        else
        {
            assert(0);
        }
        for(int i=0;i<value;i++)
        {
            stringstream coord;
            x+=x_step;
            y+=y_step;
            coord << x << " " << y;
            wireMap[coord.str()] |= wire;
//            cout<<coord.str()<<endl;
        }
    }
}

static int checkWire(std::map<string, int>& wireMap)
{
    int minDist = INT_MAX;
    for( auto p : wireMap )
    {
        if(p.second == 3)
        {
            int x = 0;
            int y = 0;
            stringstream coord(p.first);
            coord >> x;
            coord >> y;
            {
                int dist = abs(x) + abs(y);
                if(dist < minDist)
                {
                    minDist = dist;
                }
            }
        }
    }
    return minDist;
}

static void runWiresFromFile(string filename, std::map<string, int>& wireMap)
{
    ifstream file;
    file.open(filename);
    string line;
    getline(file, line);
    runWire(line, wireMap, 2);
    getline(file, line);
    runWire(line, wireMap, 1);
}

static void runWire2(string str, std::map<string, int>& wireMap)
{
    stringstream ss(str);
    string opcode;
    int value;
    int x = 0;
    int y = 0;
    int steps = 0;
    while(getline(ss,opcode,','))
    {
        int x_step = 0;
        int y_step = 0;
        value = stoi(&opcode[1]);
        if(opcode[0] == 'U')
        {
            y_step = 1;
        }
        else if(opcode[0] == 'D')
        {
            y_step = -1;
        }
        else if(opcode[0] == 'R')
        {
            x_step = 1;
        }
        else if(opcode[0] == 'L')
        {
            x_step = -1;
        }
        else
        {
            assert(0);
        }
        for(int i=0;i<value;i++)
        {
            stringstream coord;
            x+=x_step;
            y+=y_step;
            steps++;
            coord << x << " " << y;

            if(wireMap[coord.str()] == 0 )
            {
                wireMap[coord.str()] = steps;
//                cout<<coord.str()<<endl;
            }
        }
    }
}

static int checkWire2(std::map<string, int>& wireMap, std::map<string, int>& wireMap2)
{
    int minDist = INT_MAX;
    for( auto p : wireMap )
    {
        if(wireMap2[p.first] != 0)
        {
                int dist = p.second + wireMap2[p.first];
                if(dist != 0)
                    if(dist < minDist)
                        minDist = dist;
        }
    }
    return minDist;
}

static void runWiresFromFile2(string filename, std::map<string, int>& wireMap, std::map<string, int>& wireMap2)
{
    ifstream file;
    file.open(filename);
    string line;
    getline(file, line);
    runWire2(line, wireMap);
    getline(file, line);
    runWire2(line, wireMap2);
}


TEST_CASE("test")
{
    std::map<string, int> wireMap;
    wireMap.clear();
    runWire("R8,U5,L5,D3", wireMap, 1);
    runWire("U7,R6,D4,L4", wireMap, 2);
    CHECK(checkWire(wireMap)==6);

    wireMap.clear();
    runWire("R75,D30,R83,U83,L12,D49,R71,U7,L72", wireMap, 1);
    runWire("U62,R66,U55,R34,D71,R55,D58,R83", wireMap, 2);
    CHECK(checkWire(wireMap)==159);

    wireMap.clear();
    runWire("R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51", wireMap, 1);
    runWire("U98,R91,D20,R16,D67,R40,U7,R15,U6,R7", wireMap, 2);
    CHECK(checkWire(wireMap)==135);

    wireMap.clear();
    runWiresFromFile("../tests/day3.txt", wireMap);
    CHECK(checkWire(wireMap)==258);

    wireMap.clear();
    std::map<string, int> wireMap2;
    runWire2("R8,U5,L5,D3", wireMap);
    runWire2("U7,R6,D4,L4", wireMap2);
    CHECK(checkWire2(wireMap, wireMap2)==30);

    wireMap.clear();
    wireMap2.clear();
    runWire2("R75,D30,R83,U83,L12,D49,R71,U7,L72", wireMap);
    runWire2("U62,R66,U55,R34,D71,R55,D58,R83", wireMap2);
    CHECK(checkWire2(wireMap, wireMap2)==610);

    wireMap.clear();
    wireMap2.clear();
    runWire2("R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51", wireMap);
    runWire2("U98,R91,D20,R16,D67,R40,U7,R15,U6,R7", wireMap2);
    CHECK(checkWire2(wireMap, wireMap2)==410);
    wireMap.clear();
    wireMap2.clear();
    runWiresFromFile2("../tests/day3.txt", wireMap, wireMap2);
    CHECK(checkWire2(wireMap, wireMap2)==12304);
}
