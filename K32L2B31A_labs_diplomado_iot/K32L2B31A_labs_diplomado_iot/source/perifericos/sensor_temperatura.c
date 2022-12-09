/*! @file : sensor_temperature.c
 * @author  Victor Alfonso Fernandez Hoyos
 * @version 1.0.0
 * @date    09/12/2022
 * @brief   Driver para lectura de sensor de temperatura interna del microcontrolador
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sensor_temperatura.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/
uint32_t getTemperatureADC(void);
uint32_t getBandGapADC(void);

/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
 uint32_t getTemperatureADC(void) {
	uint32_t adc_temperature_value;
	ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP,	&ADC0_channelsConfig[2]);

	while (0U== (kADC16_ChannelConversionDoneFlag& ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL,ADC0_CH0_CONTROL_GROUP))) {
	}

	adc_temperature_value=ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP);

	return(adc_temperature_value);
}

 uint32_t getBandGapADC(void) {
	uint32_t adc_bandgap_value;
	ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP,	&ADC0_channelsConfig[1]);

	while (0U== (kADC16_ChannelConversionDoneFlag& ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL,ADC0_CH0_CONTROL_GROUP))) {
	}

	adc_bandgap_value=ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP);

	return(adc_bandgap_value);
}

 float getTemperatureValue(void){
	float Vdd;
	float Temp;
 	float Vtemp;
 	float Vslope;

 	uint32_t adc_temp_value;
 	uint32_t adc_bandgap_value;

 	adc_bandgap_value=getBandGapADC();						//Captura ADC para obtener valor de referencia bandgap
 	Vdd=(float)((float)4095*1.2/adc_bandgap_value);			//Convierte ADC a Voltaje para obtener Vdd

 	adc_temp_value=getTemperatureADC();						//Captura ADC del sensor de temperatura
 	Vtemp = (float)((adc_temp_value)*(Vdd)/ (float)(4095)); //Convierte ADC a voltaje para obtener Vtemp

 	if(Vtemp>=VTEMP25)
 		Vslope=(float)1.646;	//Cold Slope
 	else
 		Vslope=(float)1.769;	//Hot Slope

 	Temp=(float)(((float)(Vtemp)-(float)(VTEMP25))/(float)(Vslope)); //Convierte voltaje a temperatura
 	Temp=(float)(25-Temp);

 	return(Temp);
 }
