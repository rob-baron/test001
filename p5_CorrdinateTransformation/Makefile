
CC:=gcc
CCFLAGS:=-I. -Wall -Isrc
CPP:=g++
CPPFLAGS:=-I. -Wall -Isrc -std=c++14

gis_radar.o: src/gis_radar.c src/gis_radar.h
	$(CC) $(CCFLAGS)  -c  src/gis_radar.c -o gis_radar.o

test_basic.o: test/test_basic.cpp src/gis_radar.h
	$(CPP) $(CCFLAGS)  -c test/test_basic.cpp -o test_basic.o

test_gis_radar.o: test/test_gis_radar.cpp src/gis_radar.h
	$(CPP) $(CCFLAGS)  -c test/test_gis_radar.cpp -o test_gis_radar.o

test_gis_radar2.o: test/test_gis_radar2.cpp src/gis_radar.h
	$(CPP) $(CCFLAGS)  -c test/test_gis_radar2.cpp -o test_gis_radar2.o

test_overall.o: test/test_overall.cpp src/gis_radar.h
	$(CPP) $(CCFLAGS)  -c test/test_overall.cpp -o test_overall.o

test_radar_gis.o: test/test_radar_gis.cpp src/gis_radar.h
	$(CPP) $(CCFLAGS)  -c test/test_radar_gis.cpp -o test_radar_gis.o

test_basic: gis_radar.o test_basic.o
	$(CPP) $(CCFLAGS) gis_radar.o test_basic.o -lm -lgtest -lgtest_main -o test_basic

test_overall: gis_radar.o test_overall.o
	$(CPP) $(CCFLAGS) gis_radar.o test_overall.o -lm -lgtest -lgtest_main -o test_overall

test_gis_radar: gis_radar.o test_gis_radar.o
	$(CPP) $(CCFLAGS) gis_radar.o test_gis_radar.o -lm -lgtest -lgtest_main -o test_gis_radar

test_gis_radar2: gis_radar.o test_gis_radar2.o
	$(CPP) $(CCFLAGS) gis_radar.o test_gis_radar2.o -lm -lgtest -lgtest_main -o test_gis_radar2

test_radar_gis: gis_radar.o test_radar_gis.o
	$(CPP) $(CCFLAGS) gis_radar.o test_radar_gis.o -lm -lgtest -lgtest_main -o test_radar_gis

check: test_basic test_overall test_gis_radar test_radar_gis test_gis_radar2
	./test_gis_radar2

#	./test_basic
#	./test_gis_radar
#	./test_overall
#	./test_radar_gis

clean:
	rm gis_radar.o
	rm test_basic.o
	rm test_gis_radar.o
	rm test_gis_radar.o
	rm test_radar_gis.o
	rm test_overall.o
	rm test_basic
	rm test_gis_radar
	rm test_gis_radar2
	rm test_overall
	rm test_radar_gis
