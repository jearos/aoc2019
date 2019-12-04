#include "doctest.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

static bool checkAdjacent(int pw)
{
    int digit0 = pw%10;
    int digit1 = (pw/10)%10;
    int digit2 = (pw/100)%10;
    int digit3 = (pw/1000)%10;
    int digit4 = (pw/10000)%10;
    int digit5 = (pw/100000)%10;

    bool isDoubleDigit = false;

    if(digit1 == digit0)
    {
        isDoubleDigit = true;
    }
    else if(digit2 == digit1)
    {
        isDoubleDigit = true;
    }
    else if(digit3 == digit2)
    {
        isDoubleDigit = true;
    }
    else if(digit4 == digit3)
    {
        isDoubleDigit = true;
    }
    else
    if(digit5 == digit4)
    {
        isDoubleDigit = true;
    }

    if(isDoubleDigit)
    {
        if ((digit5 <= digit4) &&
            (digit4 <= digit3) &&
            (digit3 <= digit2) &&
            (digit2 <= digit1) &&
            (digit1 <= digit0))
        {
            return true;
        }
    }
    return false;
}

static bool checkAdjacent2(int pw)
{
    int digit0 = pw%10;
    int digit1 = (pw/10)%10;
    int digit2 = (pw/100)%10;
    int digit3 = (pw/1000)%10;
    int digit4 = (pw/10000)%10;
    int digit5 = (pw/100000)%10;

    bool isDoubleDigit = false;

    if((digit2 != digit1) && (digit1 == digit0))
    {
        isDoubleDigit = true;
    }
    else if((digit3 != digit2) && (digit2 == digit1) && (digit1 != digit0))
    {
        isDoubleDigit = true;
    }
    else if((digit4 != digit3) && (digit3 == digit2) && (digit2 != digit1))
    {
        isDoubleDigit = true;
    }
    else if((digit5 != digit4) && (digit4 == digit3) && (digit3 != digit2))
    {
        isDoubleDigit = true;
    }
    else
    if((digit5 == digit4) && (digit4 != digit3))
    {
        isDoubleDigit = true;
    }

    if(isDoubleDigit)
    {
        if ((digit5 <= digit4) &&
            (digit4 <= digit3) &&
            (digit3 <= digit2) &&
            (digit2 <= digit1) &&
            (digit1 <= digit0))
        {
            return true;
        }
    }
    return false;
}

static int checkNofPasswords(int start, int stop)
{
    int count = 0;
    for(int i=start; i<=stop; i++)
    {
        if(checkAdjacent(i))
        {
            count++;
        }
    }
    return count;
}

static int checkNofPasswords2(int start, int stop)
{
    int count = 0;
    for(int i=start; i<=stop; i++)
    {
        if(checkAdjacent2(i))
        {
            count++;
        }
    }
    return count;
}
TEST_CASE("test1")
{
    CHECK(checkAdjacent(111111) == true);
    CHECK(checkAdjacent(223450) == false);
    CHECK(checkAdjacent(123789) == false);
    CHECK(checkNofPasswords(206938,679128) == 1653);

    CHECK(checkAdjacent2(112233) == true);
    CHECK(checkAdjacent2(123444) == false);
    CHECK(checkAdjacent2(111122) == true);
    CHECK(checkNofPasswords2(206938,679128) == 1133);
}
