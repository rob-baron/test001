# IRI_EDP
## overview

For this assignment, I created a docker container that downloads and builds the iri object that can be statically linked.

I found it convienent that gfortran can be run with the command line of f77.  I had also used the command line of gfortran in the past.

## Building the iri.so file

This was straight forward and done with 2 different methods.  The first was in the docker file and the second was done to complete the requirement of buiding with the makefile.

## Calling from C

This isn't as trivial as it should be as I don't know of a tool that will convert fortran function definitions to C declarations.

From the Fortran code, I am assuming that we need to call the function iri_sub.

``` F77
SUBROUTINE IRI_SUB(JF,JMAG,ALATI,ALONG,IYYYY,MMDD,DHOUR,
     &    HEIBEG,HEIEND,HEISTP,OUTF,OARR)
C-----------------------------------------------------------------
C
C INPUT:  JF(1:50)      true/false switches for several options
C         JMAG          =0 geographic   = 1 geomagnetic coordinates
C         ALATI,ALONG   LATITUDE NORTH AND LONGITUDE EAST IN DEGREES
C         IYYYY         Year as YYYY, e.g. 1985
C         MMDD (-DDD)   DATE (OR DAY OF YEAR AS A NEGATIVE NUMBER)
C         DHOUR         LOCAL TIME (OR UNIVERSAL TIME + 25) IN DECIMAL
C                          HOURS
C         HEIBEG,       HEIGHT RANGE IN KM; maximal 100 heights, i.e.
C          HEIEND,HEISTP        int((heiend-heibeg)/heistp)+1.le.100
```

The first stage is to determine how fortran mangles the names.  The following command was used to get the name magnaling from the object file:
```
    # nm iri | grep -i IRI_SUB
    0000000000008e78 T iri_sub_
```
so we need to call iri_sub_.  The next step is to determin the C parameter types for the fortran inputs.

## Some simple examples first

Start with the simplest fortran call:

``` F77
C*********************************************************************72
C
C  A subroutine with 0 parameters
C    declaration in C - void t001_();
C
      Subroutine t001()
      implicit none

      write ( *, '(a)' ) '  hello, world!'

      return
      end
```
Some C code
``` C
/* example to call fortran */
void t001_();

void main()
{
    t001_();
}
```
Build and run with the following:
```
f77 -shared -fPIC -ofortranSub.so fortranSub.for
# gcc CallingFortran.c /App/test001/p1_IRI_EDP/src/example/FortranSub.so
# ./a.out
  hello, world!

```
