#include <gps.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <fcgi_stdio.h>

int main() {
int rc;
int altitude_aux;


struct timeval tv;

struct gps_data_t gps_data;
if ((rc = gps_open("localhost", "2947", &gps_data)) == -1) {
    //printf("code: %d, reason: %s\n", rc, gps_errstr(rc));
    return EXIT_FAILURE;
}
gps_stream(&gps_data, WATCH_ENABLE | WATCH_JSON, NULL);

while(FCGI_Accept() >= 0) {
puts("Content-type: application/xml\n");
puts("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
puts("<Response>");
    /* wait for 2 seconds to receive data */
    if (gps_waiting (&gps_data, 2000000)) {
        /* read data */
        if ((rc = gps_read(&gps_data)) == -1) {
            printf("error occured reading gps data. code: %d, reason: %s\n", rc, gps_errstr(rc));
        } else {
            /* Display data from the GPS receiver. */
            if ((gps_data.status == STATUS_FIX) && 
                (gps_data.fix.mode == MODE_2D || gps_data.fix.mode == MODE_3D) &&
                !isnan(gps_data.fix.latitude) && 
                !isnan(gps_data.fix.longitude)) {
                    gettimeofday(&tv, NULL);

				
				if (gps_data.fix.mode == MODE_2D) altitude_aux = -9999;//if two satellites reachable, make altitude invalid instad of zero
				else altitude_aux = (int) (gps_data.fix.altitude - 0.5f);	
				printf("	<positionX>%f</positionX>", gps_data.fix.longitude);
				printf("	<positionY>%f</positionY>", gps_data.fix.latitude);
				printf("	<timestamp>%ld</timestamp>", tv.tv_sec);
				printf("	<altitude>%d</altitude>", altitude_aux);
				printf("	<speed>%d</speed>", (int) (gps_data.fix.speed - 0.5f));
				//printf("	<satellites>%d</satellites>", gps_data.fix.satellites_used);
            } else {
                printf("no GPS data available\n");
            }
        }
    }
puts("</Response>");

}

/* When you are done... */
gps_stream(&gps_data, WATCH_DISABLE, NULL);
gps_close (&gps_data);

return EXIT_SUCCESS;
}
