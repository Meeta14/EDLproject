/********************
EE337: ADC IC MCP3008 interfacing with pt-51
(1) Complete spi_init() function from spi.h 
(2) Uncomment LM35 interfacing code from main function
		Choose appropriate scaling factor for converting temperature 
		sensor reading sampled using ADC such that it will be in 
		degree Celsius and display it on the LCD)
*********************/

#include <at89c5131.h>
#include "lcd.h"																				//Driver for interfacing lcd 
#include "mcp3008.h"																		//Driver for interfacing ADC ic MCP3008


char adc_ip_data_ascii[6]={0,0,0,0,0,'\0'};							//string array for saving ascii of input sample
code unsigned char display_msg1[]="Volt.: ";						//Display msg on 1st line of lcd
code unsigned char display_msg2[]=" mV";								//Display msg on 2nd line of lcd
unsigned char display_msg3[]={0,0,0,'.',0,' ',223,'C','\0'};//"xxx °C", Display msg on 2nd line of lcd
code unsigned char display_msg4[]="Temp.: ";						//Display msg on 2nd line of lcd
unsigned int array_adc[10];
unsigned int idx=0;
unsigned int x;	
unsigned int min_value=0;
unsigned int i=0;

void main(void)
{
	int j=0;
	unsigned int adc_data=0;
	//float temperature;
	
	spi_init();																					
	adc_init();
  lcd_init();	
	
	while(1)
	{
		if(idx==9){	                            // when we have completed 10 instances of measurement
			i=0;
			min_value=array_adc[0];
			for(i=1;i<10;i++){                     // find the minimum voltage
					if(min_value>array_adc[i]){
						min_value=array_adc[i];
					}
			}
			adc_data = (unsigned int) (min_value*3.2258); 							//Converting received 10 bits value to milli volt (3.3*1000*i/p /1023)	
			lcd_cmd(0x80);																			//Move cursor to first line
			lcd_write_string(display_msg1);											//Display "Volt: " on first line of lcd
			
			if(adc_data>10500){                                // if the voltage is more than 10% V ratings we add the  IR component
					adc_data=adc_data+0.0112;
			}
			int_to_string(adc_data,adc_ip_data_ascii);					//Converting integer to string of ascii
			
			lcd_write_string(adc_ip_data_ascii);								//Print analog sampled input on lcd
			lcd_write_string(display_msg2);											//Display "XXXXX mV"
					
			idx=0;
		}
	 	 
		x = adc(0);																					//Read analog value from 0th channel of ADC Ic MCP3008
    array_adc[idx] = x;                                // keep the array updated
	}
}