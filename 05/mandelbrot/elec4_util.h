#pragma once


#include <cmath>
#include <vector>
#include <map>
#include <Eigen/Dense>

namespace ELEC4 {

template<typename tpl_t>
class Clamp {
private:
    tpl_t _min;
    tpl_t _max;
public:
    Clamp(tpl_t min, tpl_t max) : _min{min}, _max{max}{};
    int operator()(int value) {
        return (value < _min) ? 0 : ((value > _max) ? _max : value);
    }
};

//class Commify {
//private:
//  std::string str_;
//public:
//  explicit Commify(int value) {
//    std::ostringstream out;
//    out << value;
//    str_ = out.str();
//    for (int pos = 0; pos > 0; pos-=3) {
//        str_.insert(pos, 1, ',');
//    }
//  }
//  friend std::ostream& operator<<(std::ostream &os, const Commify &c) {
//    os << c.str_;
//    return os;
//  }
//};

class Pixel2Rect {
private:
    double _mid_coord;
    double _max_val;
    double _d;
    double _mux_1;
    double _mux_2;

public:
    Pixel2Rect(double mid_coord_val, double max_val, double d, double mux_1, double mux_2) :
        _mid_coord{mid_coord_val}, _max_val{max_val}, _d{d}, _mux_1{mux_1}, _mux_2{mux_2}{};
    double operator()(int value) {
        return _mid_coord-_mux_1*_d+(double(value)*_mux_2/_max_val);
    }
};

class LinearInterpolation {
 private:
    std::vector<double> _xs, _ys;
    std::vector<double> _as;
    size_t _size;

    void calculateSlope() {
        double a;
        for (size_t i = 0; i < _size; i++) {
            // store slope coeffs into priv member variable
            a = (_ys[i+1] - _ys[i])/(_xs[i+1]- _xs[i]);
            _as.push_back(a);
            // std::cout << a << std::endl;
        }
    }

 public:
    LinearInterpolation(std::vector<double> xs,
                        std::vector<double> ys) : _xs{xs}, _ys{ys} {
        // initialize priv member variables
        _size = _xs.size();

        _xs.push_back(1.0);
        _ys.push_back(_ys[0]);

        calculateSlope();
    }

    double get_value(double x) const {
        int idx = 0;

        // find range for x
        while (x > _xs[idx+1]) idx++;

        return _ys[idx] + _as[idx]*(x - _xs[idx]);
    }

};

class Spline {
 private:
    std::vector<double> _xs, _ys;   // input vectors: x, y coords
    std::vector<double> _as, _bs, _cs;  // vectors for n unknowns
    size_t _size;
    Eigen::MatrixXd _M;
    Eigen::VectorXd _v, _u;  // v for delta ys, u for the unknowns

    void calcNoDerivAndColVector(size_t n, size_t band, double delta) {
        // polynomials >> first row
        for (size_t i = 0; i < 3; i++) {
            _M(band, band + i) = pow(delta, 3-i);
        }
        _v(band) = _ys[n + 1] - _ys[n];
    }

    void solveLPwithConditions() {
        int16_t band = 0;
        double delta = _xs[1] - _xs[0];

        for (size_t i = 0; i < _size-1; i++) {
            calcNoDerivAndColVector(i, band, delta);

            // first derivative >> second row
            _M(band + 1, band + 0) = 3 * delta * delta;
            _M(band + 1, band + 1) = 2 * delta;
            _M(band + 1, band + 2) = 1.0;

            // second derivative >> third row
            _M(band + 2, band + 0) = 6 * delta;
            _M(band + 2, band + 1) = 2.0;

            _M(band + 1, band + 3 + 2) = -1.0;
            _M(band + 2, band + 3 + 1) = -2.0;

            band = 3 * (i + 1);
            delta = _xs[i + 2] - _xs[(i+1)];
            // std::cout << _M << std::endl;
        }

        calcNoDerivAndColVector(_size-1, band, delta);

        // null tangents + out of boundary cond.
        _M(band + 1,  2) = 1.0;

        _M(band + 2,  band + 0) = 3 * delta * delta;
        _M(band + 2,  band + 1) = 2 * delta;
        _M(band + 2,  band + 2) = 1.0;

        // solving the LP
        _u = _M.fullPivHouseholderQr().solve(_v);
    }

    void buildCoeffVectors() {
        int16_t band;
        for (size_t i = 0; i < _size; ++i) {
            band = 3 * i;
            _as.push_back(_u[band + 0]);
            _bs.push_back(_u[band + 1]);
            _cs.push_back(_u[band + 2]);
        }
    }

 public:
    Spline(std::vector<double> xs, std::vector<double> ys) : _xs{xs}, _ys{ys} {
        // initialize member variables
        _size = _xs.size();
        _xs.push_back(1.0);
        _ys.push_back(_ys[0]);

        _M = Eigen::MatrixXd::Zero(_size*3, _size*3);
        _v = Eigen::VectorXd::Zero(_size*3);

        // solve linear problem
        solveLPwithConditions();
        // get coefficients
        buildCoeffVectors();
    }

    double get_value(double x) const {
        size_t idx = 0;

        while (x > _xs[idx+1] && idx < _size-1) idx++;
        // std::cout << "slow: " << idx << std::endl;
        double delta = x - _xs[idx];

        // R_i(x) = a_i (x − x_i )^3 + b_i (x − x_i )^2 + c_i (x − x_i) + d_i
        // return _as[idx] * pow(delta , 3) +
        //        _bs[idx] * pow(delta, 2) +
        //        _cs[idx] * delta +
        //        _ys[idx];

        // Optional - math optimization
        // d(c + d * (b + d * a)) > ad^3 + bd^2 + cd
        return _ys[idx] +
                delta * (_cs[idx] + delta * (_bs[idx] + (delta * _as[idx])));
    }


    Eigen::MatrixXd get_matrix() const { return _M;}
};
}   // namespace ELEC4
