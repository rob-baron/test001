
CC:=gcc
CCFLAGS:=-I. -Wall -Isrc
CPP:=g++
CPPFLAGS:=-I. -Wall -Isrc

gis_radar.o: src/gis_radar.c src/gis_radar.h
	$(CC) $(CCFLAGS)  -c  src/gis_radar.c -o gis_radar.o

test_p5.o: test/test_gis_radar.cpp src/gis_radar.h
	$(CPP) $(CCFLAGS)  -c test/test_gis_radar.cpp -o test_p5.o

test_p5: gis_radar.o test_p5.o
	$(CPP) $(CCFLAGS) gis_radar.o test_p5.o -lm -lgtest -lgtest_main -o test_p5

check: test_p5
	./test_p5

clean:
	rm gis_radar.o
	rm test_p5.o
	rm test_p5
