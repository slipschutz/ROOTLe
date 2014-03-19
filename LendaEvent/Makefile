LENDAHEAD=$(shell echo $$LENDAHEAD)
CXX=$(shell root-config --cxx)
CFLAGS=-c -g -Wall $(shell root-config --cflags)
INCLUDES= -I./include -I$(LENDAHEAD)/LendaCommonInclude
LDLIBS=$(shell root-config --glibs)
LDFLAGS=$(shell root-config --ldflags)
#SOURCES=./src/SL_Event.cc ./src/FileManager.cc ./src/Filter.cc

EXTRASOURCE=Correctable.cc CorrectionInfo.cc
EXTRAOBJS=$(patsubst %.cc,%.o,$(EXTRASOURCE))
EXTRAHEADS=$(patsubst %.cc,%.hh,$(EXTRASOURCE))

ROOTCINT=rootcint

NAME=LendaEvent



temp= $(addprefix lib,$(NAME))
#temp2=$(addsuffix $(LDFLAGS),$(temp))
LIBRARY= $(addsuffix .so,$(temp))
DICTNAME=$(addsuffix Dictionary,$(NAME))
DICTOBJ=$(addsuffix .o,$(DICTNAME))
OBJECT=$(addsuffix .o, $(NAME))
HEAD=$(addsuffic .hh,$(NAME))
.PHONY: clean get put all test sclean

all: $(LIBRARY) 

$(LIBRARY) : $(OBJECT) $(EXTRAOBJS) $(DICTOBJ) $(EXTRAHEADS) $(HEAD) 
	@echo "Building Library"
	@$(CXX) `root-config --cflags` -fPIC -shared -o $@ $^
	@echo "Build succeed"

%Dictionary.o : %.hh $(EXTRAHEADS) %LinkDef.h
	@echo "Generating Dictionary"
	@$(ROOTCINT) -f $(patsubst %.o,%.cc,$@) -c $(INCLUDES) $^;
	@echo "Compiling Dicionary"
	@$(CXX) -p -fPIC $(CFLAGS) $(INCLUDES) -c $(patsubst %.o,%.cc,$@)

%Parsed.o : %.cc %.hh
	@echo "Making parsed file for" $< "..."
	@./Parse $(patsubst %.cc,%.hh,$<)
	@echo "Compiling mapped file" $@
	@$(CXX) $(CFLAGS) -fPIC $(patsubst %.o,%.cc,$@) -o $@ 

%.o : %.cc
	@echo "Compiling" $< "..."
	@$(CXX) $(CFLAGS) $(INCLUDES) -fPIC $< -o $@ 

test:
	@echo $(LIBRARY) : $(DICTOBJ) $(OBJECTS)
	echo $(EXTRAOBJS)
	@echo $(INCLUDES)



clean:
	-rm -f $(DICTOBJ) $(OBJECT) $(EXTRAOBJS)
	-rm -f *Dictionary.h
	-rm -f *Dictionary.cc
	-rm -f $(LIBRARY)
	-rm -f *Parsed.cc
	-rm -f *Parsed.o
sclean:
	-rm *~
	make clean