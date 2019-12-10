#include "doctest.h"

#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_set>
#include <complex>
#include <numeric>

using namespace std;

class Detect
{
    public:
        void decodeFile(string filename);
        bool isAsteroid(unsigned long x, unsigned long y);
        string calcSlope(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2);
        unsigned long nofAsteroids(unsigned long x, unsigned long y);
        unsigned long getMaxSurroundingAsteroids(unsigned long& x0, unsigned long& y0);
    private:
        map<string, char> asteroids_map;
        unsigned long x_size;
        unsigned long y_size;
};


bool Detect::isAsteroid(unsigned long x, unsigned long y)
{
    stringstream coord;
    coord << x << " " << y;
    if(asteroids_map[coord.str()] == '#')
    {
        return true;
    }
    return false;
}


string Detect::calcSlope(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2)
{
    int dx = static_cast<int>(x2)-static_cast<int>(x1);
    int dy = static_cast<int>(y2)-static_cast<int>(y1);

    int l = gcd(dx,dy);

    if(l!=0)
    {
    dx = dx / l;
    dy = dy / l;
    }
    stringstream coord;
    coord << dx << " " << dy;
    return coord.str();

}

unsigned long Detect::nofAsteroids(unsigned long x0, unsigned long y0)
{
    unordered_set<string> asteroids_set;
    for(unsigned long y=0;y<y_size;y++)
    {
        for(unsigned long x=0;x<x_size;x++)
        {
            if(isAsteroid(x,y))
            {
                asteroids_set.insert(this->calcSlope(x0,y0,x,y));
//                cout << this->calcSlope(x0,y0,x,y);
            }
        }
    }
    return asteroids_set.size()-1;
}

unsigned long Detect::getMaxSurroundingAsteroids(unsigned long& x0, unsigned long& y0)
{
    unsigned long max_asteroids = 0;
    for(unsigned long y=0;y<y_size;y++)
    {
        for(unsigned long x=0;x<x_size;x++)
        {
            if(isAsteroid(x,y))
            {
                unsigned long nof_asteroids = nofAsteroids(x,y);
                if(nof_asteroids > max_asteroids)
                {
                    max_asteroids = nof_asteroids;
                    x0 = x;
                    y0 = y;
                }
            }
        }
    }
    return max_asteroids;
}

void Detect::decodeFile(string filename)
{
    ifstream file;
    file.open(filename);
    string line;
    unsigned long y=0;

    asteroids_map.clear();
    while(getline(file, line))
    {
        for(unsigned long x=0;x<line.size();x++)
        {
            stringstream coord;
            coord << x << " " << y;
            asteroids_map[coord.str()] = line[x];
        }
        y++;
    }
    x_size = line.size();
    y_size = y;
}


TEST_CASE("test")
{
    Detect detect;
    unsigned long x0;
    unsigned long y0;

    detect.decodeFile("../tests/day10_ut1.txt");
    CHECK(detect.isAsteroid(0,0) == false);
    CHECK(detect.isAsteroid(1,0) == true);
    CHECK(detect.nofAsteroids(1,0) == 7);
    CHECK(detect.nofAsteroids(3,4) == 8);
    CHECK(detect.getMaxSurroundingAsteroids(x0,y0) == 8);

    detect.decodeFile("../tests/day10_ut2.txt");
    CHECK(detect.nofAsteroids(5,8) == 33);
    CHECK(detect.getMaxSurroundingAsteroids(x0,y0) == 33);
    CHECK(x0 == 5);
    CHECK(y0 == 8);
    detect.decodeFile("../tests/day10.txt");
    CHECK(detect.getMaxSurroundingAsteroids(x0,y0) == 278);
    CHECK(x0 == 23);
    CHECK(y0 == 19);
}
