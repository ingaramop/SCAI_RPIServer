#Makefile

all: sensor_data gps_data system_info

sensor_data: webserver/cgi-bin/imu_query_mock_src/imu_query_mock.o
	g++ webserver/cgi-bin/imu_query_mock_src/imu_query_mock.cpp -lfcgi -o webserver/cgi-bin/imuQueryMock.fcgi
gps_data: webserver/cgi-bin/gps_query_mock_src/gps_query_mock.o
	g++ webserver/cgi-bin/gps_query_mock_src/gps_query_mock.cpp -lfcgi -o webserver/cgi-bin/gpsQueryMock.fcgi
system_info: webserver/cgi-bin/system_info_src/system_info.o
	gcc webserver/cgi-bin/system_info_src/system_info.c -o webserver/cgi-bin/systemInfo.cgi



