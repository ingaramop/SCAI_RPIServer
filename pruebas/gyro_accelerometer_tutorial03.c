/*
	This program  reads the angles from the accelerometer and gyroscope
	on a BerryIMU connected to a Raspberry Pi.
	http://ozzmaker.com/


    Copyright (C) 2014  Mark Williams

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Library General Public License for more details.
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
    MA 02111-1307, USA
*/

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


#define DT 0.02         // [s/loop] loop period. 20ms
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
float kalmanFilterX(float accAngle, float gyroRate);
float kalmanFilterY(float accAngle, float gyroRate);
int kalmanInclinationRead(float *kalmanX, float *kalmanY);

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
int file;
int compassRead(float *compass);

////////////Main Function//////////////////////////////
int main(int argc, char *argv[]){

///kalman execution////
float kalmanX =0;
float kalmanY =0;
float *px = &kalmanX;
float *py = &kalmanY;
if (kalmanInclinationRead(px, py) < 0){//if error
	kalmanX=-999.9;
	kalmanY=-999.9;
		printf ("Error calculating inclination.");
	}
else{
		printf ("kalmanX %f kalmanY %f \n",kalmanX,kalmanY);
	}

////compass execution//////
	float compass =0;
	float *pcompass = &compass;

if (compassRead(pcompass) < 0){//if error
	compass=-999.9;
		printf ("Error calculating inclination.");
	}
else{
		printf ("compass %f \n",compass);
	}
	

}

////////////Kalman Inclination functions implementation///////////////

int kalmanInclinationRead(float *kalmanX, float *kalmanY)
{

	float rate_gyr_y = 0.0;   // [deg/s]
	float rate_gyr_x = 0.0;    // [deg/s]
	float rate_gyr_z = 0.0;     // [deg/s]

	int  accRaw[3];
	int  magRaw[3];
	int  gyrRaw[3];



	float gyroXangle = 0.0;
	float gyroYangle = 0.0;
	float gyroZangle = 0.0;
	float AccYangle = 0.0;
	float AccXangle = 0.0;
	float CFangleX = 0.0;
	float CFangleY = 0.0;

	//int startInt  = mymillis();
	struct  timeval tvBegin, tvEnd,tvDiff;


    signal(SIGINT, INThandler);

	enableIMU();

	gettimeofday(&tvBegin, NULL);


	//read ACC and GYR data
	readACC(accRaw);
	readGYR(gyrRaw);

	//Convert Gyro raw to degrees per second
	rate_gyr_x = (float) gyrRaw[0]  * G_GAIN;
	rate_gyr_y = (float) gyrRaw[1]  * G_GAIN;
	rate_gyr_z = (float) gyrRaw[2]  * G_GAIN;



	//Calculate the angles from the gyro
	gyroXangle+=rate_gyr_x*DT;
	gyroYangle+=rate_gyr_y*DT;
	gyroZangle+=rate_gyr_z*DT;




	//Convert Accelerometer values to degrees
	AccXangle = (float) (atan2(accRaw[1],accRaw[2])+M_PI)*RAD_TO_DEG;
	AccYangle = (float) (atan2(accRaw[2],accRaw[0])+M_PI)*RAD_TO_DEG;






        //Change the rotation value of the accelerometer to -/+ 180 and move the Y axis '0' point to up.
        //Two different pieces of code are used depending on how your IMU is mounted.
        //If IMU is upside down
	/*
        if (AccXangle >180)
                AccXangle -= (float)360.0;

        AccYangle-=90;
        if (AccYangle >180)
                AccYangle -= (float)360.0;
	*/

        //If IMU is up the correct way, use these lines
        AccXangle -= (float)180.0;
	if (AccYangle > 90)
	        AccYangle -= (float)270;
	else
		AccYangle += (float)90;

	//Kalman Filter
	(*kalmanX) = kalmanFilterX(AccXangle, rate_gyr_x);
	(*kalmanY) = kalmanFilterY(AccYangle, rate_gyr_y);


/*	//Complementary filter used to combine the accelerometer and gyro values.
	CFangleX=AA*(CFangleX+rate_gyr_x*DT) +(1 - AA) * AccXangle;
	CFangleY=AA*(CFangleY+rate_gyr_y*DT) +(1 - AA) * AccYangle;
	printf ("AccXangle: %f AccYangle %f ",AccXangle,AccYangle);*/

	return 0;
}


  float kalmanFilterX(float accAngle, float gyroRate)
  {
    float  y, S;
    float K_0, K_1;


    KFangleX += DT * (gyroRate - x_bias);

    XP_00 +=  - DT * (XP_10 + XP_01) + Q_angle * DT;
    XP_01 +=  - DT * XP_11;
    XP_10 +=  - DT * XP_11;
    XP_11 +=  + Q_gyro * DT;

    y = accAngle - KFangleX;
    S = XP_00 + R_angle;
    K_0 = XP_00 / S;
    K_1 = XP_10 / S;

    KFangleX +=  K_0 * y;
    x_bias  +=  K_1 * y;
    XP_00 -= K_0 * XP_00;
    XP_01 -= K_0 * XP_01;
    XP_10 -= K_1 * XP_00;
    XP_11 -= K_1 * XP_01;

    return KFangleX;
  }


  float kalmanFilterY(float accAngle, float gyroRate)
  {
    float  y, S;
    float K_0, K_1;


    KFangleY += DT * (gyroRate - y_bias);

    YP_00 +=  - DT * (YP_10 + YP_01) + Q_angle * DT;
    YP_01 +=  - DT * YP_11;
    YP_10 +=  - DT * YP_11;
    YP_11 +=  + Q_gyro * DT;

    y = accAngle - KFangleY;
    S = YP_00 + R_angle;
    K_0 = YP_00 / S;
    K_1 = YP_10 / S;

    KFangleY +=  K_0 * y;
    y_bias  +=  K_1 * y;
    YP_00 -= K_0 * YP_00;
    YP_01 -= K_0 * YP_01;
    YP_10 -= K_1 * YP_00;
    YP_11 -= K_1 * YP_01;

    return KFangleY;
  }
///////////Compass Functions implementation//////////////////////////
int compassRead(float *compass)

{
	char filename[20];
	int magRaw[3];
	int accRaw[3];
	float accXnorm,accYnorm,pitch,roll,magXcomp,magYcomp;




	//Open the i2c bus
	sprintf(filename, "/dev/i2c-%d", 1);
	file = open(filename, O_RDWR);
	if (file<0) {
        	printf("Unable to open I2C bus!");
                exit(1);
	}


	//Select the magnetomoter
	if (ioctl(file, I2C_SLAVE, MAG_ADDRESS) < 0) {
                printf("Error: Could not select magnetometer\n");
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
        	if(heading < 0)
	  	      heading += 360;

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
		if(heading < 0)
		      heading += 360;

		(*compass) = heading;
		//printf("Compensated  Heading %7.3f  \n", heading);

}




