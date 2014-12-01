/*
 * mcp4921-Dac.c
 * This file is part of spi_dac_demo
 *
 * Copyright (C) 2014 - DivyaKrishna
 *
 * spi_dac_demo is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * spi_dac_demo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with spi_dac_demo. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <errno.h>
#include <linux/spi/spidev.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>      
#include <stdint.h>

#include "spi-dev-lib.h"
#include "mcp4921-Dac.h"

/************************************************************************/
int writeDac(spiData *data)
{
	int ret;
    	int loopCount = 2000;
    	int count = 0;	
    	uint8_t sineWave[128] = {0x63, 0x38, 0x2B, 0x39, 0xF1, 0x39, 0xB1, 0x3A, 0x6A, 0x3B, 0x1C, 0x3C, 0xC3, 0x3C, 0x5E, 0x3D,
                          0xEC, 0x3D, 0x6C, 0x3E, 0xDB, 0x3E, 0x3A, 0x3F, 0x87, 0x3F, 0xC1, 0x3F, 0xE8, 0x3F, 0xFC, 0x3F, 
                          0xFC, 0x3F, 0xE8, 0x3F, 0xC1, 0x3F, 0x87, 0x3F, 0x3A, 0x3F, 0xDB, 0x3E, 0x6C, 0x3E, 0xEC, 0x3D, 
                          0x5E, 0x3D, 0xC3, 0x3C, 0x1C, 0x3C, 0x6A, 0x3B, 0xB1, 0x3A, 0xF1, 0x39, 0x2B, 0x39, 0x63, 0x38, 
                          0x9B, 0x37, 0xD3, 0x36, 0x0D, 0x36, 0x4D, 0x35, 0x94, 0x34, 0xE2, 0x33, 0x3B, 0x33, 0xA0, 0x32, 
                          0x12, 0x32, 0x92, 0x31, 0x23, 0x31, 0xC4, 0x30, 0x77, 0x30, 0x3d, 0x30, 0x16, 0x30, 0x02, 0x30, 
                          0x02, 0x30, 0x16, 0x30, 0x3D, 0x30, 0x77, 0x30, 0xC4, 0x30, 0x23, 0x31, 0x92, 0x31, 0x12, 0x32, 
                          0xA0, 0x32, 0x3B, 0x33, 0xE2, 0x33, 0x94, 0x34, 0x4D, 0x35, 0x0D, 0x36, 0xD3, 0x36, 0x9B, 0x37};
 

	while(loopCount > 0)
  	{  
    		for (count=0; count < 128; count=count+2)
    		{
       			data->tx[0] = sineWave[count];
       			data->tx[1] = sineWave[count+1];
			
			ret = spiTransfer(data);
       			if (ret < 1)                                 
     			{ 
    				printf("can't send SPI message\n");
        			perror("MSG snd failed\n");
        			usleep(10000);
       
     			}
    		}		
     		loopCount--;
   	}
	return 0;
}

int getDigitalData(spiData *data)
{
	unsigned int Din = 0;
	float Vout = 0;
	float res = 0;
	int ret;
	unsigned int temp;
	
	printf("Enter the Vout value in the range of 0V-3.3V\n");
	scanf("%f",&Vout);
	if(Vout > VREF || Vout < 0){
		printf("Voltage range: (0V-3.3V)\n");
		return -1;
	}
	
	res = (NUMBEROFBITS)/(VREF*GAIN);
	Din = (unsigned int) (Vout * res);
	Din = Din & 0xFFF;
	printf("The digital value of the given input voltage is %d : %X\n", Din, Din);
	temp = (Din & 0xF00);
		
	data->tx[0] = (Din & 0x0FF);          
	data->tx[1] = ((CONFIG_INPUT_ONE_TO_DAC) | (temp >> 8)); 	
			
	ret = spiTransfer(data);
	
	printf("data->tx[0] is %d : %X \n",data->tx[0],data->tx[0]);
	printf("data->tx[1] is %d : %X \n",data->tx[1],data->tx[1]);
	
	
       	if (ret < 1)                                          
     	{ 
    		printf("can't send SPI message\n");
        	perror("MSG snd failed\n");
        	
       	}
 	return 0;
}
	
/***************************************************************************/
int main(int argc, char **argv)
{
	int opt;
	spiData *data = malloc(sizeof(spiData));

	if(argc < 2)
	{
		printf("Usage: %s <spi device name>, e.g: mcp4921-Dac /dev/spidev3.0",argv[0]);
		exit(-1);
	}
	
	data->mode = 0;
	data->bits = 16;
	data->speed = 1000000;           
	data->delay = 0;
	sprintf(data->device, "%s", argv[1]);


	if(spiInit(data)){
		perror("SPI Init failed");
		exit(-1);
	}
	
	printf("Select 0 to get digital value and 1 to generate a sine wave\n");
	scanf("%d",&opt);

	switch(opt)
	{
		case 0: printf("Get the digital value for the given input voltage\n");
			getDigitalData(data);
			break;
		case 1: printf("Generate a sine wave\n");
			writeDac(data);
			break;
	}
	
	free(data);
	close(data->fileDescriptor);

	return 0;             
}


