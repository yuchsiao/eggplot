#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

#include "chebyshev.h"


#include "eggplot.h"

using namespace std;
using namespace eggp;

#ifndef M_PI
    #define M_PI		3.14159265358979323846
#endif

double normal_pdf (double x, double mean, double sigma);

// Four examples in README.md
void example1(vector<double> &t, vector<double> &x);
void example2(vector<double> &t, vector<double> &x1, vector<double> &x2 );
void example3(vector<double> &t, vector<double> &x1, vector<double> &x2 );
void example4(vector<double> &t, vector<double> &x1, vector<double> &x2 );

// Two extra examples
void exampleChebyshev();
void exampleMultiLine();

int main()
{
    // Prepare sample data
    unsigned nPoint = 51;
    vector<double> x;
    x = linspace(0, 8, nPoint);
    vector<double> pdf11(nPoint);
    vector<double> pdf22(nPoint);
    double mu1 = 1;
    double mu2 = 2;
    double sigma1 = 1;
    double sigma2 = 2;

    for (unsigned i=0; i<nPoint; ++i) {
        pdf11[i] = normal_pdf(x[i], mu1, sigma1);
        pdf22[i] = normal_pdf(x[i], mu2, sigma2);
    }


    // Example 1
    example1(x, pdf11);

    // Example 2
    example2(x, pdf11, pdf22);

    // Example 3
    example3(x, pdf11, pdf22);

    // Example 4
    example4(x, pdf11, pdf22);


    // Example Chebyshev polynomials
    exampleChebyshev();

    // Example many lines
    exampleMultiLine();
    return 0;
}

void example1(vector<double> &t, vector<double> &x) {
    eggp::Eggplot curvePlot;

    curvePlot.plot({t,x});
    curvePlot.exec();
}

void example2(vector<double> &t, vector<double> &x1, vector<double> &x2 ) {
    eggp::Eggplot curvePlot;

    curvePlot.xlabel("x");
    curvePlot.ylabel("pdf_{{/Symbol m},{/Symbol s}} (x)");  // enhanced texts
    curvePlot.plot({ t,x1, t,x2 });  // plot two curves by cascading data pairs
    curvePlot.legend({"({/Symbol m}, {/Symbol s}) = (1, 1)",
                      "({/Symbol m}, {/Symbol s}) = (2, 2)"});
    curvePlot.grid(true);            // turn on grids

    curvePlot.exec();
}

void example3(vector<double> &t, vector<double> &x1, vector<double> &x2 ) {
    eggp::Eggplot curvePlot;

    curvePlot.plot({ t,x1, t,x2 });

    using namespace eggp;

    // multiple property setup in one statement
    curvePlot.linespec(1, {{MarkerSize, "0.5"}, {Marker, "*"}});

    // single property setup in multiple statements
    curvePlot.linespec(2, Color, "b");
    curvePlot.linespec(2, LineWidth, 2);
    curvePlot.linespec(2, Marker, "none");

    curvePlot.exec();
}

void example4(vector<double> &t, vector<double> &x1, vector<double> &x2 ) {
    eggp::Eggplot curvePlot(SCREEN|PNG|EPS|PDF|HTML|SVG);

    // set export file name, default: "eggp-export"
    curvePlot.print("eggp-export-ex4");

    curvePlot.plot({ t,x1, t,x2 });
    curvePlot.exec();
}

void exampleChebyshev() {
    eggp::Eggplot curvePlot;  // default show on screen

    // Setup some random curves
    unsigned nPoint = 50;

    vector<double> t = linspace(-1,1,nPoint);  // same as MATLAB
    vector<double> x0(nPoint);
    vector<double> x1(nPoint);
    vector<double> x2(nPoint);
    vector<double> x3(nPoint);
    vector<double> x4(nPoint);
    vector<double> x5(nPoint);

    for (unsigned i=0; i<nPoint; ++i) {
        x0[i] = chebyshev::T0(t[i]);
        x1[i] = chebyshev::T1(t[i]);
        x2[i] = chebyshev::T2(t[i]);
        x3[i] = chebyshev::Tn(3, t[i]);
        x4[i] = chebyshev::Tn(4, t[i]);
        x5[i] = chebyshev::Tn(5, t[i]);
    }

    // Setup labels
    curvePlot.xlabel("x");
    curvePlot.ylabel("T_i(x)");
    curvePlot.title("Chebyshev Polynomials");

    // Set no markers for all lines
    for (unsigned i=1; i<=6; ++i) {
        curvePlot.linespec(i, Marker, "none");  // set to no marker
    }

    // Plot command
    curvePlot.plot({t,x0,
                    t,x1,
                    t,x2,
                    t,x3,
                    t,x4,
                    t,x5});

    // Setup legends
    curvePlot.legend({"T_0(x)", "T_1(x)", "T_2(x)", "T_3(x)", "T_4(x)", "T_5(x)"});

    // Execute. exec() must be the last command.
    // the order of other commands are interchangeable.
    curvePlot.exec();
}


void exampleMultiLine() {
    // Show on screen and
    // export to .png, .eps, .pdf, .html, .svg files
    eggp::Eggplot curvePlot(SCREEN|PNG|EPS|PDF|HTML|SVG);

    // Setup some random curves
    unsigned nPoint = 50;

    vector<double> t(nPoint);
    vector<double> x1(nPoint);
    vector<double> x2(nPoint);
    vector<double> x3(nPoint);
    vector<double> x4(nPoint);
    vector<double> x5(nPoint);
    vector<double> x6(nPoint);
    vector<double> x7(nPoint);
    vector<double> x8(nPoint);
    vector<double> x9(nPoint);
    vector<double> x10(nPoint);
    vector<double> x11(nPoint);

    for (unsigned i=0; i<nPoint; ++i) {
        t[i] = static_cast<double>(i*2);
        x1[i] = sin(t[i]/50*2*M_PI);
        x2[i] = cos(t[i]/40*2*M_PI);
        x3[i] = x1[i]*x2[i];
        x4[i] = exp(t[i]/150)-1;
        x5[i] = -(exp(t[i]/150)-1);
        x6[i] = lgamma(t[i]/5)/40;
        x7[i] = 0;
        x8[i] = sin(t[i]/60*2*M_PI)*cos(t[i]/60*2*M_PI);
        x9[i] = -(1/(1+exp((-t[i]+nPoint)/10))-0.5)*2;
        x10[i] = (1/(1+exp((-t[i]+nPoint)/10))-0.5)*2;
        x11[i] = (sin(t[i]/50*2*M_PI)+cos(t[i]/40*2*M_PI))/2;
    }

    curvePlot.xlabel("time t (sec)");
    curvePlot.ylabel("f_i(t)\\n(voltage)");
    curvePlot.plot({t,x1,
                    t,x2,
                    t,x3,
                    t,x4,
                    t,x5,
                    t,x6,
                    t,x7,
                    t,x8,
                    t,x9,
                    t,x10,
                    t,x11});

    // Setup two properties for Line 3 in one statement
    curvePlot.linespec(3, {{Marker, "*"}, {LineStyle, "--"}});

    // Order of setups does not matter
    curvePlot.linespec(1, {{MarkerSize, "1.9827"}});

    // For a single property setup, three-argument version is also available
    curvePlot.linespec(2, LineWidth, "2");

    // Both string and numeric values are acceptable
    curvePlot.linespec(2, MarkerSize, 3);

    // Color can be specified in five ways
    // hex code, decimal code, color name, or color name shortcut
    curvePlot.linespec(1, Color, "[1.0, 0.01, 0.3]");
    curvePlot.linespec(1, Color, "(255, 40, 60)");
    curvePlot.linespec(1, Color, "#ff0030");
    curvePlot.linespec(1, Color, "red");
    curvePlot.linespec(1, Color, "r");

    // Setup legends for curve 1-3
    // the rest will be plotted with default legends
    curvePlot.legend({"sin", "cos", "sin*cos"});

    // Turn on grid lines
    curvePlot.grid(true);

    // Change export filename
    curvePlot.print("eggp-test");

    // Execute
    // the orders of all above are arbitrary
    // exec() has to be the last one
    curvePlot.exec();

}


double normal_pdf (double x, double mean, double sigma) {
    x -= mean;
    x /= sigma;
    return 1.0/sqrt(2*M_PI)/sigma * exp(-x*x/2);
}
