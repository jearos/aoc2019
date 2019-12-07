#include "doctest.h"

#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

static void getNumberOfOrbits(std::map<string, string>& orbits, string planet, int &sum)
{
    if(orbits[planet] != "")
    {
//        cout << orbits[planet];
        getNumberOfOrbits(orbits, orbits[planet], sum);
        sum++;
    }
}

static int getSumFromFile(std::string filename)
{
    ifstream file;
    file.open(filename);
    string parent;
    std::map<string, string> orbits;

    int sum = 0;
    while(getline(file, parent,')'))
    {
        string child;
        getline(file, child);
//        cout << parent << child << endl;
        orbits[child] = parent;
    }

    for( auto orbit : orbits )
    {
        getNumberOfOrbits(orbits, orbit.first, sum);
    }
    return sum;
}

static bool getNumberOfOrbitFromTo(std::map<string, string>& orbits, string planet, int &sum, string to)
{
    if(orbits[planet] == "")
    {
        return false;
    }
    else if(orbits[planet] != to)
    {
//        cout << orbits[planet];
        sum++;
        return getNumberOfOrbitFromTo(orbits, orbits[planet], sum, to);
    }
    return true;
}

static int getOrbitsFromFile(std::string filename)
{
    ifstream file;
    file.open(filename);
    string parent;
    std::map<string, string> orbits;

    int sum = 0;
    while(getline(file, parent,')'))
    {
        string child;
        getline(file, child);
//        cout << parent << child << endl;
        orbits[child] = parent;
    }
    sum = 0;

    int minSum = INT_MAX;

    for( auto orbit : orbits )
    {
        sum = 0;
        if(getNumberOfOrbitFromTo(orbits, "YOU", sum, orbit.first) &&
           getNumberOfOrbitFromTo(orbits, "SAN", sum, orbit.first) )
        {
            if(sum < minSum)
            {
                minSum = sum;
            }
        }
    }

    return minSum;
}

TEST_CASE("test")
{
    CHECK(getSumFromFile("../tests/day6_ut.txt")==42);
    CHECK(getSumFromFile("../tests/day6.txt")==621125);
    CHECK(getOrbitsFromFile("../tests/day6_ut2.txt")==4);
    CHECK(getOrbitsFromFile("../tests/day6.txt")==550);
}
