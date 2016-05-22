// @file index.cpp
// @author Pablo Ingaramo
// @date 21 mar 2016
// @brief cgi-bin para el servidor apache2. Obtiene la informacion de los sensores y arma el response RESTfull

#include <fcgi_stdio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


class TrailerData {
	struct tm *dateTime;
	int inclination; //degrees
	int speed; //hm/h
	int positionX, positionY; //coordenadas 
	int orientation; //degrees
	int temperature; //celsius
  public:
    void setDateTime (); //consulta y setea el horario del sistema
	void printData(); 
	void setSensorValues();//consulta y setea la informacion del sensor (inclinacion, orientacion, temperatura)
	void setSpeedPosition(); //consulta GPS y setea posicion y velocidad
};

void TrailerData::setDateTime () {
    time_t timer;
    time(&timer);
    dateTime = localtime(&timer);
    
	return;
}

void TrailerData::printData () {
    char date_str[50];	
	char time_str[60];
	strftime(date_str, sizeof(time_str), "<year>%Y</year><month>%m</month><day>%d</day>", dateTime); //armo string con data de la fecha
	strftime(time_str, sizeof(time_str), "<hsour>%H</hour><minute>%M</minute><second>%S</second>", dateTime);//armo string con data de la hora
	printf("	<date>");
	printf("		%s", date_str);
	printf("	</date>");
	printf("	<time>");
	printf("		%s", time_str);
	printf("	</time>");
	printf("	<inclination>%d</inclination>", inclination);
	printf("	<speed>%d</speed>", speed);
	printf("	<position>");
	printf("		<x>%d</x><y>%d</y>", positionX, positionY);
	printf("	</position>");
	printf("	<orientation>%d</orientation>", orientation);
	printf("	<temperature>%d</temperature>", temperature);
	return;
}
void TrailerData::setSensorValues () {
    inclination = rand() % 90;	
	orientation = rand() % 360;
	temperature = (rand() % 50) - 10;

	return;
}
void TrailerData::setSpeedPosition () {
	speed = rand() % 120;
	positionX = rand() % 1000000;
	positionY = rand() % 1000000;
	return;
}

int main(int argc, char **argv)
{
   // time_t timer;
   // char time_str[25];
   // struct tm* tm_info;

	TrailerData trailer;

    while(FCGI_Accept() >= 0)   {
        //time(&timer);
        //tm_info = localtime(&timer);
        //strftime(time_str, sizeof(time_str), "%Y/%m/%d %H:%M:%S", tm_info);

        /* Without this line, you will get 500 error */

  		trailer.setDateTime();
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


