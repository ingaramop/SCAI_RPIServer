// @file index.cpp
// @author Pablo Ingaramo
// @date 21 mar 2016
// @brief cgi-bin para el servidor apache2. Obtiene la informacion de los sensores y arma el response RESTfull

#include <fcgi_stdio.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265

class GpsData {
	long timestamp; 
	float positionX  , positionY ; //coordenadas 
	float initialPositionX  , initialPositionY; //coordenadas 
	int altitude, speed;
	double counter;


  public:
	void printData(); 
	void setSpeedPosition(); //consulta GPS y setea posicion y velocidad
	GpsData();
};

GpsData::GpsData(){

	timestamp = 0; //hm/h
	initialPositionY= -31.435348f, initialPositionX = -64.193980f; //coordenadas 
	positionX= initialPositionX, positionY = initialPositionY; //coordenadas 
	counter =0.0;
	altitude = 200;
	speed = 50;
}


void GpsData::printData () {
	
	printf("	<positionX>%f</positionX>", positionX);
	printf("	<positionY>%f</positionY>", positionY);
	printf("	<timestamp>%d</timestamp>", timestamp);
	printf("	<altitude>%d</altitude>", altitude);
	printf("	<speed>%d</speed>", speed);
	return;
}

void GpsData::setSpeedPosition () {
	struct timeval tv;

	gettimeofday(&tv,NULL);

 
	timestamp = (long)(tv.tv_sec * 1000 +(tv.tv_usec/1000)); // milliseconds
	positionX += sin (counter*(PI/180)) * (0.0004f + (rand() % 10)*0.000035f);
	positionY += cos (counter*(PI/180)) * (0.0004f +  (rand() % 10)*0.000035f);
	speed += (rand() % 3)-1;
	altitude += (rand() % 3)-1;

	counter += 3.0;
	return;
}

int main(int argc, char **argv)
{
   // time_t timer;
   // char time_str[25];
   // struct tm* tm_info;

	GpsData trailer;

    while(FCGI_Accept() >= 0)   {

        /* Without this line, you will get 500 error */

		trailer.setSpeedPosition();

        puts("Content-type: application/xml\n");
        puts("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        puts("<Response>");
		trailer.printData();
		puts("</Response>");




    }
    return 0;
}


