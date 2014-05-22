#include "eggplot.h"

#include<algorithm>
#include<fstream>
#include<stdexcept>
#include<cstdlib>
#include<iostream>
#include<sstream>
#include<cmath>

using namespace std;

namespace eggp {


Eggplot::Eggplot(unsigned mode)
    : filenamePrefix("eggp"),
      labelX(),
      labelY(),
      labelTitle(),
      legendVec(0),
      lineSpecInput(),
      lineSpec(),
      lineSpecAqua(),
      lineSpecCanvas(),
      lineSpecOther(),
      nCurve(0),
      isGridded(false),
      filenameExport("eggp-export")
{
    //* Test if terminal exists
    this->existsAqua   = this->existsTerminal("aqua");
    this->existsWxt    = this->existsTerminal("wxt");
    this->existsCairo  = this->existsTerminal("cairo");
    this->existsCanvas = this->existsTerminal("canvas");
    this->existsSvg    = this->existsTerminal("svg");

    //* Set up modes
    flagScreen = (SCREEN & mode) ? true : false;
    flagPng    = (PNG    & mode) ? true : false;
    flagEps    = (EPS    & mode) ? true : false;
    flagPdf    = (PDF    & mode) ? true : false;
    flagHtml   = (HTML   & mode) ? true : false;
    flagSvg    = (SVG    & mode) ? true : false;
}



void Eggplot::xlabel(const std::string &label)
{
    this->labelX = label;
}

void Eggplot::ylabel(const std::string &label)
{
    this->labelY = label;
}

void Eggplot::title(const std::string &label)
{
    this->labelTitle = label;
}

void Eggplot::legend(std::initializer_list<string> legendVec)
{
    this->legendVec.resize(legendVec.size());
    std::copy(legendVec.begin(), legendVec.end(), this->legendVec.begin());
}

void Eggplot::linespec(unsigned lineIndex, LineSpecInput lineSpec)
{
    //* Store but no process

    if (lineIndex<=0) {
        throw out_of_range("Line index must be a positive integer");
    }

    this->lineSpecInput.push_back({lineIndex, lineSpec});
}


void Eggplot::linespec(unsigned lineIndex, LineProperty property, string value)
{
    linespec(lineIndex, {{property, value}});
}

void Eggplot::linespec(unsigned lineIndex, LineProperty property, double value)
{
    linespec(lineIndex, {{property, to_string(value)}});
}

void Eggplot::grid(bool flag)
{
    this->isGridded = flag;
}

void Eggplot::plot(initializer_list<DataVector> il)
{
    //* Take Matlab-like commands but only store data
    //* Actually plotting happens at Eggplot::show()

    //* check if even number of data vectors
    if (il.size() % 2){
        throw length_error("Arguements must be even number of data vectors");
    }

    //* check if columns are of equal lengths
    ofstream fout( (this->filenamePrefix + string(".dat")).c_str() );
    this->nCurve = 0;
    for (auto it=il.begin(); it!=il.end(); ++it) {
        auto itEven = it++;
        if (it->size()!=itEven->size()){
            throw length_error("Pairwise data vectors must have the same lengths");
        }
        const DataVector &x = *itEven;
        const DataVector &y = *it;

        fout << "# Curve " << (this->nCurve)++ << endl;
        for (unsigned i=0; i<x.size(); ++i) {
            fout << x[i] << "," << y[i] << endl;
        }
        fout << endl << endl;
    }
    fout.close();
}

void Eggplot::print(const string &filenameExport)
{
    this->filenameExport = filenameExport;
}

void Eggplot::exec()
{
    //* Check if there are data
    if (this->nCurve==0) {
        return;
    }
    //* Check if legend size is zero
    else if (this->legendVec.size()==0) {
        this->legendVec.resize(this->nCurve);
        for (unsigned i=0; i<this->nCurve; ++i) {
            this->legendVec[i] = to_string(i+1);
        }
    }
    //* Check if legend size matches data pair number. If not, pad it.
    else if (this->legendVec.size()!=this->nCurve) {
        //throw length_error("Legends must match the number of data vector pairs");
        unsigned nLegend = this->legendVec.size();
        this->legendVec.resize(this->nCurve);
        for (unsigned i=nLegend; i<nCurve; ++i) {
            this->legendVec[i] = "Data " + to_string(i+1);
        }
    }

    prepareLineSpec();

    if (this->flagScreen) {
        gpScreen();
    }
    if (this->flagPng) {
        gpPng();
    }
    if (this->flagEps) {
        gpEps();
    }
    if (this->flagPdf) {
        gpPdf();
    }
    if (this->flagHtml) {
        gpHtml();
    }
    if (this->flagSvg) {
        gpSvg();
    }
}


bool Eggplot::existsTerminal(string terminalName)
{
    bool result = false;

    string commandTest = "gnuplot -e \"set print '"+this->filenamePrefix
            +"-exists-" + terminalName + "'; if (strstrt(GPVAL_TERMINALS, '" + terminalName + "')) print 1; else print 0\"";
    system(commandTest.c_str());

    ifstream fin((this->filenamePrefix+"-exists-"+terminalName).c_str());
    fin >> result;
    fin.close();

    return result;
}

void Eggplot::prepareLineSpec()
{
    LineSpec::resetLineCount();
    this->lineSpec.resize(nCurve);

    for (auto it=lineSpecInput.begin(); it!=lineSpecInput.end(); ++it) {
        unsigned lineIndex = it->first;
        if (lineIndex > nCurve) {
            continue;
        }
        for (auto itProperty=it->second.begin(); itProperty!=it->second.end(); ++itProperty) {
            this->lineSpec[lineIndex-1].set(*itProperty);
        }
    }
}

void foutGridSetting(ofstream &fout, TerminalType tt) {
    fout << "set grid lc rgb '" << LineSpec::gridColor << "' lw 1 lt " << LineSpec::getGridLineType(tt) << endl;
}

void Eggplot::gpScreen()
{
    //* Generate gnuplot batch file
    string filename = this->filenamePrefix+".gp";
    ofstream fout(filename.c_str());

    gpHeader(fout);

    //* Set terminal and line style
    if (existsAqua) {
        fout << "set terminal aqua dashed enhanced" << endl;
        if (this->isGridded) {
            foutGridSetting(fout, TERM_AQUA);
        }
        for (unsigned i=0; i<this->lineSpec.size(); ++i) {
            fout << this->lineSpec[i].toStringAqua() << endl;
        }
    }
    else if (existsWxt) {
        fout << "set terminal wxt dashed enhanced" << endl;
        if (this->isGridded) {
            foutGridSetting(fout, TERM_WXT);
        }
        for (unsigned i=0; i<this->lineSpec.size(); ++i) {
            fout << this->lineSpec[i].toStringWxtCairoSvg() << endl;
        }
    }
    else {
        fout << "# No supported display terminal found. Line styles may be not accurate." << endl;
        if (this->isGridded) {
            foutGridSetting(fout, TERM_OTHER);
        }
        for (unsigned i=0; i<this->lineSpec.size(); ++i) {
            fout << this->lineSpec[i].toStringWxtCairoSvg() << endl;
        }
    }
    gpCurve(fout, filename);

    fout.close();

}

void Eggplot::gpPng()
{
    //* Generate gnuplot batch file
    string filename = this->filenamePrefix+"-png.gp";
    string filenameExport = this->filenameExport+".png";
    ofstream fout(filename.c_str());

    gpHeader(fout);

    //* Set terminal and line style
    if (existsCairo) {
        fout << "set terminal pngcairo dashed enhanced" << endl;
        fout << "set output '" << filenameExport << "'" << endl;
        if (this->isGridded) {
            foutGridSetting(fout, TERM_CAIRO);
        }
        for (unsigned i=0; i<this->lineSpec.size(); ++i) {
            fout << this->lineSpec[i].toStringWxtCairoSvg() << endl;
        }
    }
    else {
        fout << "# Cairo terminal not found. Default png terminal used instead." << endl
             << "# Line styles may be not accurate." << endl;
        fout << "set terminal png dashed enhanced" << endl;
        fout << "set output '" << filenameExport << "'" << endl;
        if (this->isGridded) {
            foutGridSetting(fout, TERM_OTHER);
        }
        for (unsigned i=0; i<this->lineSpec.size(); ++i) {
            fout << this->lineSpec[i].toStringWxtCairoSvg() << endl;
        }
    }
    gpCurve(fout, filename);

    fout.close();
}

void Eggplot::gpEps()
{
    //* Generate gnuplot batch file
    string filename = this->filenamePrefix+"-eps.gp";
    string filenameExport = this->filenameExport+".eps";
    ofstream fout(filename.c_str());

    gpHeader(fout);

    //* Set terminal and line style
    string gridColor = "#cccccc";

    if (existsCairo) {
        fout << "set terminal epscairo transparent color dashed enhanced" << endl;
        fout << "set output '" << filenameExport << "'" << endl;
        if (this->isGridded) {
            foutGridSetting(fout, TERM_CAIRO);
        }
        for (unsigned i=0; i<this->lineSpec.size(); ++i) {
            fout << this->lineSpec[i].toStringWxtCairoSvg() << endl;
        }
    }
    else {
        fout << "# Cairo terminal not found. Postscript terminal used instead." << endl
             << "# Line styles may be not accurate." << endl;
        fout << "set terminal postscript eps color colortext dashed" << endl;
        fout << "set output '" << filenameExport << "'" << endl;
        if (this->isGridded) {
            foutGridSetting(fout, TERM_OTHER);
        }
        for (unsigned i=0; i<this->lineSpec.size(); ++i) {
            fout << this->lineSpec[i].toStringWxtCairoSvg() << endl;
        }
    }

    gpCurve(fout, filename);

    fout.close();
}

void Eggplot::gpPdf()
{
    //* Generate gnuplot batch file
    string filename = this->filenamePrefix+"-pdf.gp";
    string filenameExport = this->filenameExport+".pdf";
    ofstream fout(filename.c_str());

    gpHeader(fout);

    //* Set terminal and line style
    if (existsCairo) {
        fout << "set terminal pdfcairo transparent color dashed enhanced" << endl;
        fout << "set output '" << filenameExport << "'" << endl;
        if (this->isGridded) {
            foutGridSetting(fout, TERM_CAIRO);
        }
        for (unsigned i=0; i<this->lineSpec.size(); ++i) {
            fout << this->lineSpec[i].toStringWxtCairoSvg() << endl;
        }
    }

    gpCurve(fout, filename);

    fout.close();
}

void Eggplot::gpHtml()
{
    //* Generate gnuplot batch file
    string filename = this->filenamePrefix+"-html.gp";
    string filenameExport = this->filenameExport+".html";
    ofstream fout(filename.c_str());

    gpHeader(fout);

    //* Set terminal and line style
    if (existsCairo) {
        fout << "set terminal canvas dashed enhanced" << endl;
        fout << "set output '" << filenameExport << "'" << endl;
        if (this->isGridded) {
            foutGridSetting(fout, TERM_CANVAS);
        }
        for (unsigned i=0; i<this->lineSpec.size(); ++i) {
            fout << this->lineSpec[i].toStringWxtCairoSvg() << endl;
        }
    }

    gpCurve(fout, filename);

    fout.close();
}

void Eggplot::gpSvg()
{
    //* Generate gnuplot batch file
    string filename = this->filenamePrefix+"-svg.gp";
    string filenameExport = this->filenameExport+".svg";
    ofstream fout(filename.c_str());

    gpHeader(fout);

    //* Set terminal and line style
    if (existsCairo) {
        fout << "set terminal svg dashed enhanced" << endl;
        fout << "set output '" << filenameExport << "'" << endl;
        if (this->isGridded) {
            foutGridSetting(fout, TERM_SVG);
        }
        for (unsigned i=0; i<this->lineSpec.size(); ++i) {
            fout << this->lineSpec[i].toStringWxtCairoSvg() << endl;
        }
    }

    gpCurve(fout, filename);

    fout.close();

}

void Eggplot::gpHeader(ofstream &fout)
{
    fout << "# Gnuplot script file" << endl;
    fout << "# Automatically generated by eggplot Ver. " << version << endl;
    fout << "set datafile separator ','" << endl;
}

void Eggplot::gpCurve(ofstream &fout, const string &filename)
{
    fout << "set style increment userstyle" << endl;
    fout << "set autoscale" << endl;
    fout << "unset log" << endl;
    fout << "unset label" << endl;
    fout << "set xtic auto" << endl;
    fout << "set ytic auto" << endl;
    fout << "set title " << this->labelTitle << endl;
    fout << "set xlabel \"" << this->labelX << "\"" << endl;
    fout << "set ylabel \"" << this->labelY << "\"" << endl;
    fout << "plot ";

    for (unsigned i=0; i<this->nCurve; ++i) {

        string filename = this->filenamePrefix+".dat";
        fout << "'" << filename << "' index " << i
             << " title '" << this->legendVec[i]
             << "' with ";

        if (this->lineSpec[i].isPointOnly()) {
            fout << "points";
        }
        else {
            fout << "linespoints";
        }
        fout << ", ";
    }
    fout << endl;

    system(("gnuplot "+filename).c_str());

}



}




