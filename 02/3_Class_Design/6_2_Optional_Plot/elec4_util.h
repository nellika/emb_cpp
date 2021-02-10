#pragma once


#include <cmath>
#include <vector>
#include <utility>
#include <Eigen/Dense>
#include "gnuplot-iostream.h"

namespace ELEC4 {

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

    void plot_graph() const {
        Gnuplot gp;
        std::vector<std::pair<double, double> > xy_pts;
        for (double x = 0.0; x <1.0; x += 0.01) {
            double y = get_value(x);
            xy_pts.push_back(std::make_pair(x, y));
        }

        gp << "set xrange [0:1]" << std::endl
           << "set yrange [-50:300]" << std::endl
           << "set title \"PW Linear interpolation\"" << std::endl
           << "set term wxt title \"Plot for Linear interpolation\"" << std::endl;
        gp << "plot" << gp.file1d(xy_pts)
            << "with lines title 'linear'" << std::endl;
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

        // null tangents
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

    void plot_graph() const {
        Gnuplot gp;
        std::vector<std::pair<double, double> > xy_pts;
        for (double x = 0.0; x <1.0; x += 0.01) {
            double y = get_value(x);
            xy_pts.push_back(std::make_pair(x, y));
        }

        gp << "set xrange [0:1]" << std::endl
           << "set yrange [-50:300]" << std::endl
           << "set title \"Spline interpolation\"" << std::endl
           << "set term wxt title \"Plot for Cubic Spline interpolation\""
           << std::endl;
        gp << "plot" << gp.file1d(xy_pts)
            << "with lines title 'cubic spline'"<< std::endl;
    }

    double get_value(double x) const {
        size_t idx = 0;

        while (x > _xs[idx+1] && idx < _size-1) idx++;

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
