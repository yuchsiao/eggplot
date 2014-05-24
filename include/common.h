#ifndef COMMON_H
#define COMMON_H

#include<map>
#include<string>
#include<vector>

namespace eggp{

enum LineProperty {LineStyle, LineWidth, Marker, MarkerSize, Color};
typedef std::map<LineProperty, std::string> LineSpecInput;

enum Mode         {SCREEN=1, PNG=2, EPS=4, PDF=8, HTML=16, SVG=32};

inline std::vector<double> linspace(double a, double b, unsigned n) {
    std::vector<double> result(n);
    for (unsigned i=0; i<n; i++) {
        result[i] = static_cast<double>(i)/(n-1)*(b-a)+a;
    }
    return result;
}


}

#endif // COMMON_H
