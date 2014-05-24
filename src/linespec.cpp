#include "linespec.h"

#include <exception>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <sstream>

#include <iostream>

using namespace std;

namespace eggp{


LineSpec::LineSpec(unsigned index) :
    lineType("-"),
    lineWidth(1),
    pointType(""),
    pointSize(1)
{
    if (index==0) {
        this->lineIndex = ++(LineSpec::lineCount);
    }
    else {
        this->lineIndex = index;
    }
    this->color = LineSpec::defaultColor[(this->lineIndex-1)%LineSpec::defaultColor.size()];
}

void LineSpec::set(const LineProperty property, const string &value)
{
    switch (property) {
    case LineStyle:
        if (value.empty()){
            throw invalid_argument("LineStyle cannot be empty");
        }
        this->lineType = value;
        break;
    case LineWidth:
        try{
           this->lineWidth = stod(value);
        }
        catch (const invalid_argument &) {
            throw invalid_argument("LineWidth must be numeric");
        }
        this->lineWidth = (this->lineWidth<0) ? 0 : this->lineWidth;
        break;
    case Marker:
        this->pointType = value;
        break;
    case MarkerSize:
        try{
            this->pointSize = stod(value);
        }
        catch (const invalid_argument &) {
            throw invalid_argument("MarkerSize must be numeric");
        }
        this->pointSize = (this->pointSize<0) ? 0 : this->pointSize;
        break;
    case Color:
        if (value.empty()){
            throw invalid_argument("Color spec cannot be empty");
        }
        this->color = value;
        break;
    default:
        throw invalid_argument("Invalid line property");
    }
}

void LineSpec::set(const std::pair<LineProperty, string> &input)
{
    this->set(input.first, input.second);
}

string LineSpec::toStringAqua() const
{
    string style;
    toStringBasic(this->lineTypeMappingAqua, this->pointTypeMappingAqua, style);
    return style;
}

string LineSpec::toStringWxtCairoSvg() const
{
    string style;
    toStringBasic(this->lineTypeMappingOther, this->pointTypeMappingOther, style);
    return style;
}

string LineSpec::toStringHtml() const
{
    string style;
    toStringBasic(this->lineTypeMappingOther, this->pointTypeMappingCanvas, style);
    return style;
}



void LineSpec::toStringBasic(const std::map<string, int> &lineTypeMapping,
                             const std::map<string, int> &pointTypeMapping,
                             std::string &style) const
{
    style = "set style line " + to_string(this->lineIndex);

    //* linetype
    try {
        stringstream ss;
        ss << setprecision(3) << lineTypeMapping.at(this->lineType);
        style += " lt " + ss.str();
    }
    catch (const out_of_range &) {
        throw invalid_argument("LineStyle must be \"-\", \"--\", \":\", \"-.\", or \"none\"");
    }

    //* linewidth
    stringstream ssLineWidth;
    ssLineWidth << setprecision(3) << this->lineWidth;
    style += " lw " + ssLineWidth.str();

    //* pointtype
    try {
        style += " pt " + to_string( (pointType.empty()) ? lineIndex : pointTypeMapping.at(pointType) );
    }
    catch (const out_of_range &) {
        throw invalid_argument("Marker must be one of \"o+*.xsd^v><ph\" or \"none\"");
    }

    //* pointsize
    stringstream ssPointSize;
    ssPointSize << setprecision(3) << this->pointSize;
    style += " ps " + ssPointSize.str();

    //* color
    string color = this->color;
    if (color.size()==1) {
        //* color shortcut
        try {
            style += " lc rgb '" + this->colorShortCutMapping.at(this->color[0]) + "'";
        }
        catch (const out_of_range &) {
            throw out_of_range("Color shortcut must be one of \"ymcrgbwk\"");
        }
    }
    else {

        //* remove any white space
        color.erase(remove_if(color.begin(), color.end(), ::isspace), color.end());

        if (color[0]=='[' || color[0]=='(') {

            bool isDecimal = (color[0]=='(') ? true : false;

            //* Matlab color code or decimal code
            color = color.substr(1, color.size());

            //* count number of commas
            int nComma = 0;
            for (auto it=color.begin(); it!=color.end(); ++it) {
                if (*it == ',') {
                    nComma++;
                }
            }
            if (nComma!=2) {
                throw invalid_argument("Brackted color triples must contain exact two commas, \"[r,g,b]\" or \"(r,g,b)\"");
            }

            //* Get rgb respective values
            const int nRGB = 3;
            vector<int>    rgbValue(nRGB);
            vector<double> frgbValue(nRGB);
            stringstream   ss(color);
            string         str;
            if (isDecimal) {
                //* decimal color code
                int index=0;
                while (getline(ss, str, ',')) {
                    try {
                        rgbValue[index] = stod(str);  // may throw invalid_argument
                    }
                    catch (const invalid_argument &) {
                        throw invalid_argument("Parenthesized color triples must be in integers within 0-255: " + str);
                    }
                    //* check range
                    if (rgbValue[index]>255 || rgbValue[index]<0) {
                        throw invalid_argument("Parenthesized color triples must be in integers within 0-255: " + str);
                    }
                    index++;
                }
            }
            else {
                //* Matlab color code
                int index=0;
                const double epsilon=0.001;  // small value to ensure 1->255
                while (getline(ss, str, ',')) {
                    try {
                        frgbValue[index] = stod(str);  // may throw invalid_argument
                    }
                    catch (const invalid_argument &) {
                        throw invalid_argument("Bracketed color triples must be in decimals within 0.0-1.0: " + str);
                    }
                    //* check range
                    if (rgbValue[index]>1 || rgbValue[index]<0) {
                        throw invalid_argument("Bracketed color triples must be in decimals within 0.0-1.0: " + str);
                    }
                    //* convert to [0,255] values
                    rgbValue[index] = floor(frgbValue[index]*256-epsilon);
                    index++;
                    cout << rgbValue[index-1] << endl;
                }
            }
            //* Convert xterm format to hex code
            stringstream ssHexcode;
            ssHexcode << std::hex << "#"
                         << setw(2) << setfill('0') << rgbValue[0]
                         << setw(2) << setfill('0') << rgbValue[1]
                         << setw(2) << setfill('0') << rgbValue[2];
            string hexcode = ssHexcode.str();

            style += " lc rgb '" + hexcode + "'";
        }
        else {
            //* color string is a color name or hex color code
            style += " lc rgb '" + color + "'";
        }
    }
}


unsigned LineSpec::lineCount = 0;

void LineSpec::resetLineCount()
{
    LineSpec::lineCount = 0;
}

bool LineSpec::isPointOnly() const
{
    return (this->lineType.compare("none")==0)? true : false;
}

unsigned LineSpec::getGridLineType(TerminalType tt)
{
    switch (tt) {
    case TERM_AQUA:
        return LineSpec::lineTypeMappingAqua.at(":");
        break;
    case TERM_WXT:
    case TERM_SVG:
    case TERM_CANVAS:
    case TERM_CAIRO:
        return LineSpec::lineTypeMappingOther.at(":");
        break;
    default:  // TERM_OTHER
        return LineSpec::lineTypeMappingOther.at(":");
    }
}


//* Static member init
const vector<string> LineSpec::defaultColor {
    "#f00032",  // red
    "#227500",  // green
    "#1a3bea",  // blue
    "#e700f0",  // magenta
    "#00beb1",  // cyan
    "#8b4513",  // brown
    "#f0c000",  // yellow
    "#808000",  // olive
    "#505050",  // gray
    "#6b00d2"   // purple
};

std::string LineSpec::gridColor = "#cccccc";

const map<string, int> LineSpec::lineTypeMappingAqua {
    {"-",    1},
    {"--",   2},
    {":",    4},
    {"-.",   5},
    {"none", 0}
};

const map<string, int> LineSpec::lineTypeMappingOther {
    {"-",    1},
    {"--",   2},
    {":",    3},
    {"-.",   4},
    {"none", 0}
};

const map<string, int> LineSpec::pointTypeMappingAqua {
    {"o", 5},
    {"+", 1},
    {"*", 3},
    {".", 5},
    {"x", 2},
    {"s", 4},
    {"square", 4},
    {"d", 5},
    {"diamond", 5},
    {"^", 6},
    {"v", 6},
    {">", 6},
    {"<", 6},
    {"p", 5},
    {"pentagram", 5},
    {"h", 5},
    {"hexagram", 5},
    {"none", 0}
};

const map<string, int> LineSpec::pointTypeMappingCanvas {
    {"o", 6},
    {"+", 1},
    {"*", 3},
    {".", 7},
    {"x", 2},
    {"s", 4},
    {"square", 4},
    {"d", 5},
    {"diamond", 5},
    {"^", 8},
    {"v", 9},
    {">", 8},
    {"<", 9},
    {"p", 4},
    {"pentagram", 4},
    {"h", 8},
    {"hexagram", 8},
    {"none", 0}
};

const map<string, int> LineSpec::pointTypeMappingOther {
    {"o", 6},
    {"+", 1},
    {"*", 3},
    {".", 7},
    {"x", 2},
    {"s", 4},
    {"square", 4},
    {"d", 12},
    {"diamond", 12},
    {"^", 8},
    {"v", 10},
    {">", 9},
    {"<", 11},
    {"p", 5},
    {"pentagram", 5},
    {"h", 13},
    {"hexagram", 13},
    {"none", 0}
};

const map<char, string> LineSpec::colorShortCutMapping {
    {'y', "yellow"},
    {'m', "magenta"},
    {'c', "cyan"},
    {'r', "red"},
    {'g', "green"},
    {'b', "blue"},
    {'w', "white"},
    {'k', "black"}
};




}
