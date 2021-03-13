#pragma once

#include <iostream>

class Car {
 private:
    int dummy;
 public:
    Car() {
        std::cout << dummy << std::endl;
    }
};

class ElectricCar : public Car {
 private:
    double dummydoub;
    int dummy;
 public:
    ElectricCar(double dummydoub, int dummy) : dummydoub{dummydoub}, dummy{dummy}{
        dummy = 1;
    }
};
