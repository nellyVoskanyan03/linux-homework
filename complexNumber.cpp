#include <iostream>
#include <vector>
#include "complexNumber.h"
ComplexNumber ComplexNumber::operator + (const ComplexNumber& c) {
    ComplexNumber tmp;
    tmp.real = real + c.real;
    tmp.img = img + c.img;
    return tmp;
}

ComplexNumber ComplexNumber::operator - (const ComplexNumber& c) {
    ComplexNumber tmp;
    tmp.real = real - c.real;
    tmp.img = img - c.img;
    return tmp;
}

ComplexNumber ComplexNumber::operator* (int num) {
    ComplexNumber tmp;
    tmp.real = real * num;
    tmp.img = img * num;
    return tmp;
}



