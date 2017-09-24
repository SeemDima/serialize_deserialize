#ifndef SERIALIZE_H
#define SERIALIZE_H
#pragma once

//#include "deserialize_exception.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <map>

#define CHECK_BYTE 1

enum DATA_TYPE {
    BASIC,
    VECTOR,
    MAP,
    CLASS
};
//template <class C>

struct serializer {
    template <typename T>
    static void apply(const T &obj, std::ostream &os) {
        const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&obj); //ptr - is pointer to 1 byte of info

        std::ostream_iterator<uint8_t> oi(os);
        if (CHECK_BYTE)
            os << uint8_t(BASIC);

        std::copy(ptr, ptr + sizeof(T), oi); //побайтовое копирование
    }
    template <typename T>
    static void apply(const std::vector<T> &obj, std::ostream &os) {
        if (CHECK_BYTE)
            os << uint8_t(DATA_TYPE::VECTOR);
        for (auto it:obj)
        {
            apply(it, os);
        }
    }
    template <typename T, typename U>
    static void apply(const std::map<T,U> &obj, std::ostream &os) {
        if (CHECK_BYTE)
            os << uint8_t(DATA_TYPE::MAP);
        for (std::map<T, U>::iterator it = obj.begin(); it < obj.end(); ++it)
        {
            apply(it->first, os);
            apply(it->second, os);
        }
    }

};

///////////////////////////////////////////////////////////////////////


struct deserializer {
    template <typename T>
    static void apply(T &val, std::istream &is) {

        if (CHECK_BYTE)
        {
            uint8_t check;
            is >> check;
            if (check != DATA_TYPE::BASIC)
                throw BasicDeserializationException();
        }

        uint8_t *ptr = reinterpret_cast<uint8_t*>(&val);

        std::istream_iterator<uint8_t> ii(is);

        std::copy_n(ii, sizeof(T), ptr);
    }

    template <typename T>
    static void apply(std::vector<T> &val, std::istream &is) {
        if (CHECK_BYTE)
        {
            uint8_t check;
            is >> check;
            if (check != DATA_TYPE::VECTOR)
                throw VectorDeserializationException();
        }
        while (is.peek() != std::ifstream::traits_type::eof())
        {
            T temp_value;
            apply(temp_value, is);
            val.push_back(temp_value);
        }
    }
    template <typename T, typename U>
    static void apply(std::map<T, U> &val, std::istream &is) {
        if (CHECK_BYTE)
        {
            uint8_t check;
            is >> check;
            if (check != DATA_TYPE::MAP)
                throw MapDeserializationException();
        }
        while (is.peek() != std::ifstream::traits_type::eof())
        {
            std::pair<T, U> temp_pair;
            apply(temp_pair.first, is);
            apply(temp_pair.second, is);
            val.insert(temp_pair);
        }
    }
};

///////////////////////////////////////////////////////////////////////

template <typename T>
inline void serialize(const T &obj, std::ostream &os)
{
    serializer::apply(obj, os);
}

template <typename T>
inline void serialize(const std::vector<T> &obj, std::ostream &os)
{
    serializer::apply(obj, os);
}

template <typename T1, typename T2>
inline void serialize(const std::map<T1,T2> &obj, std::ostream &os)
{
    serializer::apply(obj, os);
}

template <typename T>
inline void deserialize(T &obj, std::istream &is)
{
    return deserializer::apply(obj, is);
}

template <typename T>
inline void deserialize(std::vector<T> &obj, std::istream &is)
{
    return deserializer::apply(obj, is);
}

#endif // SERIALIZE_H
