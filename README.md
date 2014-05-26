Eggplot
=======

A C++ library that allows you to plot just as in MATLAB 


Platforms
---------

+ Windows, Linux, OS X
+ Tested on g++, clang++ 
+ C++11 (API simplified a lot by `std::initializer_list`)
+ gnuplot 4.6 and above in `$PATH`


Quick examples
--------------

### 1. Simple curve


![simple-curve](http://github.com/yuchsiao/eggplot/raw/master/img/eggp-test.png)

![simple-curve](http://github.com/yuchsiao/eggplot/raw/master/img/eggp-test.svg)

![simple-curve](http://github.com/yuchsiao/eggplot/raw/master/img/eggp-test.eps)

![simple-curve](http://github.com/yuchsiao/eggplot/raw/master/img/eggp-test.html)



Include the header file `eggplot.h` and assume __x__ and __t__ are `vector<double>` of the same length:

```c++11
eggp::Eggplot curvePlot; 

curvePlot.plot({t,x});
curvePlot.exec();
```

The code above will plot __x__ against __t__ with default labels for both axes.
The function call `.exec()` must be the last command and object initialization `eggp::Eggplot objectName` must be the first. 
The orders of all the other setup and plot commands can be arbitrary.

### 2. Multiple curves with legends

Include the header file `eggplot.h` and assume __x1__, __x2__, and __t__ are `vector<double>` of the same length:

```
eggp::Eggplot curvePlot; 

curvePlot.xlabel("time\\n(sec)");       
curvePlot.ylabel("x_i(t) (volt)");  // supports subs, supers, and breaklines
curvePlot.plot({ t,x1, t,x2 });     // plot two curves by cascading data pairs
curvePlot.legend({"Curve 1", 
                  "Curve 2"});
curvePlot.grid(true);               // turn on grids

curvePlot.exec();
```

Eggplot can plot more than one curve simultaneously on the same plot by cascading data pairs just as in MATLAB. 
Legends can be set up in a similar manner.

Texts can be formatted with subscripts (`"_"`), superscript (`"^"`), and breaklines (`"\\n"`).
Greek letters and other symbols are also possible but with different syntaxes from Latex. 
[See here](http://mathewpeet.org/lists/symbols/) for a list of available symbols.

### 3. Curve customization

The same assumptions as the previous example:

```
eggp::Eggplot curvePlot; 

curvePlot.plot({ t,x1, t,x2 });

using namespace eggp;

curvePlot.linespec(1, {{MarkerSize, "2.5"}, {Marker, "*"}});  // Curve 1
curvePlot.linespce(2, Color, "red");                          // Curve 2

curvePlot.exec();
```

Member function `.linespec()` customizes the line with the specified index. 
Two propertypes are implemented: the first one utilizes `std::initializer_list` and the 2nd one sets up a single property.

Customizable properties  include `LineStyle`, `LineWidth`, `Marker`, `MarkerSize`, and `Color`, each of which is defined within the namespace `eggp`.
A list of options can be referred in the API section.


### 4. File export

Eggplot can export plots to various image types, including `.png`, `.eps`, `.pdf`, `.html`, and `.svg`.

```
eggp::Eggplot curvePlot(SCREEN|PNG|EPS|PDF|HTML|SVG);

curvePlot.print("eggp-export");    // set export file name

curvePlot.plot({ t,x1, t,x2 });
curvePlot.exec();
```

The output files will then be `eggp-export.png`, `eggp-export.eps`, and so on. 
Note that both EPS and PDF modes also support LaTeX-formatted texts.


API
---

### class eggp::Eggplot

**Constructor**

**```Eggplot(unsigned mode=eggp::SCREEN)```**

Init object and set up where to plot. 
The default is `eggp::SCREEN` to plot on screen. 
Other output modes include `eggp::PNG`, `eggp::EPS`, `eggp::PDF`, `eggp::HTML`, and `eggp::SVG` that plot in `.png`, `.eps`, `.pdf`, `.html`, and `.svg` files, respectively.

**Member functions**

*Text Related*

**```void xlabel(const std::string &label)```** sets up x axis label

**```void ylabel(const std::string &label)```** sets up y axis label

**```void title(const std::string &label)```** sets up figure title

**```void legend(std::initializer_list<std::string> legendVec)```** sets up figure legeneds. If the length of `legendVec` is shorter than the number of curves, default legends will be used for those whose legends are not specified.

*Line Property Related*

**```void linespec(unsigned lineIndex, LineSpecInput lineSpec)```** customizes curves with the index `lineIndex`, starting from 1. `LineSpecInput` is a `std::map` that takes different `LineProperty` (`eggp::LineStyle`, `eggp::LineWidth`, `eggp::Marker`, `eggp::MarkerSize`, `eggp::Color`) as the key and a string as the value. In practice, written the curve setups in an initializer list is useful as in Example 3. Color can be specified in five ways: color name, color name shortcut, hex code, decimal code, and rgb values between 0 and 1. For example, for red, "r", "red", "#ff0000", "(255,0,0)", and "[1.0, 0, 0]" are equivalent.

**```void linespec(unsigned lineIndex, LineProperty property, std::string value)```** customizes a single curve property. All options are the same as the previous one.

**```void linespec(unsigned lineIndex, LineProperty property, double value)```** customizes a single curve property with a `double` value. Same as the previous one except for the `double` data type. Only valid for `eggp::LineWidth` and `eggp::MarkerSize`.

**```void grid(bool flag)```** turns on or off grids of the plot

*Output Related*

**```void plot(std::initializer_list<DataVector> il)```** saves data in file `eggp.dat`. The argument must be paired (even-numbered vectors in `il`) such that each pair (the (2N-1)-th and (2N)-th vectors , N=1,2,...) has the same length. This command does not plot but only store data in hard drives. The actual plots and exports happen at function `.exec()`.

**```void print(const std::string &filenameExport)```** sets up export file name, or the default file name `eggp-export` will be used, otherwise. Again, this command does not really print to files but only set up the file name. The actual print and export processes happen at function `.exec()`.
 
**```void exec()```** executes everything. All previous functions set up and store necessary information for plotting and export. This function automatically generates file `eggp.gp` as input for _gnuplot_ and call `gnuplot eggp.gp` as a system call in Windows cmd or bash in Linux or OS X). The function must be called as the last command in order to generate plots.


Future features
---------------

+ xtic and ytic setup
+ axis range customization
+ Other types of plots are considered also in the future.





