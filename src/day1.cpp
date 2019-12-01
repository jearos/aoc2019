#include "doctest.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

static int CalcFuel(int mass)
{
    return (mass/3)-2;
}

static int CalcFuel2(int mass)
{
    int fuel = 0;
    fuel = CalcFuel(mass);
    if(fuel < 0)
        return 0;
    else
    return fuel+(CalcFuel2(fuel));
}

static int getFuelFromString(string massStr)
{
    int mass = 0;
    stringstream ss(massStr);

    ss >> mass;

    return CalcFuel(mass);
}

static int getFuelFromString2(string massStr)
{
    int mass = 0;
    stringstream ss(massStr);

    ss >> mass;

    return CalcFuel2(mass);
}

static int getFuelFromFile(string filename)
{
    ifstream file;
    file.open(filename);
    string line;
    int sum = 0;
    while(getline(file, line))
    {
        sum += getFuelFromString(line);
    }
    return sum;
}

static int getFuelFromFile2(string filename)
{
    ifstream file;
    file.open(filename);
    string line;
    int sum = 0;
    while(getline(file, line))
    {
        sum += getFuelFromString2(line);
    }
    return sum;
}

TEST_CASE("test1")
{
    CHECK(getFuelFromString("12") == 2);
    CHECK(getFuelFromString("14") == 2);
    CHECK(getFuelFromString("1969") == 654);
    CHECK(getFuelFromString("100756") == 33583);
    CHECK(getFuelFromFile("../tests/day1.txt") == 3560353);
    CHECK(getFuelFromString2("12") == 2);
    CHECK(getFuelFromString2("1969") == 966);
    CHECK(getFuelFromString2("100756") == 50346);
    CHECK(getFuelFromFile2("../tests/day1.txt") == 5337642);
}
