// @file index.cpp
// @author Pablo Ingaramo
// @date 21 mar 2016
// @brief cgi-bin para el servidor apache2. Obtiene la informacion de los sensores y arma el response RESTfull

#include <fcgi_stdio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


class TrailerData {
	int tipperInclination; //degrees
	int sideInclination; //degrees
	int speed; //hm/h
	int positionX , positionY; //coordenadas 
	int compass; //degrees
	int temperature; //celsius
	int altitude;
  public:
	void printData(); 
	void setSensorValues();//consulta y setea la informacion del sensor (inclinacion, orientacion, temperatura)
	void setSpeedPosition(); //consulta GPS y setea posicion y velocidad
	void setAltitude(); //reads altitude from sensor
	TrailerData();
};

TrailerData::TrailerData(){
	tipperInclination = 0; //degrees
	sideInclination = 0; //degrees
	speed = 0; //hm/h
	positionX = 0, positionY = 0; //coordenadas 
	compass = 0; //degrees
	temperature = 0; //celsius
	altitude = 200;
}


void TrailerData::printData () {
	printf("	<altitude>%d</altitude>", altitude);
	printf("	<tipperInclination>%d</tipperInclination>", tipperInclination);
	printf("	<sideInclination>%d</sideInclination>", sideInclination);
	printf("	<speed>%d</speed>", speed);
	printf("	<positionX>%d</positionX>", positionX);
	printf("	<positionY>%d</positionY>", positionY);
	printf("	<compass>%d</compass>", compass);
	printf("	<temperature>%d</temperature>", temperature);
	return;
}
void TrailerData::setSensorValues () {
    tipperInclination += (rand() % 3)-1;	
	sideInclination += (rand() % 2)-1;	
	compass += ((rand() % 3)-1);
	temperature += (rand() % 3)-1;
	altitude += (rand() % 3)-1;	
	return;
}
void TrailerData::setSpeedPosition () {
	speed += (rand() % 3)-1;
	positionX += (rand() % 3)-1;
	positionY += (rand() % 3)-1;
	return;
}

int main(int argc, char **argv)
{
   // time_t timer;
   // char time_str[25];
   // struct tm* tm_info;

	TrailerData trailer;

    while(FCGI_Accept() >= 0)   {

        /* Without this line, you will get 500 error */

		trailer.setSpeedPosition();
		trailer.setSensorValues();

        puts("Content-type: application/xml\n");
        puts("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        puts("<Response>");
		trailer.printData();
		puts("</Response>");




    }
    return 0;
}


