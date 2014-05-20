#ifndef COMMON_H
#define COMMON_H

#include<map>
#include<string>

namespace eggp{

enum LineProperty {LineStyle, LineWidth, Marker, MarkerSize, Color};
typedef std::map<LineProperty, std::string> LineSpecInput;

enum Mode         {SCREEN=1, PNG=2, EPS=4, PDF=8, HTML=16, SVG=32};


}

#endif // COMMON_H
