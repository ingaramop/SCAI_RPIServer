// @file index.cpp
// @author Pablo Ingaramo
// @date 21 mar 2016
// @brief cgi-bin para el servidor apache2. Obtiene la informacion de los sensores y arma el response RESTfull

#include <fcgi_stdio.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>


class GpsData {
	long timestamp; 
	float positionX  , positionY ; //coordenadas 


  public:
	void printData(); 
	void setSpeedPosition(); //consulta GPS y setea posicion y velocidad
	GpsData();
};

GpsData::GpsData(){

	timestamp = 0; //hm/h
	positionX= -31.435348f, positionY = -64.193980f; //coordenadas 
}


void GpsData::printData () {
	
	printf("	<positionX>%f</positionX>", positionX);
	printf("	<positionY>%f</positionY>", positionY);
	printf("	<timestamp>%d</timestamp>", timestamp);
	return;
}

void GpsData::setSpeedPosition () {
	struct timeval tv;

	gettimeofday(&tv,NULL);
 
	timestamp = (long)(tv.tv_sec * 1000 +(tv.tv_usec/1000)); // milliseconds
	positionX += (rand() % 10)*0.00005f-0.00025;
	positionY += (rand() % 10)*0.00005f-0.00025;
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


