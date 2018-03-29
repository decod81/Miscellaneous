#include <stdio.h>
#include <wiringPiSPI.h>

main() {
	float k;
	int i, j;
	unsigned char data[3];

	if(wiringPiSPISetup(0, 1000000)<0) printf("Error occurred while SPISetup.\n");

	printf("CH1\tCH2\tCH3\tCH4\n");
	for(j=0; j<24; j++) {
		for(i=0; i<4; i++) {
			data[0] = 128+64 + 8*i;
			data[1] = 0;
			data[2] = 0;
			wiringPiSPIDataRW(0, data, 3);
			printf("%d\t", 256*data[1] + data[2]);
		}
	printf("\n");
	}

	while(1) {
		data[0] = 128+64 + 16;
		data[1] = 0;
		data[2] = 0;
		wiringPiSPIDataRW(0, data, 3);
		k = 80.0*(256*data[1] + data[2])/65535;
		for(j=0; j<k; j++)
			printf(" ");
		printf("*\n");
		usleep(50000);
	}
}
