#include <iostream>
#include <vector>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;


//M, v, u, x i , y i , a i , b i , c i , d i
//Eigen Matrix, vector, map, double

class Spline {
 private:
    int temp;
 public:
    Spline(vector<double> xs, vector<double> ys) {
        //initialize my matrix with dimensions xs.ys
        // compute a-s here
    }

    // double get_value(const double x) const {
    double get_value(double x) const {
        return 0;
    }
};

int main(int argc, char *argv[]) {
    vector<double> xs{ 0., 0.16, 0.42, 0.6425, 0.8575};
    vector<double> ys{100., 183. , 235. , 40. , 15.};

    Spline spline1(xs,ys);

    for (double x = 0.0 ; x <= 1.0 ; x += 0.1) {
        std::cout << "Value for " << x << " is ";
        std::cout << spline1.get_value(x) << std::endl;
    }
}
