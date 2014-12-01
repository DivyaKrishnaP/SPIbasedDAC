
#define ARRAY_SIZE(a)   (sizeof(a) / sizeof((a)[0]))

/* Variables for struct spi_ioc_transfer */
typedef struct{
	uint8_t tx[2];          /*FIXME*/
	uint8_t rx[2];		/*FIXME*/
	uint8_t mode;                     
	uint8_t bits;                   
	uint32_t speed;           
	uint16_t delay;  

	char device[20];
	int fileDescriptor;
	       
}spiData;

/******************************************************************************/
int spiInit(spiData *data);

/******************************************************************************/
int spiTransfer(spiData *data);
