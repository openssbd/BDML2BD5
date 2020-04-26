root := $(XSD_ROOT)/examples

include $(root)/build/cxx/rules.make
include $(root)/build/xsd/tree-rules.make

override LIBS += -lhdf5 -lhdf5_cpp

# Build.
#
bdml2bd5: bdml2bd5.o bdml0.18.o

bdml0.18.o: bdml0.18.cxx bdml0.18.hxx
bdml2bd5.o: bdml2bd5.cxx bdml0.18.hxx

bdml0.18.cxx bdml0.18.hxx: bdml0.18.xsd


# Clean.
#
clean:
	rm -f bdml0.18.o bdml0.18.?xx bdml2bd5.o bdml2bd5
