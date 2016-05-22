#Makefile

all: sensor_data system_info

sensor_data: webserver/cgi-bin/sensor_data.o
	g++ webserver/cgi-bin/sensor_data.cpp -lfcgi -o webserver/cgi-bin/sensor_data.fcgi
system_info: webserver/cgi-bin/system_info.o
	gcc webserver/cgi-bin/system_info.c -o webserver/cgi-bin/system_info.cgi



