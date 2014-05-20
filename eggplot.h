#ifndef EGGPLOT_H
#define EGGPLOT_H

#include <vector>
#include <list>
#include <string>
#include <map>
#include <utility>
#include <initializer_list>
#include <fstream>

#include "common.h"
#include "linespec.h"

/*
 * 1. Markers are mostly the same (up to pt 13) except for terminal aqua.
 * 2. Only pngcairo and postscript export types support dashed lines.
 *    (jpeg, svg do not)
 * 3. Implement export for .png (pngcairo), .eps (postscript eps),
 *    .svg (no dashed), and monitor display (wxt or aqua)
 *
 * pngcairo: best
 * postscript: best
 * aqua: different markers no dashed
 * svg: no dashed
 */


namespace eggp{

const std::string version = "0.1.0";

typedef std::vector<double> DataVector;

class Eggplot
{
public:
    Eggplot(unsigned mode=SCREEN);
    void xlabel(const std::string &label);
    void ylabel(const std::string &label);
    void title(const std::string &label);
    void legend(std::initializer_list<std::string> legendVec);
    void linespec(unsigned lineIndex, LineSpecInput lineSpec);
    void plot(std::initializer_list<DataVector> il);
    void show();

private:
    std::string filenamePrefix;
    std::string labelX;
    std::string labelY;
    std::string labelTitle;
    std::vector<std::string>        legendVec;
    std::list<std::pair<unsigned, LineSpecInput>> lineSpecInput;
    std::vector<LineSpec>           lineSpec;
    std::list<std::string>          lineSpecAqua;
    std::list<std::string>          lineSpecCanvas;
    std::list<std::string>          lineSpecOther;
    unsigned nCurve;
    std::string filenameExport;
    bool flagScreen;
    bool flagHtml;
    bool flagPng;
    bool flagEps;
    bool flagPdf;
    bool flagSvg;

    bool existsAqua;
    bool existsWxt;
    bool existsCanvas;
    bool existsCairo;
    bool existsSvg;

//    //* line type mapping
//    std::map<std::string, int> lineTypeMappingAqua;
//    std::map<std::string, int> lineTypeMappingOther;

//    //* point type mapping
//    std::map<std::string, int> pointTypeMappingAqua;
//    std::map<std::string, int> pointTypeMappingCanvas;
//    std::map<std::string, int> pointTypeMappingOther;

//    //* color mapping
//    std::map<char, std::string> colorMapping;


//    void generateBatchFile();
//    void generateBatchFileScreen();
//    void setupLineSpec( const unsigned lineIndex,
//                        const LineSpecInput &lineSpecMatlab,
//                        const std::map<std::string, int> &lineType,
//                        const std::map<std::string, int> &pointType,
//                        std::string &lineSpecGnuplot,
//                        bool &flagPointOnly);


    bool existsTerminal(std::string);
    void prepareLineSpec();

    //* gp setup
//    void gpDefaultLineStyle(std::ofstream &fout);

    //* plot curve .gp files
    void gpScreen();
    void gpPng();
    void gpEps();
    void gpPdf();
    void gpHtml();
    void gpSvg();
    void gpHeader(std::ofstream &fout);
    void gpCurve(std::ofstream &fout, const std::string &filename);
};

}

#endif // EGGPLOT_H
