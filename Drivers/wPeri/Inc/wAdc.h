#ifndef __wADC_H
#define __wADC_H

EXT u16 ADC1In[SampleNum*ADChNum];

void wADC1DMA2Init(void);
void wADCIOInit(void);
void wADC1Init(void);
void wDMA2Stream0Init(void);
void wADC1DMA2Trigger(void);

#endif
