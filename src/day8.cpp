#include "doctest.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

static int decodeString(string pixel, unsigned long width, unsigned long height)
{
    unsigned long layers = pixel.size() / width / height;

    vector<vector<int>> image;
    vector<int> nofZeros;
    vector<int> nofOnes;
    vector<int> nofTwos;
    char *p = &pixel[0];

    for(unsigned long l=0;l<layers;l++)
    {
        vector<int> layer;
        for(unsigned long j=0;j<width*height;j++)
        {
                layer.push_back(*p++-'0');
        }
        image.push_back(layer);
        nofZeros.push_back(0);
        nofOnes.push_back(0);
        nofTwos.push_back(0);
    }

    for(unsigned long l=0;l<layers;l++)
    {
        for(unsigned long j=0;j<width*height;j++)
        {
            if(image[l][j] == 0)
            {
                nofZeros[l]++;
            }
            if(image[l][j] == 1)
            {
                nofOnes[l]++;
            }
            if(image[l][j] == 2)
            {
                nofTwos[l]++;
            }
        }
    }

    vector<int> decodedImage;
    for(unsigned long j=0;j<width*height;j++)
    {
        int q = 2;
        unsigned long int l = layers;
        while(l--)
        {
            if(image[l][j] == 0)
            {
                q = 0;
            }
            else if(image[l][j] == 1)
            {
                q = 1;
            }
        }
        decodedImage.push_back(q);
    }

   for(unsigned long y=0;y<height;y++)
    {
        for(unsigned long x=0;x<width;x++)
        {
            if(decodedImage[x+y*width] == 1)
            {
                cout << "*";
            }
            else
            {
                cout << " ";
            }
        }
        cout << endl;
    }

    unsigned long minZerosLayerIndex =static_cast<unsigned long>(std::min_element(nofZeros.begin(),nofZeros.end()) - nofZeros.begin());

    return(nofOnes[minZerosLayerIndex] * nofTwos[minZerosLayerIndex]);

}


static int decodeFile(string filename, unsigned long width, unsigned long height)
{
    ifstream file;
    file.open(filename);
    string line;

    while(getline(file, line))
    {
        return decodeString(line, width, height);
    }
    return 0;
}


TEST_CASE("test1")
{
    CHECK(decodeString("123456789012", 3, 2) == 1);
    CHECK(decodeFile("../tests/day8.txt", 25, 6) == 1560);
}
