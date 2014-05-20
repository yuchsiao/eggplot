#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

#include <eggplot.h>

using namespace std;
using namespace eggp;

//constexpr double pi() {
//    return std::atan(1)*4;
//}
#ifndef M_PI
    #define M_PI		3.14159265358979323846
#endif

int main()
{
    eggp::Eggplot test;

    vector<double> t(100);
    vector<double> x(100);
    vector<double> y(100);
    vector<double> z(100);

    for (int i=0; i<100; ++i) {
        t[i] = static_cast<double>(i);
        x[i] = sin(t[i]/50*2*M_PI);
        y[i] = cos(t[i]/40*2*M_PI);
        z[i] = x[i]*y[i];
    }

    test.xlabel("Time_t\\n(sec)");
    test.ylabel("Voltage (V)");
    test.plot({t,x,t,y,t,z});
    test.linespec(3, {{Marker, "*"}, {LineStyle, "--"}});
    test.linespec(1, {{MarkerSize, "4.9827"}});

    test.legend({"sin", "cos", "sin*cos"});
    test.show();

    return 0;
}
