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
        this->lineType = value;
        break;
    case LineWidth:
        try{
           this->lineWidth = stod(value);
        }
        catch (const invalid_argument &e) {
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
        catch (const invalid_argument &e) {
            throw invalid_argument("MarkerSize must be numeric");
        }
        this->pointSize = (this->pointSize<0) ? 0 : this->pointSize;
        break;
    case Color:
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

//        style += " lt " + to_string(lineTypeMapping.at(this->lineType));
        style += " lt " + ss.str();
    }
    catch (const out_of_range &e) {
        throw invalid_argument("LineType must be \"-\", \"--\", \":\", \"-.\", or \"none\"");
    }

    //* linewidth
    stringstream ssLineWidth;
    ssLineWidth << setprecision(3) << this->lineWidth;
    style += " lw " + ssLineWidth.str();

    //* pointtype
    try {
        style += " pt " + to_string( (pointType.empty()) ? lineIndex : pointTypeMapping.at(pointType) );
    }
    catch (const out_of_range &e) {
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
        catch (const out_of_range &e) {
            throw out_of_range("Color shortcut must be one of \"ymcrgbwk\"");
        }
    }
    else {

        //* remove any white space
        color.erase(remove_if(color.begin(), color.end(), ::isspace), color.end());

        if (color[0]!='[' || color[0]=='#') {
            //* color string is a color name or hex color code
            style += " lc rgb '" + color + "'";
        }
        else {
            //* color string is specified as bracked values

            //* remove brackets
            if (color.size()>2){
                color = color.substr(1, color.size());
            }
            else {
                throw invalid_argument("Color format must be color names, shortcuts, "
                                       "hex hash codes '#rrggbb', or bracketed triples '[r,g,b]'");
            }

            //* count number of commas
            int nComma = 0;
            for (auto it=color.begin(); it!=color.end(); ++it) {
                if (*it == ',') {
                    nComma++;
                }
            }

            if (nComma!=2) {
                throw invalid_argument("Brackted color triples must contain exact two commas, '[r,g,b]'");
            }

            //* Get rgb respective values
            stringstream   ss(color);
            string         str;
            vector<double> rgbValue(3);

            int index=0;
            while (getline(ss, str, ',')) {
                //* Receive r, g, b code respectively
                rgbValue[index++] = stod(str);  // may throw invalid_argument
            }

            //* Check range and convert to [0,255] values
            for (int i=0; i<3; ++i) {
                if (rgbValue[i]>255 || rgbValue[i]<0 || (rgbValue[i]>1 && floor(rgbValue[i])!=rgbValue[i])) {
                    throw invalid_argument("Color triples must be all pure decimals between 0 and 1.0 "
                                           "or all integers between 0 and 255");
                }
            }

            //* Check if all values are of the same format
            bool flagXterm = true;  // default format as xterm [0,255]
            if ( rgbValue[0]<1 && rgbValue[1]<1 && rgbValue[2]<1 ) {
                flagXterm = false;
            }
            else if ( floor(rgbValue[0])==rgbValue[0] &&
                      floor(rgbValue[1])==rgbValue[1] &&
                      floor(rgbValue[2])==rgbValue[2]    ) {
                flagXterm = true;
            }
            else {
                throw invalid_argument("Color triples must be all pure decimals between 0 and 1.0 "
                                       "or all integers between 0 and 255");
            }

            //* Convert Matlab format to xterm format
            vector<int> xtermValue(3);
            if (flagXterm==false) {
                xtermValue[0] = floor(rgbValue[0]*256);
                xtermValue[1] = floor(rgbValue[1]*256);
                xtermValue[2] = floor(rgbValue[2]*256);
            }

            //* Convert xterm format to hex code
            stringstream ssHexcode;
            ssHexcode << std::hex << "#" << xtermValue[0] << xtermValue[1] << xtermValue[2];
            string hexcode = ssHexcode.str();

            style += " lc rgb '" + hexcode + "'";
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


//* Static member init
const vector<string> LineSpec::defaultColor {
    "#ff0000",  // red
    "green",
    "blue",
    "magenta",
    "darkturquoise",
    "brown",
    "goldenrod",
    "darkolivegreen",
    "dimgray",
    "purple"
};

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
