#ifndef SERIALIZE_H
#define SERIALIZE_H
#pragma once

#include "deserialize_exception.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <map>
#include <string>

enum DATA_TYPE {
    BASIC,
    VECTOR,
    MAP,
    STRING,
    CLASS
};

struct serializer {
    template <typename T>
    static void apply(const T &obj, std::ostream &os) {
        const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&obj); //ptr - is pointer to 1 byte of info

        std::ostream_iterator<uint8_t> oi(os);
        os << uint8_t(BASIC);

        std::copy(ptr, ptr + sizeof(T), oi); //побайтовое копирование
    }
    template <typename T>
    static void apply(const std::vector<T> &obj, std::ostream &os) {
        os << uint8_t(DATA_TYPE::VECTOR);
        os << uint32_t(obj.size());
        for (auto it:obj)
        {
            apply(it, os);
        }
    }
    template <typename T, typename U>
    static void apply(const std::map<T,U> &obj, std::ostream &os) {
        os << uint8_t(DATA_TYPE::MAP);
        os << uint32_t(obj.size());
        for (auto& it : obj)
        {
            apply(it.first, os);
            apply(it.second, os);
        }
    }
    static void apply(const std::string &obj, std::ostream &os) {
        os << uint8_t(DATA_TYPE::STRING);
        os << uint32_t(obj.size() + 1);
        const char* char_str = obj.c_str(); //return char[] with '\0' at the end
        for (size_t i=0; i <= obj.size(); ++i) // <= because \0 is at array[obj.size()]
        {
            apply(char_str[i], os);
        }
    }
};

// ####################################### deserialization #######################################


struct deserializer {
    template <typename T>
    static void apply(T &val, std::istream &is) {

        uint8_t check;
        is >> check;
        if (check != DATA_TYPE::BASIC)
            throw BasicDeserializationException();

        uint8_t *ptr = reinterpret_cast<uint8_t*>(&val);

        std::istream_iterator<uint8_t> ii(is);

        std::copy_n(ii, sizeof(T), ptr);
    }

    template <typename T>
    static void apply(std::vector<T> &val, std::istream &is) {

        uint8_t check;
        is >> check;
        if (check != DATA_TYPE::VECTOR)
            throw VectorDeserializationException();

        uint32_t vec_size;
        is >> vec_size;
        while (is.peek() != std::ifstream::traits_type::eof() &&
               val.size() != size_t(vec_size))
        {
            T temp_value;
            apply(temp_value, is);
            val.push_back(temp_value);
        }
    }
    template <typename T, typename U>
    static void apply(std::map<T, U> &val, std::istream &is) {
        uint8_t check;
        is >> check;
        if (check != DATA_TYPE::MAP)
            throw MapDeserializationException();

        uint32_t map_size;
        is >> map_size;

        while (is.peek() != std::ifstream::traits_type::eof()&&
               val.size() != size_t(map_size))
        {
            std::pair<T, U> temp_pair;
            apply(temp_pair.first, is);
            apply(temp_pair.second, is);
            val.insert(temp_pair);
        }
    }
    static void apply(std::string &val, std::istream &is) {
        uint8_t check;
        is >> check;
        if (check != DATA_TYPE::STRING)
            throw StringDeserializationException();

        uint32_t str_size;
        is >> str_size;

        char char_arr[str_size];
        size_t counter = 0;
        while (is.peek() != std::ifstream::traits_type::eof()&&
               counter < size_t(str_size))
        {
            apply(char_arr[counter], is);
            ++counter;
        }
        std::string temp_str(char_arr);
        val = temp_str;
    }
};

// ####################################### inline functions #######################################

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

template <typename T1, typename T2>
inline void serialize(const std::string &obj, std::ostream &os)
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

template <typename T, typename U>
inline void deserialize(std::map<T, U> &obj, std::istream &is)
{
    return deserializer::apply(obj, is);
}

inline void deserialize(std::string &obj, std::istream &is)
{
    return deserializer::apply(obj, is);
}
#endif // SERIALIZE_H
