#include "platform.h"

// Device Descriptor
code unsigned char device_desc[] = {
	0x12,	//	Length = 18 (0x12)
	0x01,	//	Device Descriptor Type= 1
	0x00,	//	USB Version L (.00) 
	0x02,	//	USB Version H (2.)
	0xEF,	//	Device Class (EF) Miscellaneous Device Class
	0x02,	//	Device Sub Class (02) Common Class
	0x01,	//	Device Protocol (01) Interface association descriptor ;EF 02 01 for Multi-Function Class code  Bruce
	0x40,	//	Max Packet Size for EP0 = 64
	0xBA,	//	Vendor ID Low 
	0x05,	//	Vendor ID High (0x5BA)
	0x0B,	//	Product ID Low
	0x00,	//	Product ID High (0x000B)
	0x01,	//	BCD Device Revision Low
	0x01,	//	BCD Device Revision High (0x0001)
	0x01,	//	Manufacturer String Index = 1
	0x02,	//	Product String Index = 2
	0x03,	//	Serial Number String = 3
	0x01	//	Number of Configurations = 1
};

// String Descriptor 0, language ID
code unsigned char string_desc0[] = {
	0x04,		// Length = 4
	0x03,		// String Descriptor Type = 3
	0x09,		
	0x04		// 0x0409 is Engligh
};

// String Descriptor 1, manufacturer string
code unsigned char string_desc1[] = {
	0x28,		// Length = 0x28
	0x03,		// String Descriptor Type = 3
	'D',0x00,'I',0x00,'G',0x00,'I',0x00,'T',0x00,'A',0x00,'L',0x00,'P',0x00,
	'E',0x00,'R',0x00,'S',0x00,'O',0x00,'N',0x00,'A',0x00,' ',0x00,'I',0x00,
	'n',0x00,'c',0x00,'.',0x00
};

// String Descriptor 2, product string
code unsigned char string_desc2[] = {
	0x24,		// Length = 0x24
	0x03,		// String Descriptor Type = 3
	'U',0x00,'.',0x00,'r',0x00,'.',0x00,'U',0x00,' ',0x00,'5',0x00,'X',0x00,
	'0',0x00,'0',0x00,' ',0x00,'B',0x00,'a',0x00,'n',0x00,'n',0x00,'e',0x00,
	'r',0x00
};

// String Descriptor 3, serial string
code unsigned char string_desc3[] = {
	0x04,		// Length = 0x04
	0x03,		// String Descriptor Type = 3
	'0',0x00
};

// Device Qualifier Descriptor
code unsigned char Ide_Dvc_QF_D[] = {
    0x0A,0x06,0x00,0x02,0x00,0x00,0x00,0x40,
    0x01,0x00
};


/////////////////////////////////////////////
// UVC configuration Descriptor
code unsigned char UVC_Std_D[] = {
    0x09,0x02,0x4B,0x02,0x04,0x01,0x00,0x80,0xFA,	// Configuration descriptor
													//  Descriptor type:          Configuration
	                                                //  NumInterface:             0x04 (4 interfaces)
	                                                //  ID ofthis Configuration:  0x01												


	0x08,0x0B,0x00,0x02,0x0E,0x03,0x00,0x02,		// Standard Video Interface Collection IAD
	                                                 // Descriptor type:  0x0B (INTERFACE ASSOCIATION DESCRIPTOR)
													 // FunctionClass:    0x0E (CC_VIDEO)
	                                                 // FunctionSubClass: 0x03 (SC_VIDEO_INTERFACE_COLLECTION)


	0x09,0x04,0x00,0x00,0x01,0x0E,0x01,0x00,0x02,	// Standard VC interface descriptor 
	                                                 // Descriptor type:   INTERFACE
													 // InterfaceNumber:   0x00	(Index of this interface)
													 // AlternateSetting:  0x00
													 // InterfaceClass:    0x0E (CC_VIDEO) 
													 // InterfaceSubClass: 0x01 (SC_VIDEOCONTROL)
													 // NumEndpoint:	   0x01	(1 interrupt endpoint)
	
	0x0D,0x24,0x01,0x00,0x01,0x4F,0x00,0x80,0xC3,0xC9,0x01,0x01,0x01,// Class-specific VC interface descriptor
																	  // Descriptor type:               CS_INTERFACE
																	  // Descriptor subtype:            VC_HEADER
																	  // Number of streaming interface: 0x01
	
	0x12,0x24,0x02,0x01,0x01,0x02,0x00,0x00,0x00,	// Input Terminal descriptor,
	0x00,0x00,0x00,0x00,0x00,0x03,0x0A,0x00,0x00,	   //  Descriptor type:     CS_INTERFACE,
	                                                   //  Descriptor subtype:  VC_INPUT_TERMINAL type
	                                                   //  ID of this terminal: 1
													   //  Terminal type:       ITT_CAMERA

	
	0x0B,0x24,0x05,0x02,0x01,0x00,0x00,0x02,0x7F,0x17,0x00,	// Processing Unit
															  // Descriptor type:    CS_INTERFACE
															  // Descriptor subtype: VC_PROCESSING_UNIT
															  // ID ofthis unit:     2
	
	0x09,0x24,0x03,0x03,0x01,0x01,0x00,0x02,0x00,	// Output Terminal descriptor, 
													  // Descriptor type:     CS_INTERFACE
													  // Descriptor subtype:  VC_OUTPUT_TERMINAL type
	                                                  // ID ofthis unit:      3
													  // Terminal type:       TT_STREAMING

	
	0x1C,0x24,0x06,0x04,0x5D,0xC7,0x17,0xA9,0x19,0x41,0xDA,0x11,0xAE,0x0E,// Extension Unit descriptor
	0x00,0x0D,0x56,0xAC,0x7B,0x4C,0x08,0x01,0x01,0x03,0x59,0x80,0x80,0x00, // Descriptor type:    CS_INTERFACE
	                                                                       // Descriptor subtype: VC_EXTENSION_UNIT
	                                                                       



	0x07,0x05,0x84,0x03,0x0A,0x00,0x05,	  		   // Standard Interrupt Endpoint Descriptor
													 // Descriptor type:  ENDPOINT
													 // Endpoint address: 0x81== (IN endpoint 1)
													 // bmAttributes:     0x03 (Interrupt)

    
	0x05,0x25,0x03,0x0A,0x00,					   //Class-specific interrupt endpoint Descriptor
													// Descriptor type:    CS_ENDPOINT
													// Descriptor subtype: EP_INTERRUPT

	
	
	
	0x09,0x04,0x01,0x00,0x00,0x0E,0x02,0x00,0x00,  //Standard VS interface Descriptor (Alternate Setting 0)
													// Descriptor type:   INTERFACE
													// InterfaceNumber:   0x01 (index of this interface)
													// AlternateSetting:  0x00 (index of this Alternate Setting)
													// NumEndpoint:       0x00
													// InterfaceClass:    0x0E (CC_VIDEO)
													// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)


	0x0E,0x24,0x01,0x01,0xDF,0x00,0x86,0x00,0x03,0x02,0x01,0x01,0x01,0x00, // Class-specific VS header descriptor
																			// Descriptor type:    0x24 (CS_INTERFACE)
																			// Descriptor subtype: 0x01 (VS_INPUT_HEADER)
																			// EndpointAddress:    0x82 ==(Address of the iso endpoint)
																			// StillCaptureMethod: 0x02 (Method 2)

	0x1B,0x24,0x04,0x01,0x05,0x59,0x55,0x59,0x32,0x00,0x00,0x10,0x00,	   // Class-specific VS format descriptor
	0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71,0x10,0x01,0x00,0x00,0x00,0x00,  // Descriptor type:    0x24 (CS_INTERFACE)
																			// Descriptor subtype: 0x04 (VS_FORMAT_UNCOMPRESSED)															
																			// NumFrameDescriptor: 0x05 (5 frams descriptors)

/*
	0x1B,0x24,0x04,0x01,0x05,0x7D,0xEB,0x36,0xE4,0x4F,0x52,0xCE,0x11,	   // Class-specific VS format descriptor
	0x9F,0x53,0x00,0x20,0xAF,0x0B,0xA7,0x70,0x10,0x01,0x00,0x00,0x00,0x00,  // Descriptor type:    0x24 (CS_INTERFACE)
																			// Descriptor subtype: 0x04 (VS_FORMAT_UNCOMPRESSED)															
																			// NumFrameDescriptor: 0x05 (5 frams descriptors)
                                                                            // guidFormat:         32595559-0000-0010-8000-00aa00389b71 (bit5~20)
                                                                            // bBitsPerPixel:      16 (bit 21) 0x10
*/



	0x1E,0x24,0x05,0x01,0x00,0x80,0x02,0xE0,0x01,0x00,0x00,0xB8,0x0B,0x00,0x00,	// Class-specific VS frame descriptor
	0xB8,0x0B,0x00,0x60,0x09,0x00,0x15,0x16,0x05,0x00,0x01,0x15,0x16,0x05,0x00,  // Descriptor type:    0x24 (CS_INTERFACE)
																			     // Descriptor subtype: 0x05 (VS_FRAME_UNCOMPRESSED)
																				 // 1st frame descriptor
	
	0x1E,0x24,0x05,0x02,0x00,0x60,0x01,0x20,0x01,0x00,0x00,0xB8,0x0B,0x00,0x00,	// Class-specific VS frame descriptor
	0xB8,0x0B,0x00,0x18,0x03,0x00,0x15,0x16,0x05,0x00,0x01,0x15,0x16,0x05,0x00,	 // Descriptor type:    0x24 (CS_INTERFACE)
																			     // Descriptor subtype: 0x05 (VS_FRAME_UNCOMPRESSED)
																				 // 2nd frame descriptor
	
	0x1E,0x24,0x05,0x03,0x00,0x40,0x01,0xF0,0x00,0x00,0x00,0xB8,0x0B,0x00,0x00,	// Class-specific VS frame descriptor
	0xB8,0x0B,0x00,0x58,0x02,0x00,0x15,0x16,0x05,0x00,0x01,0x15,0x16,0x05,0x00,	 // Descriptor type:    0x24 (CS_INTERFACE)
																			     // Descriptor subtype: 0x05 (VS_FRAME_UNCOMPRESSED)
																				 // 3rd frame descriptor
   
    0x1E,0x24,0x05,0x04,0x00,0xB0,0x00,0x90,0x00,0x00,0x00,0xB8,0x0B,0x00,0x00,	 //	  Class-specific VS frame descriptor
    0xB8,0x0B,0x00,0xC6,0x00,0x00,0x15,0x16,0x05,0x00,0x01,0x15,0x16,0x05,0x00,	 // Descriptor type:    0x24 (CS_INTERFACE)
																			     // Descriptor subtype: 0x05 (VS_FRAME_UNCOMPRESSED)
																				 // 4th frame descriptor

	0x1E,0x24,0x05,0x05,0x00,0xA0,0x00,0x78,0x00,0x00,0x00,0xB8,0x0B,0x00,0x00,	// Class-specific VS frame descriptor
	0xB8,0x0B,0x00,0x96,0x00,0x00,0x15,0x16,0x05,0x00,0x01,0x15,0x16,0x05,0x00,	 //	Descriptor type:    0x24 (CS_INTERFACE)
																			     // Descriptor subtype: 0x05 (VS_FRAME_UNCOMPRESSED)
																				 // 5th frame descriptor
	
	0x1A,0x24,0x03,0x00,0x05,0x80,0x02,0xE0,0x01,0x60,0x01,0x20,0x01, // Class-specific Still Image descriptor
	0x40,0x01,0xF0,0x00,0xB0,0x00,0x90,0x00,0xA0,0x00,0x78,0x00,0x00,  // Descriptor type:    0x24 (CS_INTERFACE)
																	   // Descriptor subtype: 0x03 (VS_STILL_IMAGE_FRAME)
																       // EndpointAdress:     0x00 (still image capture method 2 set to zero)


	
	0x06,0x24,0x0D,0x00,0x00,0x00,	 //Class-specific color matching descriptor
									  // Descriptor type:    0x24 (CS_INTERFACE)
							          // Descriptor subtype: 0x0D (VS_COLORFORMAT)


	0x09,0x04,0x01,0x01,0x01,0x0E,0x02,0x00,0x00,  //Standard VS interface Descriptor (Alternate Setting 1)
													// Descriptor type:   INTERFACE
													// InterfaceNumber:   0x01 (index of this interface)
													// AlternateSetting:  0x01 (index of this Alternate Setting)
													// NumEndpoint:       0x01
													// InterfaceClass:    0x0E (CC_VIDEO)
													// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)

	
	0x07,0x05,0x86,0x05,0x80,0x00,0x01,	  // Standard VS isochronous video data endpoint descriptor
										   // Descriptor type:   0x05 (ENDPOINT)
										   // EndpointAddress:   0x82== (IN endpoint 2)
										   // Attributes:        0x05 (Isochronous transfer type)								

	
	0x09,0x04,0x01,0x02,0x01,0x0E,0x02,0x00,0x00,  //Standard VS interface Descriptor (Alternate Setting 1)
													// Descriptor type:   INTERFACE
													// InterfaceNumber:   0x01 (index of this interface)
													// AlternateSetting:  0x02 (index of this Alternate Setting)
													// NumEndpoint:       0x01
													// InterfaceClass:    0x0E (CC_VIDEO)
													// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)
	
	0x07,0x05,0x86,0x05,0x00,0x02,0x01,	 // Standard VS isochronous video data endpoint descriptor
										   // Descriptor type:   0x05 (ENDPOINT)
										   // EndpointAddress:   0x82== (IN endpoint 2)
										   // Attributes:        0x05 (Isochronous transfer type)
	
	0x09,0x04,0x01,0x03,0x01,0x0E,0x02,0x00,0x00,  //Standard VS interface Descriptor (Alternate Setting 1)
													// Descriptor type:   INTERFACE
													// InterfaceNumber:   0x01 (index of this interface)
													// AlternateSetting:  0x03 (index of this Alternate Setting)
													// NumEndpoint:       0x01
													// InterfaceClass:    0x0E (CC_VIDEO)
													// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)
	
	0x07,0x05,0x86,0x05,0x00,0x04,0x01,	 // Standard VS isochronous video data endpoint descriptor
										   // Descriptor type:   0x05 (ENDPOINT)
										   // EndpointAddress:   0x82== (IN endpoint 2)
										   // Attributes:        0x05 (Isochronous transfer type)
	
	0x09,0x04,0x01,0x04,0x01,0x0E,0x02,0x00,0x00,  //Standard VS interface Descriptor (Alternate Setting 1)
													// Descriptor type:   INTERFACE
													// InterfaceNumber:   0x01 (index of this interface)
													// AlternateSetting:  0x04 (index of this Alternate Setting)
													// NumEndpoint:       0x01
													// InterfaceClass:    0x0E (CC_VIDEO)
													// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)
	
	0x07,0x05,0x86,0x05,0x00,0x0B,0x01,	 // Standard VS isochronous video data endpoint descriptor
										   // Descriptor type:   0x05 (ENDPOINT)
										   // EndpointAddress:   0x82 ==(IN endpoint 2)
										   // Attributes:        0x05 (Isochronous transfer type)
	
	0x09,0x04,0x01,0x05,0x01,0x0E,0x02,0x00,0x00,	//Standard VS interface Descriptor (Alternate Setting 1)
													// Descriptor type:   INTERFACE
													// InterfaceNumber:   0x01 (index of this interface)
													// AlternateSetting:  0x05 (index of this Alternate Setting)
													// NumEndpoint:       0x01
													// InterfaceClass:    0x0E (CC_VIDEO)
													// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)
	
	0x07,0x05,0x86,0x05,0x00,0x0C,0x01,	  // Standard VS isochronous video data endpoint descriptor
										   // Descriptor type:   0x05 (ENDPOINT)
										   // EndpointAddress:   0x82 ==(IN endpoint 2)
										   // Attributes:        0x05 (Isochronous transfer type)
	
	0x09,0x04,0x01,0x06,0x01,0x0E,0x02,0x00,0x00,  //Standard VS interface Descriptor (Alternate Setting 1)
													// Descriptor type:   INTERFACE
													// InterfaceNumber:   0x01 (index of this interface)
													// AlternateSetting:  0x06 (index of this Alternate Setting)
													// NumEndpoint:       0x01
													// InterfaceClass:    0x0E (CC_VIDEO)
													// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)
	
	0x07,0x05,0x86,0x05,0x80,0x13,0x01,	 // Standard VS isochronous video data endpoint descriptor
										   // Descriptor type:   0x05 (ENDPOINT)
										   // EndpointAddress:   0x82 ==(IN endpoint 2)
										   // Attributes:        0x05 (Isochronous transfer type)
	
	0x09,0x04,0x01,0x07,0x01,0x0E,0x02,0x00,0x00,  //Standard VS interface Descriptor (Alternate Setting 1)
													// Descriptor type:   INTERFACE
													// InterfaceNumber:   0x01 (index of this interface)
													// AlternateSetting:  0x07 (index of this Alternate Setting)
													// NumEndpoint:       0x01
													// InterfaceClass:    0x0E (CC_VIDEO)
													// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)
	
	0x07,0x05,0x86,0x05,0x00,0x14,0x01,	 // Standard VS isochronous video data endpoint descriptor
										   // Descriptor type:   0x05 (ENDPOINT)
										   // EndpointAddress:   0x82 ==(IN endpoint 2)
										   // Attributes:        0x05 (Isochronous transfer type)

	

 // Audio
	0x08,0x0B,0x02,0x02,0x01,0x02,0x00,0x03,  //  Descriptor type:   INTERFACE
											   // InterfaceNumber:   0x02	(Index of this interface)
											   // AlternateSetting:  0x00
	
	0x09,0x04,0x02,0x00,0x00,0x01,0x01,0x00,0x03,	// USB Microphone Standard AC Interface descriptor
	                                                 // Descriptor type:   INTERFACE
													 // InterfaceNumber:   0x02	(Index of this interface)
													 // AlternateSetting:  0x00
													 // InterfaceClass:    0x01 (Audio) 
													 // InterfaceSubClass: 0x01 (AUDIO_CINTROL)
													 // NumEndpoint:	   0x00	
	
	0x09,0x24,0x01,0x00,0x01,0x27,0x00,0x01	,0x03,	// USB Microphone Class-specific AC Interface Descriptor
												     // Descriptor type:               CS_INTERFACE
													 // Descriptor subtype:            HEADER
													 // Number of Streaming Interface: 0x01
												    
	
	0x0C,0x24,0x02,0x01,0x01,0x02,0x00,0x01,0x00,0x00,0x00,0x00,// USB Microphone Input Terminal Descriptor
	                                                             //	Descriptor type:       0x24 (CS_INTERFACE)
													             // Descriptor subtype:    0x02 (INPUT_TERMINAL)
																 // TerminalType:		   0x0201 (Microphone)
																 // TerminalId:			   0x01
	
	0x09,0x24,0x06,0x02,0x01,0x01,0x00,0x03,0x00,	// FEATURE_UNIT	Descriptor
	                                                 // Descriptor type:       0x24 (CS_INTERFACE)
													 // Descriptor subtype:    0x06 (FEATURE_UNIT)
													 // UnitId:				   0x02
													

	
	0x09,0x24,0x03,0x03,0x01,0x01,0x01,0x02,0x00, // USB Microphone Output Terminal Descriptor
	                                                // Descriptor type:       0x24 (CS_INTERFACE)
													// Descriptor subtype:    0x03 (OUTPUT_TERMINAL)
													// TerminalId:			  0x03
													
	
	0x09,0x04,0x03,0x00,0x00,0x01,0x02,0x00,0x00,   // USB Microphone Standard AS Interface Descriptor (Alt. Set. 0) 
	                                                 // Descriptor type:   INTERFACE
													 // InterfaceNumber:   0x03	(Index of this interface)
													 // AlternateSetting:  0x00
													 // InterfaceClass:    0x01 (AUDIO) 
													 // InterfaceSubClass: 0x02 (AUDIO_STREAMING)
													 // NumEndpoint:	   0x00 
	
	0x09,0x04,0x03,0x01,0x01,0x01,0x02,0x00,0x00, // USB Microphone Standard AS Interface Descriptor (Alt. Set. 1) 
	                                                // Descriptor type:   INTERFACE
													// InterfaceNumber:   0x03	(Index of this interface)
													// AlternateSetting:  0x01
													// InterfaceClass:    0x01 (AUDIO) 
													// InterfaceSubClass: 0x02 (AUDIO_STREAMING)
													// NumEndpoint:	   0x01
	
	0x07,0x24,0x01,0x03,0x01,0x01,0x00,			   // USB Microphone Class-specific AS General Interface Descriptor
	                                                // Descriptor type:   0x24 (CS_INTERFACE)
													// DescriptorSubType: 0x01 (GENERAL)
												
	
	0x1D,0x24,0x02,0x01,0x01,0x02,0x10,0x07,0x40,0x1F,0x00,0x11,0x2B,0x00,0x80,	 // USB Microphone Type I Format Type Descriptor
	0x3E,0x00,0x22,0x56,0x00,0x00,0x7D,0x00,0x44,0xAC,0x00,0x80,0xBB,0x00,          // Descriptor type:   0x24 (CS_INTERFACE)
													                                // DescriptorSubType: 0x02 (FORMAT_TYPE_I)

	
	
	0x09,0x05,0x85,0x01,0x62,0x00,0x04,0x00,0x00,  // USB Microphone Standard Endpoint Descriptor 
	                                                // Descriptor type:   Endpoint
													// EndpointAdress:    0x83== (IN endpoint 3)
													// Attributes:        0x01 (iso)
	
	0x07,0x25,0x01,0x01,0x00,0x00,0x00				// Descriptor type:    CS_Endpoint
													// DescriptorSubType:  0x01 (GENERAL)
													
};
 
 