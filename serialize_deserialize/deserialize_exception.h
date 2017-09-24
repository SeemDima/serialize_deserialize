#ifndef DESERIALIZE_EXCEPTION_H
#define DESERIALIZE_EXCEPTION_H

class DeserializeException :public std::exception {};

class BasicDeserializationException :public DeserializeException
{
public :const char *what() const noexcept {return "error of basic data type deserialization";}
};

class VectorDeserializationException :public DeserializeException
{
public :const char *what() const noexcept {return "error of vector deserialization";}
};

class MapDeserializationException :public DeserializeException
{
public :const char *what() const noexcept {return "error of map deserialization";}
};

#endif // DESERIALIZE_EXCEPTION_H
