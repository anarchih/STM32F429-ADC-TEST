#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_gpio.h"
#define ADC1_DR_ADDRESS 0x4001204C
uint8_t value[1024];

int main(){

    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef DMA_InitStructure;    
    GPIO_InitTypeDef      GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* Configure ADC3 Channel13 pin as analog input ******************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonStructInit(&ADC_CommonInitStructure);
    ADC_CommonInit(&ADC_CommonInitStructure);
    
    ADC_StructInit(&ADC_InitStructure); 
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_0; 
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr =(unsigned int) &value;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = 1024;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; 
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream0, ENABLE);

    ADC_Cmd(ADC1, ENABLE);

    ADC_DMACmd(ADC1, ENABLE);
    ADC_SoftwareStartConv(ADC1);

    

    while(1);   

    return 0;
}
