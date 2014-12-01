/* Formula to find the ideal output voltage 
Vout = (Vref*Gain*Din)/2^n
where n= number of bits=12

To get the digital input value
Din=(Vout*2^n)/(Vref*Gain) */

#define VREF 3.3
#define GAIN 1
#define NUMBEROFBITS 4096


/*DAC A or DAC B Select bit*/
#define DAC_B (1 << 7)
#define DAC_A (0 << 7)

/*BUF: V REF Input Buffer Control bit*/
#define BUFFERED (1 << 6)
#define UNBUFFERED (0 << 6)

/*GA: Output Gain Select bit*/
#define CONFIGGAIN (1 << 5)
#define CONFIGGAINTWO (0 << 5)

/*SHDN: Output Power Down Control bit*/
#define OUTPUT_POWER_DOWN (1 << 4)
#define OUTPUT_BUFFER_DISABLED (0 << 4)

/*Config bits input to Dac*/
#define CONFIG_INPUT_ONE_TO_DAC (DAC_A | BUFFERED | CONFIGGAIN | OUTPUT_POWER_DOWN)
#define CONFIG_INPUT_TWO_TO_DAC (DAC_B | UNBUFFERED | CONFIGGAINTWO | OUTPUT_BUFFER_DISABLED)




