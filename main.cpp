#include <iostream>
#include "smarter_unique_ptr.h"

using namespace std;

int main()
{
    std::cout<<"[] " << (-5*( (-2>>31)) -2)%5 << std::endl;
    std::cout<<"[] " << (-5*(3>>31) + 3)%5 << std::endl;
    std::cout<<"[] " << (-5*(1>>31) + 1)%5 << std::endl;

    unsigned int N1 = 10;
    int *arr1 = new int[N1];
    for(int i = 0; i < N1; i++)
        arr1[i] = i;

    unsigned int N2 = 5;
    int *arr2 = new int[N2];
    arr2[0] = 6;

    smarter_unique_ptr<int, array_deleter<int> > ptr1(arr1, N1);
    smarter_unique_ptr<int, array_deleter<int> > copy_ptr1(arr1, N1);
    if(ptr1 == copy_ptr1)
    {
        std::cout << "hey" << std::endl;
    }
    std::cout << ptr1.getLength() << std::endl;

    smarter_unique_ptr<int, array_deleter<int> > ptr2(arr2, N2);
    std::cout << ptr1.getLength() << " " << ptr2.getLength() << std::endl;
    ptr1.swap(ptr2);
    std::cout << ptr1.getLength() << " " << ptr2.getLength() << std::endl;

    //memory leak test 1
    /*
    while(true)
    {
        int N3 = 100000;
        int *arr3 = new int[N3];
        smarter_unique_ptr<int, array_deleter<int> > pTest(arr3, N3);
    }
    */

    //memory leak test 2
    /*
    while(true)
    {
        int N3 = 100000;
        int *arr3 = new int[N3];
        smarter_unique_ptr<int, array_deleter<int> > pTest(arr3, N3);
        pTest = std::move(pTest);
    }
    */

    //memory leak test 3
    /*
    while(true)
    {
        int N3 = 100000;
        int *arr3 = new int[N3];
        smarter_unique_ptr<int, array_deleter<int> > pTest1(arr3, N3);
        smarter_unique_ptr<int, array_deleter<int> > pTest2( std::move(pTest1));
    }
    */

    int arr4 = *ptr1;
    std::cout << arr4 << std::endl;

    ptr1.reset(arr2, N2);
    int* arr3 = ptr1.get();

    smarter_unique_ptr<int, array_deleter<int> > ptmp;
    if(!ptmp)
        std::cout << "i don't own anything" << std::endl;

    smarter_unique_ptr<int, array_deleter<int> > ptr3 (std::move(ptmp));

    smarter_unique_ptr<int, array_deleter<int> >ptr4(std::move(ptr3));

    ptr4 = std::move(ptr4);

    std::cout << "test []: " << ptr2[-9] << std::endl;


    ptr1.release();
    std::cout << "test isNull: "<< ptr1.isNull() << std::endl;

    return 0;
}
