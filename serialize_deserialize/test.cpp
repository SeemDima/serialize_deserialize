#include <fstream>
#include <iostream>

#include "gtest/gtest.h"
#include "serialize.h"

TEST(serialization_test, basic_data_type)
{
    std::ofstream ofs("test.ser", std::ofstream::out | std::ofstream::binary);
    char hello[7] = "hello!";
    int boo[3] = { 1, 2, 3 };

    serialize(hello, ofs);
    serialize(boo, ofs);

    char hello2[7];
    int boo2[3];

    ofs.close();

    std::ifstream ifs("test.ser", std::ifstream::in | std::ofstream::binary);
    ifs >> std::noskipws;
    try {
        deserialize(hello2, ifs);
        deserialize(boo2, ifs);
    }
    catch (DeserializeException &exc)
    {
        std::cout << exc.what();
        FAIL();
    }

    if (!std::equal(hello, hello + 7, hello2))
        FAIL();

    if (!std::equal(boo, boo + 3, boo2))
        FAIL();
}

TEST(serialization_test, vector_data_type)
{
    std::ofstream ofs("test.ser", std::ofstream::out | std::ofstream::binary);

    std::vector<int> vecA = {1, 2, 3};
    serialize(vecA, ofs);

    ofs.close();

    std::ifstream ifs("test.ser", std::ifstream::in | std::ofstream::binary);
    ifs >> std::noskipws;

    std::vector<int> vecB;

    try {
        deserialize(vecB, ifs);
    }
    catch (DeserializeException &exc)
    {
        std::cout << exc.what();
        FAIL();
    }
    for (size_t i=0; i < vecB.size(); ++i)
    {
        if (vecB.at(i) != vecA.at(i))
        {
            FAIL();
        }
    }

}

TEST(serialization_test, map_data_type)
{
    std::ofstream ofs("test.ser", std::ofstream::out | std::ofstream::binary);
    std::map<char, int> mapA;
    mapA.insert(std::pair<char, int>(1, 3));
    mapA.insert(std::pair<char, int>(2, 5));
    mapA.insert(std::pair<char, int>(3, 7));

    serialize(mapA, ofs);

    ofs.close();

    std::ifstream ifs("test.ser", std::ifstream::in | std::ofstream::binary);
    ifs >> std::noskipws;

    std::map<char, int> mapB;


    try {
        deserialize(mapB, ifs);
    }
    catch (DeserializeException &exc)
    {
        std::cout << exc.what();
        FAIL();
    }

    for (auto& it: mapB)
    {
        if (it.second != mapA.at(it.first))
        {
            FAIL();
        }
    }

}

TEST(serialization_test, string_data_type)
{
    std::ofstream ofs("test.ser", std::ofstream::out | std::ofstream::binary);

    std::string strA1("string example");
    std::string strA2("string example number two, and it is very big string. Really biiiiiiiiiiig");
    std::string strA3("");

    serialize(strA1, ofs);
    serialize(strA2, ofs);
    serialize(strA3, ofs);

    ofs.close();

    std::ifstream ifs("test.ser", std::ifstream::in | std::ofstream::binary);
    ifs >> std::noskipws;

    std::string strB1;
    std::string strB2;
    std::string strB3;

    try {
        deserialize(strB1, ifs);
        deserialize(strB2, ifs);
        deserialize(strB3, ifs);
    }
    catch (DeserializeException &exc)
    {
        std::cout << exc.what();
        FAIL();
    }

    if (strA1 != strB1 || strA2 != strB2 || strA3 != strB3)
        FAIL();

}
