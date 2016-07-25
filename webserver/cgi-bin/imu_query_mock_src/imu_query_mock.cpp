// @file index.cpp
// @author Pablo Ingaramo
// @date 21 mar 2016
// @brief cgi-bin para el servidor apache2. Obtiene la informacion de los sensores y arma el response RESTfull

#include <fcgi_stdio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


class ImuData {
	int tipperInclination; //degrees
	int sideInclination; //degrees
	int compass; //degrees
	int temperature; //celsius
	int altitude;
	int pressure;
  public:
	void printData(); 
	void setSensorValues();//consulta y setea la informacion del sensor (inclinacion, orientacion, temperatura)
	void setAltitude(); //reads altitude from sensor
	ImuData();
};

ImuData::ImuData(){
	tipperInclination = 0; //degrees
	sideInclination = 0; //degrees
	compass = 0; //degrees
	temperature = 0; //celsius
	altitude = 200;
	pressure = 1018;
}


void ImuData::printData () {
	printf("	<tipperInclination>%d</tipperInclination>", tipperInclination);
	printf("	<sideInclination>%d</sideInclination>", sideInclination);
	printf("	<compass>%d</compass>", compass);
	printf("	<temperature>%d</temperature>", temperature);
	printf("	<pressure>%d</pressure>", pressure);
	return;
}
void ImuData::setSensorValues () {
	
    tipperInclination += (rand() % 9)-4;	
	if (tipperInclination < 0) tipperInclination =5;
	if (tipperInclination > 35) tipperInclination =30;
	sideInclination += (rand() % 9)-4;	
	if (sideInclination < -30) sideInclination =-25;
	if (sideInclination > 30) sideInclination =25;	
	compass += ((rand() % 30)-15);
	temperature += (rand() % 3)-1;
	altitude += (rand() % 3)-1;	
	//pressure += (rand() % 3)-1;
	return;
}


int main(int argc, char **argv)
{
   // time_t timer;
   // char time_str[25];
   // struct tm* tm_info;

	ImuData trailer;

    while(FCGI_Accept() >= 0)   {

        /* Without this line, you will get 500 error */

		trailer.setSensorValues();

        puts("Content-type: application/xml\n");
        puts("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        puts("<Response>");
		trailer.printData();
		puts("</Response>");




    }
    return 0;
}


