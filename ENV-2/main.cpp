#include <vector>
#include <iostream>
#include "complexNumber.h"

int main()
{
    std::vector<ComplexNumber> arr(4);
    arr[0] = *(new ComplexNumber(1, 1));
    arr[1] = *(new ComplexNumber(3, 3));
    arr[2] = *(new ComplexNumber(2, 1));
    arr[3] = *(new ComplexNumber(4, 1));
    
    ComplexNumber::sort(arr);
    
    for (int i = 0; i < 4; i++) {
        std::cout << arr[i].real << "," << arr[i].img << " ";
    }

}