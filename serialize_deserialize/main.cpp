#include <fstream>
#include <iostream>
#include "serialize.h"
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    ofstream ofs("test.ser", ofstream::out | ofstream::binary);

    char hello[7] = "hello!";
    int boo[3] = { 1, 2, 3 };
    vector<int> vecA = {1, 2, 3};

    serialize(hello, ofs);
    serialize(boo, ofs);
    serialize(vecA, ofs);


    char hello2[7];
    int boo2[3];
    vector<int> vecB;

    ofs.close();

    ifstream ifs("test.ser", ifstream::in | ofstream::binary);
    ifs >> noskipws;
    try {
        deserialize(hello2, ifs);
        deserialize(boo2, ifs);
        deserialize(vecB, ifs);
    }
    catch (DeserializeException &exc)
    {
        cout << exc.what();
        return -1;
    }

    if (equal(hello, hello + 7, hello2))
        cout << "Hello OK!" << endl;

    if (equal(boo, boo + 3, boo2))
        cout << "Boo OK!" << endl;

    for (size_t i=0; i < vecB.size(); ++i)
    {
        if (vecB.at(i) != vecA.at(i))
        {
            cout << "VecB not OK!" << endl;
            return -1;
        }
    }
    cout << "vecB OK!" << endl;

    return 0;
}
