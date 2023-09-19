#include <iostream>
#include <cmath>


class  ComplexNumber {
public:
    int real;
    int img;
    ComplexNumber() : real(0), img(0) { }
    ComplexNumber(int r, int i): real(r),img(i) { }
    ComplexNumber operator + (const ComplexNumber& c);

    ComplexNumber operator - (const ComplexNumber& c);

    ComplexNumber operator* (int num);

    double static abs(const ComplexNumber& c) {
        return sqrt(pow(c.real, 2) + pow(c.img, 2));
    }

    void static sort(std::vector<ComplexNumber>& arr) {
        int i, j;
        double key;
        for (i = 1; i < arr.size(); i++) {
            key = ComplexNumber::abs(arr[i]);
            j = i - 1;

            while (j >= 0 && ComplexNumber::abs(arr[j]) > key) {
                ComplexNumber tmp = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = tmp;
                j--;
            }
        }
    }
};

