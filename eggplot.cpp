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
      filenameExport("")
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

//    //* Set up line type mapping for aqua
//    if (this->existsAqua){
//        lineTypeMappingAqua["-"]  = 1;
//        lineTypeMappingAqua["--"] = 2;
//        lineTypeMappingAqua[":"]  = 4;
//        lineTypeMappingAqua["-."] = 5;
//        lineTypeMappingAqua["none"] = 0;
//    }

//    //* Set up line type mapping for canvas, wxt, cairo (png, eps, pdf), svg (no effect)
//    lineTypeMappingOther["-"]  = 1;
//    lineTypeMappingOther["--"] = 2;
//    lineTypeMappingOther[":"]  = 3;
//    lineTypeMappingOther["-."] = 4;
//    lineTypeMappingOther["none"] = 0;

//    //* Set up point type mapping for aqua [default=5, total=6]
//    if (this->existsAqua) {
//        pointTypeMappingAqua["o"] = 5;
//        pointTypeMappingAqua["+"] = 1;
//        pointTypeMappingAqua["*"] = 3;
//        pointTypeMappingAqua["."] = 5;
//        pointTypeMappingAqua["x"] = 2;
//        pointTypeMappingAqua["s"] = 4;
//        pointTypeMappingAqua["square"] = 4;
//        pointTypeMappingAqua["d"] = 5;
//        pointTypeMappingAqua["diamond"] = 5;
//        pointTypeMappingAqua["^"] = 6;
//        pointTypeMappingAqua["v"] = 6;
//        pointTypeMappingAqua[">"] = 6;
//        pointTypeMappingAqua["<"] = 6;
//        pointTypeMappingAqua["p"] = 5;
//        pointTypeMappingAqua["pentagram"] = 5;
//        pointTypeMappingAqua["h"] = 5;
//        pointTypeMappingAqua["hexagram"] = 5;
//        pointTypeMappingAqua["none"] = 0;
//    }

//    //* Set up point type mapping for canvas [total=9]
//    if (this->existsCanvas) {
//        pointTypeMappingCanvas["o"] = 6;
//        pointTypeMappingCanvas["+"] = 1;
//        pointTypeMappingCanvas["*"] = 3;
//        pointTypeMappingCanvas["."] = 7;
//        pointTypeMappingCanvas["x"] = 2;
//        pointTypeMappingCanvas["s"] = 4;
//        pointTypeMappingCanvas["square"] = 4;
//        pointTypeMappingCanvas["d"] = 5;
//        pointTypeMappingCanvas["diamond"] = 5;
//        pointTypeMappingCanvas["^"] = 8;
//        pointTypeMappingCanvas["v"] = 9;
//        pointTypeMappingCanvas[">"] = 8;
//        pointTypeMappingCanvas["<"] = 9;
//        pointTypeMappingCanvas["p"] = 4;
//        pointTypeMappingCanvas["pentagram"] = 4;
//        pointTypeMappingCanvas["h"] = 8;
//        pointTypeMappingCanvas["hexagram"] = 8;
//        pointTypeMappingCanvas["none"] = 0;
//    }

//    //* Set up point type mapping for cairo, svg, wxt [total=13]
//    pointTypeMappingOther["o"] = 6;
//    pointTypeMappingOther["+"] = 1;
//    pointTypeMappingOther["*"] = 3;
//    pointTypeMappingOther["."] = 7;
//    pointTypeMappingOther["x"] = 2;
//    pointTypeMappingOther["s"] = 4;
//    pointTypeMappingOther["square"] = 4;
//    pointTypeMappingOther["d"] = 12;
//    pointTypeMappingOther["diamond"] = 12;
//    pointTypeMappingOther["^"] = 8;
//    pointTypeMappingOther["v"] = 10;
//    pointTypeMappingOther[">"] = 9;
//    pointTypeMappingOther["<"] = 11;
//    pointTypeMappingOther["p"] = 5;
//    pointTypeMappingOther["pentagram"] = 5;
//    pointTypeMappingOther["h"] = 13;
//    pointTypeMappingOther["hexagram"] = 13;
//    pointTypeMappingOther["none"] = 0;

//    //* Set up color shortcut mapping
//    colorMapping['y'] = "yellow";
//    colorMapping['m'] = "magenta";
//    colorMapping['c'] = "cyan";
//    colorMapping['r'] = "red";
//    colorMapping['g'] = "green";
//    colorMapping['b'] = "blue";
//    colorMapping['w'] = "white";
//    colorMapping['k'] = "black";

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

void Eggplot::show()
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
    //* Check if legend size matches data pair number
    else if (this->legendVec.size()!=this->nCurve) {
        throw length_error("Legends must match the number of data vector pairs");
    }

    prepareLineSpec();

    gpScreen();

}

//void Eggplot::exportfig(const string &filename)
//{
//    this->filenameExport = filename;
//}

//void Eggplot::setupLineSpec(const unsigned   lineIndex,
//                            const LineSpecInput   &lineSpecMatlab,
//                            const map<string, int> &lineType,
//                            const map<string, int> &pointType,
//                            string           &lineSpec,
//                            bool             &flagPointOnly)
//{
//    if (lineSpecMatlab[LineStyle].compare("none")==0) {
//        flagPointOnly = true;
//    }
//    else {
//        flagPointOnly = false;
//    }

//    lineSpec = "";
//    string spec = "";
//    bool flag = false;

//    //* line type
//    try {
//        spec = lineSpecMatlab.at(LineStyle);
//        flag = true;
//    }
//    catch (const out_of_range &e) {
//        ;
//    }

//    if (flag) {
//        try {
//            lineSpec += " lt " + to_string(lineType.at(spec)); // may throw out_of_range
//        }
//        catch (const out_of_range &e) {
//            throw invalid_argument("LineType must be \"-\", \"--\", \":\", \"-.\", or \"none\"");
//        }
//    }

//    //* line width
//    try {
//        lineSpec += " lw " + to_string(stod(lineSpecMatlab.at(LineWidth)));
//    }
//    catch (const out_of_range &e) {
//        ;
//    }
//    catch (const invalid_argument &e) {
//        throw invalid_argument("LineWidth must be numeric");
//    }

//    //* point type
//    try {
//        spec = lineSpecMatlab.at(Marker);
//    }
//    catch (const out_of_range &e) {
//        ;
//    }

//    try {
//        lineSpec += " pt " + to_string(pointType.at(spec)); // may throw out_of_range
//    }
//    catch (const out_of_range &e) {
//        throw invalid_argument("Marker must be one of \"o+*.xsd^v><ph\" or \"none\"");
//    }

//    //* point size
//    try {
//        lineSpec += " ps " + to_string(stod(lineSpecMatlab.at(MarkerSize)));
//    }
//    catch (const out_of_range &e) {
//        ;
//    }
//    catch (const invalid_argument &e) {
//        throw invalid_argument("MarkerSize must be numeric");
//    }

//    //* color
//    flag = false;
//    string colorString;
//    try {
//        colorString = lineSpecMatlab.at(Color);
//        flag = true;
//    }
//    catch (const out_of_range &e) {
//        //* if no specified, use default
//        lineSpec += " lc rgb '" + defaultColor[index] + "'";
//    }
//    if (flag) {
//        //* there is color spec
//        if (colorString.size()==1) {
//            try {
//                lineSpec += " lc rgb '" + colorMapping.at(colorString[0]) + "'";
//            }
//            catch (const out_of_range &e) {
//                throw out_of_range("Color shortcut must be one of \"ymcrgbwk\"");
//            }
//        }
//        else {
//            colorString.erase(remove_if(colorString.begin(), colorString.end(), ::isspace), colorString.end());
//            if (colorString[0]!='[' || colorString[0]=='#') {
//                //* Color string is a string color spec or hex color code
//                lineSpec += " lc rgb '" + colorString + "'";
//            }
//            else {
//                //* Color string is specified as bracked values

//                //* remove brackets
//                if (colorString.size()>2){
//                    colorString = colorString.substr(1, colorString.size());
//                }
//                else {
//                    throw invalid_argument("Color format must be color names, shortcuts, "
//                                           "hex hash codes '#rrggbb', or bracketed triples '[r,g,b]'");
//                }

//                //* count number of commas
//                int nComma = 0;
//                size_t pos = 0;
//                while (true) {
//                    pos = colorString.find_first_of(',', pos);
//                    if (pos==string::npos) {
//                        break;
//                    }
//                    else {
//                        nComma++;
//                    }
//                }
//                if (nComma!=2) {
//                    throw invalid_argument("Brackted color triples must contain exact two commas, '[r,g,b]'");
//                }

//                //* Get rgb respective values
//                stringstream   ss(colorString);
//                string         str;
//                vector<double> rgbValue(3);

//                int index=0;
//                while (getline(ss, str, ',')) {
//                    //* Receive r, g, b code respectively
//                    rgbValue[index++] = stod(str);  // may throw invalid_argument
//                }

//                //* Check range and convert to [0,255] values
//                for (int i=0; i<3; ++i) {
//                    if (rgbValue[i]>255 || rgbValue[i]<0 || (rgbValue[i]>1 && floor(rgbValue[i])!=rgbValue[i])) {
//                        throw invalid_argument("Color triples must be all pure decimals between 0 and 1.0 "
//                                               "or all integers between 0 and 255");
//                    }
//                }

//                //* Check if all values are of the same format
//                bool flagXterm = true;  // default format as xterm [0,255]
//                if ( rgbValue[0]<1 && rgbValue[1]<1 && rgbValue[2]<1 ) {
//                    flagXterm = false;
//                }
//                else if ( floor(rgbValue[0])==rgbValue[0] &&
//                          floor(rgbValue[1])==rgbValue[1] &&
//                          floor(rgbValue[2])==rgbValue[2]    ) {
//                    flagXterm = true;
//                }
//                else {
//                    throw invalid_argument("Color triples must be all pure decimals between 0 and 1.0 "
//                                           "or all integers between 0 and 255");
//                }

//                //* Convert Matlab format to xterm format
//                vector<int> xtermValue(3);
//                if (flagXterm==false) {
//                    xtermValue[0] = floor(rgbValue[0]*256);
//                    xtermValue[1] = floor(rgbValue[1]*256);
//                    xtermValue[2] = floor(rgbValue[2]*256);
//                }

//                //* Convert xterm format to hex code
//                stringstream ssHexcode;
//                ssHexcode << std::hex << "#" << xtermValue[0] << xtermValue[1] << xtermValue[2];
//                string hexcode = ssHexcode.str();

//                lineSpec += " lc rgb '" + hexcode + "'";
//            }
//        }
//    }
//}


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
//    this->lineSpecInput.sort();
//    unsigned nSpec = this->lineSpecInput.back().first;

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

//void Eggplot::prepareDefaultLineStyle(ofstream &fout)
//{
////    this->lineSpec.resize(this->maxLineSpecIndex);
////    unsigned nDefaultColor = defaultColor.size();
////    unsigned maxSpecified  = this->lineSpec.size();

////    for (unsigned i=0; i<maxSpecified; ++i) {
//    for (unsigned i=0; i<nDefaultColor; ++i) {
//        //* lt 1: "-"
//        fout << "set linetype " << to_string(i+1) << " lt 1 lc rgb "
//             <<  defaultColor[i % nDefaultColor] << endl;
//    }
//    fout << "set linetype cycle " << nDefaultColor << endl;
//}

void Eggplot::gpScreen()
{
    //* Generate gnuplot batch file
    string filename = this->filenamePrefix+".gp";
    ofstream fout(filename.c_str());

    gpHeader(fout);

    //* Set terminal and line style

    if (existsAqua) {
        fout << "set terminal aqua dashed" << endl;
        for (unsigned i=0; i<this->lineSpec.size(); ++i) {
            fout << this->lineSpec[i].toStringAqua() << endl;
        }
    }
    else if (existsWxt) {
        fout << "set terminal wxt dashed" << endl;
        for (unsigned i=0; i<this->lineSpec.size(); ++i) {
            fout << this->lineSpec[i].toStringWxtCairoSvg() << endl;
        }
    }
    else {
        fout << "# No supported display terminal found. Line styles may be not accurate." << endl;
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
    //gpDefaultLineStyle();
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

//    if (this->filenameExport.empty()==false){
//        map<string,string> exportType{//{"jpg", "jpeg"},
//                                      {"png", "pngcairo enhanced dashed"},
//                                      //{"tex", "latex"},
//                                      //{"eps", "epslatex standalone color colortext"},
//                                      //{"eps", "postscript eps color colortext dashed"},
//                                      {"eps", "epscairo transparent color enhanced dashed"},
//                                      {"pdf", "pdfcairo transparent color enhanced dashed"},
//                                      {"html", "canvas"},
//                                      {"svg", "svg"}
//                                     };
//        auto itDot = this->filenameExport.find_last_of('.');
//        string ext = this->filenameExport.substr(++itDot);
//        fout << "set terminal push" << endl;
//        fout << "set terminal " << exportType[ext] << endl;
//        fout << "set output '" << this->filenameExport << "'" << endl;
//        fout << "replot" << endl;
//        fout << "set output" << endl;
//        fout << "set terminal pop" << endl;
//    }

    system(("gnuplot "+filename).c_str());

}



}




