INCDIR = ../include
LIBDIR = ../lib

ODIR = ../build

CXX = g++
CXXFLAGS = -fPIC -Wall `pkg-config --cflags opencv` -I$(INCDIR)
LDFLAGS = -L$(LIBDIR)
LDLIBS = -lmysqlclient `pkg-config --libs opencv`

PROG = imgen 
LIB = libimgen.so

MAINOBJ = $(ODIR)/main.o
SQLOBJ = $(ODIR)/MySQL_API.o
LIBOBJS = $(ODIR)/image_generator.o $(ODIR)/utils.o

all: $(PROG) $(LIB)

$(PROG):$(MAINOBJ) $(LIBOBJS) $(SQLOBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(LIB):$(LIBOBJS) $(SQLOBJ)
	$(CXX) $^ -o $@ -shared

$(ODIR)/%.o:$(addprefix ./, %.cpp)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY:clean

clean:
	rm $(ODIR)/*.o imgen libimgen.so
