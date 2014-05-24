CXX        = g++
FLAG       = -Wall -I$(INCLUDE) -O2 -std=c++11
SRC        = src
OBJ        = tmp
BIN        = bin
INCLUDE    = include

EGGPLOT_OBJ = \
	$(OBJ)/linespec.o \
	$(OBJ)/eggplot.o \
	$(OBJ)/main.o \

all: eggplot

eggplot: $(EGGPLOT_OBJ)
	$(CXX) -o $(BIN)/$@ $^ 

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(FLAG) -c $< -o $@

.phony: clean
clean:
	rm -rf $(OBJ)/*  
