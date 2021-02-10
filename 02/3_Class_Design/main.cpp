#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "elec4_util.h"

using std::vector;
using std::string;

int main(int argc, char *argv[]) {
    vector<double> xs{ 0., 0.16, 0.42, 0.6425, 0.8575};
    vector<double> ys{100., 183., 235., 40., 15.};

    if (argc > 1) {
        string func_name{argv[1]};

        if (func_name.compare("linear") == 0) {
            ELEC4::LinearInterpolation interpolate(xs, ys);
            for (double x = 0.0 ; x <= 1.0 ; x += 0.1) {
                std::cout << "Value for "
                          << std::fixed << std::setprecision(1) << x << " is "
                          << std::setw(8) << std::right << std::fixed
                          << std::setprecision(3)
                          << interpolate.get_value(x) << std::endl;
            }
        } else if (func_name.compare("spline") == 0) {
            ELEC4::Spline interpolate(xs, ys);
            // std::cout << interpolate.get_matrix() << std::endl;
            for (double x = 0.0 ; x <= 1.0 ; x += 0.1) {
                std::cout << "Value for "
                          << std::fixed << std::setprecision(1) << x << " is "
                          << std::setw(8) << std::right << std::fixed
                          << std::setprecision(3)
                          << interpolate.get_value(x)<< std::endl;
            }
        } else {
            std::cout << "Please use \"linear\" or \"spline\" as first argument"
                      << std::endl;
        }
    } else {
        std::cout << "Please use \"linear\" or \"spline\" as first argument"
                  << std::endl;
    }
}

// std::cout << std::right << std::setw(6) << n * 100;
//     std::cout << std::right << std::setw(8) << cnt[n] << " ";