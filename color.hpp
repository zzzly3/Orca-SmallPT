#pragma once
#include <tuple>
#include <utility>
#include <cmath>
struct RayColor {
    double r, g, b, sum, n, l;
    static inline double sample(unsigned short *Xi)
    {
        return erand48(Xi) * 400 + 380;
    }
    inline double a(double r_, double g_, double b_) const
    {
        return (r_ * r + g_ * g + b_ * b) / sum;
    }
    RayColor(double lambda): l(lambda)
    {
        double alpha = 0;
        if (lambda >= 380.0 && lambda < 440.0) {
            r = -1.0 * (lambda - 440.0) / (440.0 - 380.0);
            g = 0.0;
            b = 1.0;
        }
        else if (lambda >= 440.0 && lambda < 490.0) {
            r = 0.0;
            g = (lambda - 440.0) / (490.0 - 440.0);
            b = 1.0;
        }
        else if (lambda >= 490.0 && lambda < 510.0) {
            r = 0.0;
            g = 1.0;
            b = -1.0 * (lambda - 510.0) / (510.0 - 490.0);
        }
        else if (lambda >= 510.0 && lambda < 580.0) {
            r = (lambda - 510.0) / (580.0 - 510.0);
            g = 1.0;
            b = 0.0;
        }
        else if (lambda >= 580.0 && lambda < 645.0) {
            r = 1.0;
            g = -1.0 * (lambda - 645.0) / (645.0 - 580.0);
            b = 0.0;
        }
        else if (lambda >= 645.0 && lambda <= 780.0) {
            r = 1.0;
            g = 0.0;
            b = 0.0;
        }
        else {
            r = 0.0;
            g = 0.0;
            b = 0.0;
        }
        if (lambda >= 380.0 && lambda < 420.0) {
            alpha = 0.30 + 0.70 * (lambda - 380.0) / (420.0 - 380.0);
        }
        else if (lambda >= 420.0 && lambda < 701.0) {
            alpha = 1.0;
        }
        else if (lambda >= 701.0 && lambda < 780.0) {
            alpha = 0.30 + 0.70 * (780.0 - lambda) / (780.0 - 700.0);
        }
        else {
            alpha = 0.0;
        }
        r *= alpha;
        g *= alpha;
        b *= alpha;
        sum = r + g + b;
        // constexpr double A = 1.53974, B = 4.6528e3; // ???
        constexpr double A = 1.321631, B = 6.070796e3, C = -7.062305e8, D = 6.147861e13;
        double l2 = lambda * lambda, l4 = l2 * l2, l6 = l4 * l2;
        n =  A + B / l2 + C / l4 + D / l6;
    }
};