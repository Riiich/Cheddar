///// From UsbApi.c
void UpdateUSB();
void EventHdl_BusSuspend(void) ;
unsigned char ctrlFIFORead(unsigned int len, unsigned char *buf);
unsigned char ctrlFIFOWrite(unsigned int len, unsigned char *buf);


///// From UsbLibPro.c
void controlCMD();


