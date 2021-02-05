#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

int main() {
    MatrixXd ma(3, 3);
    VectorXd b(3);

    ma << 1, 2, 3, 4, 5, 6, 7, 8, 10;
    b << 3, 3, 4;

    cout << "Here is the matrix A:\n" << ma << endl;
    cout << "Here is the vector b:\n" << b << endl;

    VectorXd x = ma.colPivHouseholderQr().solve(b);
    cout << "The solution is:\n" << x << endl;
}