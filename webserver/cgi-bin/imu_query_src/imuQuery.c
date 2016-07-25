
#include <fcgi_stdio.h>
#include "LSM9DS0.h"
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include "sensor.c"
#include "bmp180.h"



#define AA 0.97         // complementary filter constant

#define A_GAIN 0.0573      // [deg/LSB]
#define G_GAIN 0.070     // [deg/s/LSB]
#define RAD_TO_DEG 57.29578
#define M_PI 3.14159265358979323846


///////Kalman filter inclinometer variables///////////////////
float Q_angle  =  0.01;
float Q_gyro   =  0.0003;
float R_angle  =  0.01;
float x_bias = 0;
float y_bias = 0;
float XP_00 = 0, XP_01 = 0, XP_10 = 0, XP_11 = 0;
float YP_00 = 0, YP_01 = 0, YP_10 = 0, YP_11 = 0;
float KFangleX = 0.0;
float KFangleY = 0.0;

//////////Kalman Filter Inclinometer functions//////////////
int inclinationRead(float *accX, float *accY);

void  INThandler(int sig)
{
        signal(sig, SIG_IGN);
        exit(0);
}

/*int mymillis()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec) * 1000 + (tv.tv_usec)/1000;
}*/

/*int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;
    return (diff<0);
}*/

////////////// Compass Functions/////////////////////////////
int compassRead(float *compass);

////////////// bmp180 Functions/////////////////////////////
int tempPressureRead (float *temp, long *pressure);

////////////Main Function//////////////////////////////
int main(int argc, char *argv[]){

	 while(FCGI_Accept() >= 0)   {
		///kalman execution////
		float accX =0;
		float accY =0;
		float *px = &accX;
		float *py = &accY;
		if (inclinationRead(px, py) < 0){//if error
			accX=-999.9;
			accY=-999.9;
				//printf ("Error calculating inclination.");
			}
		else{
				//printf ("accX %f accY %f \n",accX,accY);
			}

		////compass execution//////
		float compass =0;
		float *pcompass = &compass;

		if (compassRead(pcompass) < 0){//if error
			compass=-999.9;
				//printf ("Error calculating inclination.");
			}
		else{
				//printf ("compass %f \n",compass);
			}

		//// bmp180 execution /////////
		long pressure;
		float temperature;

		long *ppressure = &pressure;
		float *ptemperature = &temperature;

		if (tempPressureRead(ptemperature, ppressure) < 0){//if error
			temperature = -999.9;
			pressure = -999.9;
				//printf ("Error calculating inclination.");
			}
		else{
				//printf ("pressure: %li temperature: %f \n",pressure, temperature);
			}

		accX >= 0.0f ? (accX+0.5f) : (accX-0.5f); //these are for rounding float purposes, in order to late cast to int.
		accY >= 0.0f ? (accY+0.5f) : (accY-0.5f);
		compass >= 0.0f ? (compass+0.5f) : (compass-0.5f);


		puts("Content-type: application/xml\n");
		puts("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		puts("<Response>");
		printf("	<tipperInclination>%d</tipperInclination>", (int) accY);
		printf("	<sideInclination>%d</sideInclination>", (int)accX);
		printf("	<compass>%d</compass>", (int)compass);
		printf("	<temperature>%.1f</temperature>", temperature);
		printf("	<pressure>%.1f</pressure>", ((float)pressure)/100.0f);
		puts("</Response>");	
	}
	return 0;
}


///////////Compass Functions implementation//////////////////////////
int compassRead(float *compass)
{
	char filename[20];
	int magRaw[3];
	int accRaw[3];
	float accXnorm,accYnorm,pitch,roll,magXcomp,magYcomp;
	int file;

	//Open the i2c bus
	sprintf(filename, "/dev/i2c-%d", 1);
	file = open(filename, O_RDWR);
	if (file<0) {
        	//printf("Unable to open I2C bus!");
            return -1;
	}


	//Select the magnetomoter
	if (ioctl(file, I2C_SLAVE, MAG_ADDRESS) < 0) {
              //  printf("Error: Could not select magnetometer\n");
			return -1;
        }



	// Enable accelerometer.
        writeAccReg(CTRL_REG1_XM, 0b01100111); //  z,y,x axis enabled, continuos update,  100Hz data rate
        writeAccReg(CTRL_REG2_XM, 0b00100000); // +/- 16G full scale

        //Enable the magnetometer
        writeMagReg( CTRL_REG5_XM, 0b11110000);   // Temp enable, M data rate = 50Hz
        writeMagReg( CTRL_REG6_XM, 0b01100000);   // +/-12gauss
        writeMagReg( CTRL_REG7_XM, 0b00000000);   // Continuous-conversion mode

		//comienzo del ex while
		readMAG(magRaw);
		readACC(accRaw);

		//If your IMU is upside down, comment out the two lines below which we correct the tilt calculation
		//accRaw[0] = -accRaw[0];
		//accRaw[1] = -accRaw[1];

		//Compute heading
	        float heading = 180 * atan2(magRaw[1],magRaw[0])/M_PI;

		//Convert heading to 0 - 360
        //	if(heading < 0)
	  	//      heading += 360;

		//printf("heading %7.3f \t ", heading);

		//Normalize accelerometer raw values.
                accXnorm = accRaw[0]/sqrt(accRaw[0] * accRaw[0] + accRaw[1] * accRaw[1] + accRaw[2] * accRaw[2]);
                accYnorm = accRaw[1]/sqrt(accRaw[0] * accRaw[0] + accRaw[1] * accRaw[1] + accRaw[2] * accRaw[2]);

		//Calculate pitch and roll
		pitch = asin(accXnorm);
		roll = -asin(accYnorm/cos(pitch));

		//Calculate the new tilt compensated values
		magXcomp = magRaw[0]*cos(pitch)+magRaw[02]*sin(pitch);
		magYcomp = magRaw[0]*sin(roll)*sin(pitch)+magRaw[1]*cos(roll)-magRaw[2]*sin(roll)*cos(pitch);


		//Calculate heading
		heading = 180*atan2(magYcomp,magXcomp)/M_PI;

                //Convert heading to 0 - 360
		//if(heading < 0)
		//      heading += 360;

		(*compass) = heading ;

		
		//printf("Compensated  Heading %7.3f  \n", heading);
	return 0;
}

///////////////// bmp180 altitide temperature implementation ////////////////
int tempPressureRead (float *temp, long *pressure){
	char *i2c_device = "/dev/i2c-1";
	int address = 0x77;
	
	void *bmp = bmp180_init(address, i2c_device);
	
	bmp180_eprom_t eprom;
	bmp180_dump_eprom(bmp, &eprom);
	
	
	bmp180_set_oss(bmp, 1);
	
	if(bmp != NULL){
		//int i;
		(*temp) = bmp180_temperature(bmp);
		(*pressure) = bmp180_pressure(bmp);
	
		bmp180_close(bmp);
	}

	return 0;
}

////////////Kalman Inclination functions implementation///////////////

int inclinationRead(float *AccXangle, float *AccYangle)
{

	int  accRaw[3];

    signal(SIGINT, INThandler);

	enableIMU();

	//read ACC and GYR data
	readACC(accRaw);

	//Convert Accelerometer values to degrees
	(*AccXangle) = (float) (atan2(accRaw[1],accRaw[2])+M_PI)*RAD_TO_DEG;
	(*AccYangle) = (float) (atan2(accRaw[2],accRaw[0])+M_PI)*RAD_TO_DEG;

        //Change the rotation value of the accelerometer to -/+ 180 and move the Y axis '0' point to up.
        //Two different pieces of code are used depending on how your IMU is mounted.
        //If IMU is upside down
	/*
        if ((*AccXangle) >180)
                (*AccXangle) -= (float)360.0;

        (*AccYangle)-=90;
        if ((*AccYangle) >180)
                (*AccYangle) -= (float)360.0;
	*/

        //If IMU is up the correct way, use these lines
        (*AccXangle) -= (float)180.0;
	if ((*AccYangle) > 90)
	        (*AccYangle) -= (float)270;
	else
		(*AccYangle) += (float)90;


	return 0;
}



