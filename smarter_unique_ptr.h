#ifndef SMARTER_UNIQUE_PTR_H
#define SMARTER_UNIQUE_PTR_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

template<typename T>
class default_deleter
{
public:
    static void eliminate(T *ptr)
    {
        delete ptr;
    }
};

template<typename T>
class array_deleter
{
public:
    static void eliminate(T *ptr)
    {
        delete[] ptr;
    }
};


template<typename T, typename deleter_fn = default_deleter<T> >
class smarter_unique_ptr
{
public:
    smarter_unique_ptr():ptr(nullptr), length(0){}

    smarter_unique_ptr(T *p, unsigned int len = 1):ptr(p)
    {
        length = new unsigned int;
        *length = (ptr == nullptr)?0:len;
    }

    smarter_unique_ptr(smarter_unique_ptr&& other)
    {
        ptr = other.ptr;
        length = other.length;
        other.length = nullptr;
        other.ptr = nullptr;
    }

    smarter_unique_ptr( const smarter_unique_ptr& ) = delete;
    void operator=( const smarter_unique_ptr& ) = delete;

    virtual ~smarter_unique_ptr()
    {
        deleter_fn::eliminate(ptr);
        delete length;
        ptr = nullptr;
        length = nullptr;
    }

    unsigned int getLength()
    {
        return (length == nullptr)?0:*length;
    }

    T* get() const
    {
        return ptr;
    }

    bool isNull() const
    {
        return ptr == 0;
    }

    void reset(T *other = 0, unsigned int len = 0)
    {
        if(ptr == other)
            return;
        deleter_fn::eliminate(ptr);
        *length = len;
        ptr = other;
    }

    void swap(smarter_unique_ptr<T, deleter_fn>& other)
    {
        if(&other == this)
            return;
        // std::swap? No, thanks.
        T* sas = ptr;
        ptr = other.ptr;
        other.ptr = sas;
        unsigned int* ses = length;
        length = other.length;
        other.length = ses;
    }

    smarter_unique_ptr& operator=(smarter_unique_ptr&& other)
    {
        if (this != &other)
            return *this;

        deleter_fn::eliminate(ptr);
        ptr = other.ptr;
        delete length;
        length = other.length;
        other.ptr = nullptr;
        other.length = nullptr;
        return *this;
    }

    T* release()
    {
        T* sas = ptr;
        ptr = nullptr;
        *length = 0;
        return sas;
    }

    T& operator*() const
    {
        return *ptr;
    }

    T* operator->() const
    {
        return ptr;
    }

    bool setValue(size_t index, T &&value) {
        if (length == nullptr || ptr == nullptr)
            return false;
        if (index < *length) {
                ptr[index] = value;
                return true;
        }
        return false;
    }

    bool getValue(size_t index, T& dst) {
        if (length == nullptr || ptr == nullptr)
            return false;
        if (index < *length) {
            dst = ptr[index];
            return true;
        }
        return false;
    }

    operator bool()
    {
        return ptr != nullptr;
    }

    bool operator !()
    {
        return ptr == nullptr;
    }

    bool operator==(const smarter_unique_ptr<T, deleter_fn>& other)
    {
        return ptr == other.ptr;
    }

    bool operator!=(const smarter_unique_ptr<T, deleter_fn>& other)
    {
        return ptr != other.ptr;
    }

    T& operator[](int idx)
    {
        return ptr[(idx - *length*(idx>>31))%*length]; //very smart
    }

    protected:
        T* ptr;
        unsigned int *length; //pointer since i do not want to rewrite it in smarter_shared_ptr

    private:

};

#endif // SMARTER_UNIQUE_PTR_H
