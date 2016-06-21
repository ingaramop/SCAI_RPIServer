SENSOR=bmp180

all:
	g++ -Wall -c $(SENSOR).c -o $(SENSOR).o -lm
	g++ -Wall $(SENSOR).o imuQuery.c -l rt -o imuQuery.cgi -lm -lfcgi

clean:
	rm *.o > /dev/null 2>&1 &
