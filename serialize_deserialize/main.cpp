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
    map<char, int> mapA;
        mapA.insert(pair<char, int>(1, 3));
        mapA.insert(pair<char, int>(2, 5));
        mapA.insert(pair<char, int>(3, 7));

    serialize(hello, ofs);
    serialize(boo, ofs);
    serialize(vecA, ofs);
    serialize(mapA, ofs);



    char hello2[7];
    int boo2[3];
    vector<int> vecB;
    map<char, int> mapB;

    ofs.close();

    ifstream ifs("test.ser", ifstream::in | ofstream::binary);
    ifs >> noskipws;
    try {
        deserialize(hello2, ifs);
        deserialize(boo2, ifs);
        deserialize(vecB, ifs);
        deserialize(mapB, ifs);
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

    for (auto& it: mapB)
    {
        if (it.second != mapA.at(it.first))
        {
            cout << "MapB not OK!" << endl;
            return -1;
        }
    }
    cout << "mapB OK!" << endl;

    return 0;
}
