SENSOR=bmp180

all:
	gcc -Wall -c $(SENSOR).c -o $(SENSOR).o -lm
	gcc -Wall $(SENSOR).o gyro_accelerometer_tutorial03.c -l rt -o test.o -lm

clean:
	rm *.o > /dev/null 2>&1 &
