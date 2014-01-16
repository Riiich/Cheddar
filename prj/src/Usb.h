///// From UsbApi.c
void UpdateUSB();
unsigned char ctrlFIFORead(unsigned char *buf);
unsigned char ctrlFIFOWrite(unsigned int len, unsigned char *buf);


///// From UsbLibPro.c
void controlCMD();


