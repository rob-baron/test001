# test001

## Introduction

This repository is organized around 5 basic problems

## base container builds
To build the container the c/c++ and fortran code is build in and deployed in run the following:
```
    cd test001
    Docker build -f Dockerfile.c-fortran-dev -t c-fortran-dev .
```

## p1_IRI_EDP
To build the iri-edp container:
```
docker build -f Dockerfile.iri-edp -t iri-edp .
```

To run the docker container interactively:
```
    Docker run --rm -it --entrypoint bash -v `pwd`:/App/test001/p1_IRI_EDP  iri-edp
```

## p2_DataCollection


## p3_C_Optmization
To build the p3-c-opt container run:
```
    cd p3_C_Optimization
    Docker build -f Dockerfile -t p3-c-opt .
```

To run the docker container iteratively:
```
    Docker run --rm -it --entrypoint bash -v `pwd`:/App/test001/p3_C_Optimization  p3-c-opt
```

## P4_Javascript

## p5_CoordinateTransformation
