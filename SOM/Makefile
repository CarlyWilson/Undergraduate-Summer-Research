#Generic Makefile .
#Each doublet of files (X.cc, X.hh) is compiled together into a object file
#and all object files are linked into a library (.so) in the current directory.
#A class should be defined by the 2 files (.hh, .cc). If you have 2 classes, then there
#there will be 4 files...  
#All .cc files will be compiled to .o and linked into a .so file, regardless
#of whether they define classes.
#Any .cc file that is found in a subdirectory called main/ will be assumed
#to contain a main() function. Each of these is linked to the .so file to 
#create an executable of the same name.
#After a make clean, make has to be run twice to make the library.
#R. Martin

SrcSuf = cc #hardcoded in some spots... 
ObjSuf = o
ExeSuf = 
CXX = g++
LD = g++
CXXFLAGS = -O -fPIC -O3 -funroll-loops
LDFLAGS  = -O -fPIC $(shell root-config --ldflags) -lrt
SOFLAGS  = -shared

ROOTLIB= $(shell root-config --libs)
ROOTINCLUDE = -I$(ROOTSYS)/include

CLHEPINCLUDE = -I$(CLHEP_INCLUDE_DIR)
CLHEPLIB = -L$(CLHEP_LIB_DIR) -lCLHEP

MGDOLIB= -L$(MGDODIR)/lib -lMGDOBase -lMGDOMajorana -lMGDORoot -lMGDOTransforms $(CLHEPLIB)
MGDOINCLUDE=-I$(MGDODIR)/Base -I$(MGDODIR)/Majorana -I$(MGDODIR)/Transforms -I$(MGDODIR)/Root $(CLHEPINCLUDE)

ALLINC= $(ROOTINCLUDE) $(MGDOINCLUDE)
ALLLIB= $(ROOTLIB) -lfftw3 -lm -lgmpxx -lgmp $(MGDOLIB)

EXESRC := $(wildcard main/*.$(SrcSuf))
EXEOBJ := $(EXESRC:.cc=.o)
EXE :=  $(patsubst main/%.$(ObjSuf), %$(ExeSuf), $(wildcard  main/*.$(ObjSuf)))
#$(EXESRC:.cc= )


SRC := $(wildcard *.cc)
OBJ := $(SRC:.cc=.o)



#This is the name of the library that will be built:
LIBRARY = libchangeme.so

all : $(DICT) $(OBJ) $(EXEOBJ) dynamiclib $(EXE)  


%.$(ObjSuf): %.$(SrcSuf)
	$(CXX) -g3 -g -c $(ALLINC) $(CXXFLAGS) $< -o $@ -Wno-deprecated

%$(ExeSuf): main/%.$(ObjSuf)
	@echo linking executable
	$(LD) $(LDFLAGS) $< -o $@ $(ALLLIB) $(HOME)/usr/lib/$(LIBRARY) 
	cp $@ $(HOME)/usr/bin/.

dynamiclib: $(OBJ) $(HEADERFILES) 
	@echo "Generating library $(LIBRARY)..."
	$(LD) $(SOFLAGS) $(LDFLAGS) $(OBJ) -o $(LIBRARY)
	cp $(LIBRARY) $(HOME)/usr/lib/.

.PHONY : clean

clean:
	rm $(EXEOBJ) $(OBJ) $(EXE) $(LIBRARY)
