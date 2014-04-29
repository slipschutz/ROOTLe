CXX=$(shell root-config --cxx)
CFLAGS=-c -g -Wall $(shell root-config --cflags) -I./src -I./include -I../Headers -O3
LDLIBS=$(shell root-config --glibs)
LDFLAGS=$(shell root-config --ldflags)

#SOURCES=$(shell ls ./src/*.cc)
#OBJECTS=$(SOURCES:.cc=.o) 
#HEADERS=$(shell ls ./include/*h*)

EXE= TimmingAnalyzer
EXECUTABLE= $(EXE)
MAIN=$(addsuffix .C,$(EXE))
MAINO=./$(addsuffix .o,$(EXE))
ROOTCINT=rootcint

######### .so libraries
LENDALIBS=-lLendaEvent -lDDASEvent -lSettings -lFilter -lLendaPacker


.PHONY: clean get put all test sclean

all: $(EXECUTABLE) 

$(EXECUTABLE) : $(OBJECTS) $(MAINO)
	@echo "Building target" $@ "..." 
	@echo $(shell pwd)
	$(CXX) -O3 $(LDFLAGS) -o $@ $(OBJECTS) $(MAINO) $(LDLIBS) $(LENDALIBS) -Wl,-rpath,$(shell pwd)/../Libs -L../Libs
	@echo
	@echo "Build succeed"


.cc.o:
	@echo "Compiling" $< "..."
	@$(CXX) $(CFLAGS) $< -o $@ 

$(MAINO): $(MAIN) $(HEADERS)
	@echo "Compiling" $< "..."
	@$(CXX) $(CFLAGS) $< -o $@  

%.hh: 
	@

clean:
	-rm -f ./$(EXECUTABLE)
	-rm -f ./$(MAINO)

test:
	@echo $(EVENTLIBPATH)
	@echo $(HEADERS)
sclean:
	-rm  ./$(TEMP)
	-rm  ./$(TEMP2)
	-rm  ./$(OBJECTS)
	-rm  ./$(EXECUTABLE)
	-rm  ./$(MAINO)

