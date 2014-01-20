#ifndef	__TIMER_H__
#define	__TIMER_H__

extern idata unsigned char ms_TH1_value;
extern idata unsigned char ms_TL1_value;

void InitTimer(void);
void DelayMS(unsigned short msec);

#endif

