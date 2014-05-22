#ifndef LINESPEC_H
#define LINESPEC_H

#include <string>
#include <map>
#include <vector>
#include <utility>

#include "common.h"

namespace eggp{

enum TerminalType {TERM_AQUA, TERM_WXT, TERM_CAIRO, TERM_SVG, TERM_CANVAS, TERM_OTHER};

class LineSpec {
public:
    LineSpec(unsigned index=0);

    void set(const LineProperty property, const std::string &value);
    void set(const std::pair<LineProperty, std::string> &input);

    std::string toStringAqua() const;
    std::string toStringWxtCairoSvg() const;
    std::string toStringHtml() const;

    bool isPointOnly() const;
    static unsigned getGridLineType(TerminalType tt);
    static std::string gridColor;

    //* static function
    static void resetLineCount();

private:
    unsigned    lineIndex;
    std::string lineType;
    double      lineWidth;
    std::string pointType;
    double      pointSize;
    std::string color;

    void toStringBasic(const std::map<std::string, int> &lineTypeMapping,
                       const std::map<std::string, int> &pointTypeMapping,
                       std::string                      &style) const;

    //* default color
    static const std::vector<std::string> defaultColor;

    //* line type mapping
    static const std::map<std::string, int> lineTypeMappingAqua;
    static const std::map<std::string, int> lineTypeMappingOther;

    //* point type mapping
    static const std::map<std::string, int> pointTypeMappingAqua;
    static const std::map<std::string, int> pointTypeMappingCanvas;
    static const std::map<std::string, int> pointTypeMappingOther;

    //* color shortcut
    static const std::map<char, std::string> colorShortCutMapping;

    //* static index
    static unsigned lineCount;

};

}

#endif // LINESPEC_H
