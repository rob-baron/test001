CC:=gcc -I.
CCFLAGS:=-I.
CPP:=g++
CPPFLAGS:=-I.
F77:=f77
F77FLAGS:=-shared -fPIC

iri.so: ../../iri2016/iritest.for ../../iri2016/irisub.for ../../iri2016/irifun.for ../../iri2016/iritec.for ../../iri2016/iridreg.for ../../iri2016/igrf.for ../../iri2016/cira.for ../../iri2016/iriflip.for
	$(F77) $(F77FLAGS) -o iri.so ../../iri2016/iritest.for ../../iri2016/irisub.for ../../iri2016/irifun.for ../../iri2016/iritec.for ../../iri2016/iridreg.for ../../iri2016/igrf.for ../../iri2016/cira.for ../../iri2016/iriflip.for

clean:
	rm -rf iri.o
	rm -rf iri.a
