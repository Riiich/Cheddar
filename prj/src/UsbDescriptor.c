#include "platform.h"

// Device Descriptor
code unsigned char device_desc[] = {
	0x12,	//	Length = 18 (0x12)
	0x01,	//	Device Descriptor Type= 1
	0x00,	//	bcdUSB Version L (.00) 
	0x02,	//	bcdUSB Version H (2.)
	0xEF,	//	Device Class (EF) Miscellaneous Device Class
	0x02,	//	Device Sub Class (02) Common Class
	0x01,	//	Device Protocol (01) Interface association descriptor ;EF 02 01 for Multi-Function Class code  Bruce
	0x40,	//	Max Packet Size for EP0 = 64
	0xBA,	//	Vendor ID Low 
	0x05,	//	Vendor ID High (0x5BA)
	0x0B,	//	Product ID Low
	0x00,	//	Product ID High (0x000B)
	0x01,	//	bcdDevice Revision Low
	0x01,	//	bcdDevice Revision High (0x0101)
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
/*
	"If the device is attached to a USB 1.1 hub, is operating at Full-Speed, and its USB
	Device Descriptor bcdUSB field is greater than or equal to 0x200, 
	the hub driver will issue a GET_DESCRIPTOR for descriptor type DEVICE_QUALIFIER (6).  
	The successful completion of the request indicates the device can support USB 2.0 high-speed operation."

	STALL if we don't expect device being connecting to USB 1.1 Hub
*/
code unsigned char Ide_Dvc_QF_D[] = {
    0x0A,
	0x06,0x00,0x02,0x00,0x00,0x00,0x40,0x01,0x00
};


/////////////////////////////////////////////
// UVC configuration Descriptor
code unsigned char UVC_Std_D[] = {

#ifdef OV9155
	//****Configuration descriptor
	0x09,		// Length = 9 
	0x02,		// Configuration Descriptor = 2
	0x55,		// Total length of Configuration_Descriptor (L)
	0x01,		// Total length of Configuration_Descriptor (H) 0x0155 = 341
	0x04,		// Number of Interfaces = 4
	0x01,		// Configuration Number = 1
	0x00,		// Configuration String Index = 0 (No Descriptor)
	0xA0,		// Attributes. D7=1, D6=Self Power, D5=Remote Wakeup (Why NOT???), [D4:D0]=0
	0xFA,		// Max Power in 2mA unite (0xFA=250, 500mA)
	
	//======================================================
	// Descriptor type:  0x0B Standard Video Interface Collection IAD (INTERFACE ASSOCIATION DESCRIPTOR)
	// FunctionClass:    0x0E (CC_VIDEO)
	// FunctionSubClass: 0x03 (SC_VIDEO_INTERFACE_COLLECTION)
	// ------------------------------------------------------
	0x08,		// bLength - Size of this descriptor, in bytes: 8
	0x0B,		// bDescriptorType - INTERFACE ASSOCIATION Descriptor.
	0x00,		// bFirstInterface - Interface number of the first VideoControl interface that is associated with this function.
	0x02,		// bInterfaceCount - Number of contiguous VideoStreaming interfaces that are associated with this function.
	0x0E,		// bFunctionClass - CC_VIDEO. Video Interface Class code (assigned by the USB).
	0x03,		// bFunctionSubClass - SC_VIDEO_INTERFACE_COLLECTION. Video Interface Subclass code.
	0x00,		// bFunctionProtocol - Not used. Must be set to PC_PROTOCOL_UNDEFINED.
	0x02,		// iFunction - Index of a string descriptor that describes this interface.		   

	// Standard VC interface descriptor 
	// Descriptor type:   INTERFACE
	// InterfaceNumber:   0x00	(Index of this interface)
	// AlternateSetting:  0x00
	// InterfaceClass:    0x0E (CC_VIDEO) 
	// InterfaceSubClass: 0x01 (SC_VIDEOCONTROL)
	// NumEndpoint:	      0x01 (1 interrupt endpoint)
	// ----------------------------------------------
	0x09,		// bLength - Size of this descriptor, in bytes: 9
	0x04,		// bDescriptorType - INTERFACE descriptor type
	0x00,		// bInterfaceNumber - Number of interface. A zero-based value identifying the index in the array of concurrent interfaces supported by this configuration.
	0x00,		// bAlternateSetting - Value used to select an alternate setting for the interface identified in the prior field.
	0x01,		// bNumEndpoints - Number of endpoints used by this interface (excluding endpoint 0). This number is 0 or 1.
	0x0E,		// bInterfaceClass - CC_VIDEO. Video Interface Class code (assigned by the USB).
	0x01,		// bInterfaceSubClass - SC_VIDEOCONTROL. Video Interface Subclass code. 
	0x00,		// Must be set to PC_PROTOCOL_15.
	0x02,		// Index of a string descriptor that describes this interface.
	// Class-specific VC interface descriptor
	// Descriptor type:               CS_INTERFACE
	// Descriptor subtype:            VC_HEADER
	// Number of streaming interface: 0x01
	// ----------------------------------------------
	0x0D,		// bLength - Size of this descriptor, in bytes: 12+n
	0x24,		// bDescriptorType - CS_INTERFACE descriptor type
	0x01,		// bDescriptorSubType- VC_HEADER descriptor subtype

	0x00,		// bcdUVC - Byte 1 - Video Device Class Specification release number in binary-coded decimal. 
	0x01,		// bcdUVC - Byte 2 - Video Device Class Specification release number in binary-coded decimal. 
	0x4F,		// wTotalLength - Byte 1 - Total number of bytes returned for the class-specific VideoControl interface descriptor.
	0x00,		// wTotalLength - Byte 2 - Total number of bytes returned for the class-specific VideoControl interface descriptor.
	0x80,		// dwClockFrequency Byte 1 - ?? Use of this field has been deprecated. The device clock frequency in Hz.
	0xC3,		// dwClockFrequency Byte 2 - Use of this field has been deprecated.
	0xC9,		// dwClockFrequency Byte 3 - Use of this field has been deprecated.
	0x01,		// dwClockFrequency Byte 4 - Use of this field has been deprecated.
	0x01,		// bInCollection - The number of VideoStreaming interfaces in the Video Interface Collection to which this VideoControl interface belongs: n
	0x01,		// baInterfaceNr(1) - Interface number of the first VideoStreaming interface in the Collection


	// Input Terminal descriptor,
	//  Descriptor type:     CS_INTERFACE,
	//  Descriptor subtype:  VC_INPUT_TERMINAL type
	//  ID of this terminal: 1
	//  Terminal type:       ITT_CAMERA	 
	// ----------------------------------------------
	0x12,		// bLength - Size of this descriptor, in bytes: 8 (+x)
	0x24,		// bDescriptorType - CS_INTERFACE descriptor type
	0x02,		// bDescriptorSubtype - VC_INPUT_TERMINAL descriptor subtype
	0x01,		// bTerminalID - A non-zero constant that uniquely identifies the Terminal within the video function.
	0x01,		// wTerminalType - Byte 1 Constant that characterizes the type of Terminal. This is set to the ITT_CAMERA value.
	0x02,		// wTerminalType - Byte 2 Constant that characterizes the type of Terminal. This is set to the ITT_CAMERA value.
	0x00,		// bAssocTerminal - ID of the Output Terminal to which this Input Terminal is associated.
	0x00,		// iTerminal - Index of a string descriptor that describes the Camera Terminal.
	0x00,		// Depending on the Terminal type, certain Input 
	0x00,		// Terminal descriptors have additional fields.    
	0x00,		// The descriptors for these special 
	0x00,		// Terminal types are described in 
	0x00,		// separate sections specific to 
	0x00,		// those Terminals, 
	0x03,		// and in 
	0x0A,		// accompanying 
	0x00,		// documents.
	0x00,		// 	     

	// Processing Unit
	// Descriptor type:    CS_INTERFACE
	// Descriptor subtype: VC_PROCESSING_UNIT
	// ID ofthis unit:     2
	// ----------------------------------------------
	0x0B,		// bLength - Size of this descriptor, in bytes: 9 (+x)
	0x24,		// bDescriptorType - CS_INTERFACE descriptor type
	0x05,		// bDescriptorSubType- VC_PROCESSING_UNIT descriptor subtype
	0x02,		// bUnitID - A non-zero constant that uniquely identifies the Terminal within the video function.
	0x01,		// bSourceID - ID of the Unit or Terminal to which this Unit is connected.
	0x00,		// wMaxMultiplier - Byte 1 If the Digital Multiplier control is supported, this field indicates the maximum digital magnification, multiplied by 100.
	0x00,		// wMaxMultiplier - Byte 2 If the Digital Multiplier control is supported, this field indicates the maximum digital magnification, multiplied by 100.
	0x02,		// bControlSize - Size of the bmControls field, in bytes: 3
	0x7F,		// bmControls - A bit set to 1 indicates that the mentioned Control is supported for the video stream.
	0x17,		// iProcessing - Index of a string descriptor that describes this processing unit.
	0x00,	  // bmVideoStandards - A bitmap of all analog video standards supported by the Processing Unit.

	// Output Terminal descriptor, 
	// Descriptor type:     CS_INTERFACE
	// Descriptor subtype:  VC_OUTPUT_TERMINAL type
	// ID ofthis unit:      3
	// Terminal type:       TT_STREAMING
	// ----------------------------------------------
	0x09,	  // bLength - Size of this descriptor, in bytes: 18
	0x24,	  // bDescriptorType - CS_INTERFACE descriptor type
	0x03,	  // bDescriptorSubtype - VC_OUTPUT_TERMINAL descriptor subtype
	0x03,	  // bTerminalID - A non-zero constant that uniquely identifies the Terminal within the video function.
	0x01,	  // wTerminalType - Byte 1 Constant that characterizes the type of Terminal.
	0x01,	  // wTerminalType - Byte 2 Constant that characterizes the type of Terminal.
	0x00,	  // bAssocTerminal - Constant, identifying the Input Terminal to which this Output Terminal is associated, or zero (0) if no such association exists.
	0x02,	  // bSourceID - ID of the Unit or Terminal to which this Terminal is connected.
	0x00,	  // iTerminal - Index of a string descriptor, describing the Output Terminal.   

	// Extension Unit descriptor
	// Descriptor type:    CS_INTERFACE
	// Descriptor subtype: VC_EXTENSION_UNIT
	// ----------------------------------------------
	0x1C,	  // bLength - Size of this descriptor, in bytes: 24+p+n
	0x24,	  // bDescriptorType - CS_INTERFACE descriptor type
	0x06,	  // bDescriptorSubtype - VC_EXTENSION_UNIT descriptor subtype
	0x04,	  // bUnitID - A non-zero constant that uniquely identifies the Unit within the video function.
	0x5D,	  // guidExtensionCode -  GUIDE Byte 1
	0xC7,	  // guidExtensionCode -  GUIDE Byte 2
	0x17,	  // guidExtensionCode -  GUIDE Byte 3
	0xA9,	  // guidExtensionCode -  GUIDE Byte 4
	0x19,	  // guidExtensionCode -  GUIDE Byte 5
	0x41,	  // guidExtensionCode -  GUIDE Byte 6
	0xDA,	  // guidExtensionCode -  GUIDE Byte 7
	0x11,	  // guidExtensionCode -  GUIDE Byte 8
	0xAE,	  // guidExtensionCode -  GUIDE Byte 9
	0x0E,	  // guidExtensionCode -  GUIDE Byte 10
	0x00,	  // guidExtensionCode -  GUIDE Byte 11
	0x0D,	  // guidExtensionCode -  GUIDE Byte 12
	0x56,	  // guidExtensionCode -  GUIDE Byte 13
	0xAC,	  // guidExtensionCode -  GUIDE Byte 14
	0x7B,	  // guidExtensionCode -  GUIDE Byte 15
	0x4C,	  // guidExtensionCode -  GUIDE Byte 16
	0x08,	  // bNumControls - Number of controls in this extension unit
	0x01,	  // bNrInPins -Number of Input Pins of this Unit: p
	0x01,	  // baSourceID(1) ID of the Unit or Terminal to which the first Input Pin of this Extension Unit is connected.
	0x03,	  // bControlSize - Size of the bmControls field, in bytes: n
	0x59,	  // bmControls - A bit set to 1 indicates that the mentioned Control is supported: D(n*8-1)..0: Vendor-specific
	0x80,	  // bmControls - A bit set to 1 indicates that the mentioned Control is supported: D(n*8-1)..0: Vendor-specific
	0x80,	  // bmControls - A bit set to 1 indicates that the mentioned Control is supported: D(n*8-1)..0: Vendor-specific
	0x00,   // iExtension - Index of a string descriptor that describes this extension unit.



	// Standard Interrupt Endpoint Descriptor
	// Descriptor type:  ENDPOINT
	// Endpoint address: 0x84== (IN endpoint 4)
	// bmAttributes:     0x03 (Interrupt)
	// ----------------------------------------------
	0x07,   // bLength - Size of this descriptor, in bytes: 7
	0x05,   // bDescriptorType - ENDPOINT descriptor type
	0x84,   // bEndpointAddress - The address of the endpoint on the USB device described by this descriptor. 
	// The address is encoded as follows:
	//	D7: Direction. 1 = IN endpoint
	//  D6..4: Reserved, reset to zero.
	//  D3..0: The endpoint number, determined by the designer.
	0x03,   // bmAttributes - D3..2: Synchronization type. Must be set to 00 (None) 
	// 								D1..0: Transfer type. Must be set to 11 (Interrupt). 
	//								All other bits are reserved, and must be set to zero.
	0x0A,   // wMaxPacketSize - Byte 1 Maximum packet size this endpoint is capable of sending or receiving when this configuration is selected.
	0x00,   // wMaxPacketSize - Byte 1 Maximum packet size this endpoint is capable of sending or receiving when this configuration is selected.
	0x05,	  // bInterval  - 		Interval for polling endpoint for data transfers.   

	//Class-specific interrupt endpoint Descriptor
	// Descriptor type:    CS_ENDPOINT
	// Descriptor subtype: EP_INTERRUPT
	// ----------------------------------------------	
	0x05,   // bLength - Size of this descriptor, in bytes: 5
	0x25,   // bDescriptorType - CS_ENDPOINT descriptor type
	0x03,   // bDescriptorSubType - EP_INTERRUPT descriptor type
	0x0A,   // wMaxTransferSize - Byte 1 - Maximum interrupt structure size this endpoint is capable of sending.
	0x00,	  // wMaxTransferSize - Byte 2 - Maximum interrupt structure size this endpoint is capable of sending.



	// Standard VS interface Descriptor (Alternate Setting 0)
	// Descriptor type:   INTERFACE
	// InterfaceNumber:   0x01 (index of this interface)
	// AlternateSetting:  0x00 (index of this Alternate Setting)
	// NumEndpoint:       0x00
	// InterfaceClass:    0x0E (CC_VIDEO)
	// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)
	// ----------------------------------------------
	0x09,   // bLength - Size of this descriptor, in bytes: 9
	0x04,   // bDescriptorType - INTERFACE descriptor type
	0x01,   // bInterfaceNumber - Number of the interface.
	0x00,   // bAlternateSetting - Value used to select this alternate setting for the interface identified in the prior field.
	0x00,   // bNumEndpoints - Number of endpoints used by this interface (excluding endpoint 0).
	0x0E,   // bInterfaceClass - CC_VIDEO. Video Interface Class code (assigned by the USB).
	0x02,   // bInterfaceSubClass - SC_VIDEOSTREAMING. Video interface subclass code
	0x00,   // bInterfaceProtocol - Must be set to PC_PROTOCOL_15.
	0x00,   // iInterface - Index of a string descriptor that describes this interface.


	// Class-specific VS header descriptor
	// Descriptor type:    0x24 (CS_INTERFACE)
	// Descriptor subtype: 0x01 (VS_INPUT_HEADER)
	// EndpointAddress:    0x82 ==(Address of the iso endpoint)
	// StillCaptureMethod: 0x02 (Method 2)
	// ----------------------------------------------
	0x0E,   // bLength - Size of this descriptor, in bytes: 13+(p*n).
	0x24,   // bDescriptorType - CS_INTERFACE descriptor type
	0x01,   // bDescriptorSubtype - VS_INPUT_HEADER descriptor subtype
	0x01,   // bNumFormats - Number of video payload Format descriptors following for this interface
	0x67,   // wTotalLength - Byte 1 - Total number of bytes returned for the class-specific VideoStreaming interface descriptors including this header descriptor.
	0x00,   // wTotalLength - Byte 2 - Total number of bytes returned for the class-specific VideoStreaming interface descriptors including this header descriptor.
	0x86,   // bEndpointAddress - The address of the isochronous or bulk endpoint used for video data.
	0x00,   // bmInfo - Indicates the capabilities of this VideoStreaming interface
	0x03,   // bTerminalLink - The terminal ID of the Output Terminal to which the video endpoint of this interface is connected.
	0x02,   // bStillCaptureMethod - Method of still image capture supported as described in section 2.4.2.4,
	0x01,   // bTriggerSupport - Specifies if hardware triggering is supported through this interface
	0x01,   // bTriggerUsage - Specifies how the host software shall respond to a hardware trigger interrupt event from this interface.
	0x01,   // bControlSize - Size of each bmaControls(x) field, in bytes: 1
	0x00,   // bmaControls(1) - For bits D3..0, a bit set to 1 indicates that the named field is supported by the Video Probe and Commit Control when bFormatIndex is 1

	// Descriptor type:    0x24 (CS_INTERFACE)	Class-specific VS Format Descriptor 
	// Descriptor subtype: 0x04 (VS_FORMAT_UNCOMPRESSED)															
	// NumFrameDescriptor: 0x05 (5 frams descriptors)
	// ----------------------------------------------
	0x1B, // Size of this descriptor
	0x24, // CS_INTERFACE
	0x04, // bDescriptorSubtype VS_FORMAT_UNCOMPRESSED
	0x01, // bFormatIndex 
	0x01, // bNumFrameDescriptors 
	0x59, // guidFormat YUY2
	0x55, //
	0x59, //
	0x32, //
	0x00, //
	0x00, //
	0x10, //
	0x00, //
	0x80, //
	0x00, //
	0x00, //
	0xAA, //
	0x00, //
	0x38, //
	0x9B, //
	0x71, //
	0x10, // bBitsPerPixel 
	0x01, // bDefaultFrameIndex 
	0x00, // bAspectRatioX 
	0x00, // bAspectRatioY
	0x00, // bmInterlaceFlags 
	0x00, // bCopyProtect 

	// Descriptor type:    0x24 (CS_INTERFACE)	Class-specific VS frame descriptor
	// used to describe the decoded video and still-image frame dimensions and other frame-specific characteristics
	0x1E,0x24,0x05,0x01,0x00,0x00,0x05,0x00,0x04,0x00,0x00,0xB8,0x0B,0x00,0x00,	 // Class-specific VS frame descriptor
	0xB8,0x0B,0x00,0x00,0x28,0x00,0x15,0x16,0x05,0x00,0x01,0x15,0x16,0x05,0x00,  // Descriptor type:    0x24 (CS_INTERFACE)
	// Descriptor subtype: 0x05 (VS_FRAME_UNCOMPRESSED)
	// 1st frame descriptor

	/*	0x1E,0x24,0x05,0x02,0x00,0x60,0x01,0x20,0x01,0x00,0x00,0xB8,0x0B,0x00,0x00,	 // Class-specific VS frame descriptor
	0xB8,0x0B,0x00,0x18,0x03,0x00,0x15,0x16,0x05,0x00,0x01,0x15,0x16,0x05,0x00,	 // Descriptor type:    0x24 (CS_INTERFACE)
	// Descriptor subtype: 0x05 (VS_FRAME_UNCOMPRESSED)
	// 2nd frame descriptor

	0x1E,0x24,0x05,0x03,0x00,0x40,0x01,0xF0,0x00,0x00,0x00,0xB8,0x0B,0x00,0x00,	 // Class-specific VS frame descriptor
	0xB8,0x0B,0x00,0x58,0x02,0x00,0x15,0x16,0x05,0x00,0x01,0x15,0x16,0x05,0x00,	 // Descriptor type:    0x24 (CS_INTERFACE)
	// Descriptor subtype: 0x05 (VS_FRAME_UNCOMPRESSED)
	// 3rd frame descriptor

	0x1E,0x24,0x05,0x04,0x00,0xB0,0x00,0x90,0x00,0x00,0x00,0xB8,0x0B,0x00,0x00,	 //	  Class-specific VS frame descriptor
	0xB8,0x0B,0x00,0xC6,0x00,0x00,0x15,0x16,0x05,0x00,0x01,0x15,0x16,0x05,0x00,	 // Descriptor type:    0x24 (CS_INTERFACE)
	// Descriptor subtype: 0x05 (VS_FRAME_UNCOMPRESSED)
	// 4th frame descriptor

	0x1E,0x24,0x05,0x05,0x00,0xA0,0x00,0x78,0x00,0x00,0x00,0xB8,0x0B,0x00,0x00, 	// Class-specific VS frame descriptor
	0xB8,0x0B,0x00,0x96,0x00,0x00,0x15,0x16,0x05,0x00,0x01,0x15,0x16,0x05,0x00,	  //	Descriptor type:    0x24 (CS_INTERFACE)
	*/																			                                          // Descriptor subtype: 0x05 (VS_FRAME_UNCOMPRESSED)
	// 5th frame descriptor
	// Class-specific Still Image descriptor
	// Descriptor type:    0x24 (CS_INTERFACE)
	// Descriptor subtype: 0x03 (VS_STILL_IMAGE_FRAME)
	// EndpointAdress:     0x00 (still image capture method 2 set to zero)
	// ----------------------------------------------
	0x1A,   // bLength - Size of this descriptor, in bytes: 10+(4*n)-4+m
	0x24,   // bDescriptorType - CS_INTERFACE descriptor type
	0x03,   // bDescriptorSubtype - VS_STILL_IMAGE_FRAME descriptor subtype
	0x00,   // bEndpointAddress - If method 3 of still image capture is used, this contains the address of the bulk endpoint used for still image capture.
	0x05,   // bNumImageSizePatterns - Number of Image Size patterns of this format: n
	0x00,   // Width of the still image in pattern 1 L
	0x05,   // Width of the still image in pattern 1 H  0x280=640
	0x00,   // Height of the still image in pattern 1 L
	0x04,   // Height of the still image in pattern 1 H 0x1E0=480
	0x60,   // Width of the still image in pattern 2 
	0x01,   //
	0x20,   // Height of the still image in pattern 2
	0x01,   //
	0x40,   // Width of the still image in pattern 3
	0x01,   // 
	0xF0,   // Height of the still image in pattern 3
	0x00,   // 
	0xB0,   // Width of the still image in pattern 4
	0x00,   // 
	0x90,   // Height of the still image in pattern 4
	0x00,   // 
	0xA0,   // Width of the still image in pattern 5
	0x00,   // 
	0x78,   // Height of the still image in pattern 5 
	0x00,   // 
	0x00,   // Compression of the still image in pattern 1



	// Descriptor type:    0x24 (CS_INTERFACE)	Class-specific color matching descriptor
	// Descriptor subtype: 0x0D (VS_COLORFORMAT)
	// Descriptor type:    0x24 (CS_INTERFACE)
	// Descriptor subtype: 0x0D (VS_COLORFORMAT)
	// ----------------------------------------------
	0x06, // bLength - constant 6
	0x24, // bDescriptorType - CS_INTERFACE type
	0x0D, // bDescriptorSubtype - VS_COLORFORMAT
	0x00, // bColorPrimaries BT.709, sRGB
	0x00, // bTransferCharacteristics BT.709, sRGB
	0x00, // bMatrixCoefficients used to compute luma and chroma values :SMPTE 170M (BT.601, default) 


	// Standard VS interface Descriptor (Alternate Setting 1)
	// Alternate setting 1 is the operational setting of the interface
	// Descriptor type:   INTERFACE
	// InterfaceNumber:   0x01 (index of this interface)
	// AlternateSetting:  0x01 (index of this Alternate Setting)
	// NumEndpoint:       0x01
	// InterfaceClass:    0x0E (CC_VIDEO)
	// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)
	// ----------------------------------------------
	0x09, // Size of this descriptor
	0x04, // INTERFACE descriptor type
	0x01, // Index of this interface
	0x01, // Index of this alternate setting 
	0x01, // 0 endpoints !V no bandwidth used
	0x0E, // bInterfaceClass CC_VIDEO
	0x02, // bInterfaceSubClass SC_VIDEOSTREAMING
	0x00, // bInterfaceProtocol  PC_PROTOCOL_UNDEFINED
	0x00, //iInterface no used 

	// Descriptor type:   0x05 (ENDPOINT)	Standard VS isochronous video data endpoint descriptor
	// Descriptor type:   0x05 (ENDPOINT)
	// EndpointAddress:   0x86== (IN endpoint 6)
	// Attributes:        0x05 (Isochronous transfer type)								
	// ----------------------------------------------
	0x07, // Size of this descriptor
	0x05, // ENDPOINT
	0x86, // IN endpoint 6
	0x05, // bmAttributes Isochronous transfer type
	0x80, // wMaxPacketSize L 0080
	0x00, // wMaxPacketSize H
	0x01, // bInterval 	  						


	0x09,0x04,0x01,0x02,0x01,0x0E,0x02,0x00,0x00,  //Standard VS interface Descriptor (Alternate Setting 2)
	// Descriptor type:   INTERFACE
	// InterfaceNumber:   0x01 (index of this interface)
	// AlternateSetting:  0x02 (index of this Alternate Setting)
	// NumEndpoint:       0x01
	// InterfaceClass:    0x0E (CC_VIDEO)
	// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)

	0x07,0x05,0x86,0x05,0x20,0x01,0x01,	 // Standard VS isochronous video data endpoint descriptor
	// Descriptor type:   0x05 (ENDPOINT)
	// EndpointAddress:   0x86== (IN endpoint 6)
	// Attributes:        0x05 (Isochronous transfer type)

	0x09,0x04,0x01,0x03,0x01,0x0E,0x02,0x00,0x00,  //Standard VS interface Descriptor (Alternate Setting 3)
	// Descriptor type:   INTERFACE
	// InterfaceNumber:   0x01 (index of this interface)
	// AlternateSetting:  0x03 (index of this Alternate Setting)
	// NumEndpoint:       0x01
	// InterfaceClass:    0x0E (CC_VIDEO)
	// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)

	0x07,0x05,0x86,0x05,0x80,0x01,0x01,	 // Standard VS isochronous video data endpoint descriptor
	// Descriptor type:   0x05 (ENDPOINT)
	// EndpointAddress:   0x86== (IN endpoint 6)
	// Attributes:        0x05 (Isochronous transfer type)

	0x09,0x04,0x01,0x04,0x01,0x0E,0x02,0x00,0x00,  //Standard VS interface Descriptor (Alternate Setting 4)
	// Descriptor type:   INTERFACE
	// InterfaceNumber:   0x01 (index of this interface)
	// AlternateSetting:  0x04 (index of this Alternate Setting)
	// NumEndpoint:       0x01
	// InterfaceClass:    0x0E (CC_VIDEO)
	// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)

	0x07,0x05,0x86,0x05,0x00,0x02,0x01,	 // Standard VS isochronous video data endpoint descriptor
	// Descriptor type:   0x05 (ENDPOINT)
	// EndpointAddress:   0x86 ==(IN endpoint 6)
	// Attributes:        0x05 (Isochronous transfer type)

	0x09,0x04,0x01,0x05,0x01,0x0E,0x02,0x00,0x00,	//Standard VS interface Descriptor (Alternate Setting 5)
	// Descriptor type:   INTERFACE
	// InterfaceNumber:   0x01 (index of this interface)
	// AlternateSetting:  0x05 (index of this Alternate Setting)
	// NumEndpoint:       0x01
	// InterfaceClass:    0x0E (CC_VIDEO)
	// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)

	0x07,0x05,0x86,0x05,0x00,0x03,0x01,	  // Standard VS isochronous video data endpoint descriptor
	// Descriptor type:   0x05 (ENDPOINT)
	// EndpointAddress:   0x86 ==(IN endpoint 6)
	// Attributes:        0x05 (Isochronous transfer type)

	0x09,0x04,0x01,0x06,0x01,0x0E,0x02,0x00,0x00,  //Standard VS interface Descriptor (Alternate Setting 6)
	// Descriptor type:   INTERFACE
	// InterfaceNumber:   0x01 (index of this interface)
	// AlternateSetting:  0x06 (index of this Alternate Setting)
	// NumEndpoint:       0x01
	// InterfaceClass:    0x0E (CC_VIDEO)
	// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)

	0x07,0x05,0x86,0x05,0x60,0x03,0x01,	 // Standard VS isochronous video data endpoint descriptor
	// Descriptor type:   0x05 (ENDPOINT)
	// EndpointAddress:   0x86 ==(IN endpoint 6)
	// Attributes:        0x05 (Isochronous transfer type)

	0x09,0x04,0x01,0x07,0x01,0x0E,0x02,0x00,0x00,  //Standard VS interface Descriptor (Alternate Setting 7)
	// Descriptor type:   INTERFACE
	// InterfaceNumber:   0x01 (index of this interface)
	// AlternateSetting:  0x07 (index of this Alternate Setting)
	// NumEndpoint:       0x01
	// InterfaceClass:    0x0E (CC_VIDEO)
	// InterfaceSubClass: 0x02 (SC_VIDEOSTREAMING)

	0x07,0x05,0x86,0x05,0x00,0x14,0x01,	 // Standard VS isochronous video data endpoint descriptor
	// Descriptor type:   0x05 (ENDPOINT)
	// EndpointAddress:   0x86 ==(IN endpoint 6)
	// Attributes:        0x05 (Isochronous transfer type)	


#else

	//****Configuration descriptor
	0x09,		// Length = 9 
	0x02,		// Configuration Descriptor = 2
	0x4B,		// Total length of Configuration_Descriptor (L)
	0x02,		// Total length of Configuration_Descriptor (H) 0x024B = 587
	0x04,		// Number of Interfaces = 4
	0x01,		// Configuration Number = 1
	0x00,		// Configuration String Index = 0 (No Descriptor)
	0xA0,		// Attributes. D7=1, D6=Self Power, D5=Remote Wakeup (Why NOT???), [D4:D0]=0
	0xFA,		// Max Power in 2mA unite (0xFA=250, 500mA)

	//****Standard Video Interface Collection IAD 
	// Descriptor type:	 0x0B INTERFACE ASSOCIATION DESCRIPTOR
	// FunctionClass: 0x0E (CC_VIDEO)
	// FunctionSubClass: 0x03 (SC_VIDEO_INTERFACE_COLLECTION)
	0x08,		// bLength - Size of this descriptor, in bytes: 8
	0x0B,		// bDescriptorType - INTERFACE ASSOCIATION Descriptor.
	0x00,		// bFirstInterface - Interface number of the first VideoControl interface that is associated with this function.
	0x02,		// bInterfaceCount - Number of contiguous VideoStreaming interfaces that are associated with this function.
	0x0E,		// bFunctionClass - CC_VIDEO. Video Interface Class code (assigned by the USB).
	0x03,		// bFunctionSubClass - SC_VIDEO_INTERFACE_COLLECTION. Video Interface Subclass code.
	0x00,		// bFunctionProtocol - Not used. Must be set to PC_PROTOCOL_UNDEFINED.
	0x02,		// iFunction - Index of a string descriptor that describes this interface.
	
	//****Standard VC Interface Descriptor
	// Descriptor type: 0x04 INTERFACE
	// InterfaceNumber: 0x00	(Index of this interface)
	// AlternateSetting: 0x00
	// InterfaceClass: 0x0E (CC_VIDEO) 
	// InterfaceSubClass: 0x01 (SC_VIDEOCONTROL)
	// NumEndpoint: 0x01 (1 interrupt endpoint)
	0x09,		// bLength - Size of this descriptor, in bytes: 9
	0x04,		// bDescriptorType - INTERFACE descriptor type
	0x00,		// bInterfaceNumber - Number of interface. A zero-based value identifying the index in the array of concurrent interfaces supported by this configuration.
	0x00,		// bAlternateSetting - Value used to select an alternate setting for the interface identified in the prior field.
	0x01,		// bNumEndpoints - Number of endpoints used by this interface (excluding endpoint 0). This number is 0 or 1.
	0x0E,		// bInterfaceClass - CC_VIDEO. Video Interface Class code (assigned by the USB).
	0x01,		// bInterfaceSubClass - SC_VIDEOCONTROL. Video Interface Subclass code. 
	0x00,		// Must be set to PC_PROTOCOL_15.
	0x02,		// Index of a string descriptor that describes this interface.

	// Class-specific VC Interface Descriptor
	// Descriptor type: CS_INTERFACE
	// Descriptor subtype: VC_HEADER
	// Number of streaming interface: 0x01
	0x0D,		// bLength - Size of this descriptor, in bytes: 12+n
	0x24,		// bDescriptorType - CS_INTERFACE descriptor type
	0x01,		// bDescriptorSubType- VC_HEADER descriptor subtype			
	0x00,		// bcdUVC - Byte 1 - Video Device Class Specification release number in binary-coded decimal. 
	0x01,		// bcdUVC - Byte 2 - Video Device Class Specification release number in binary-coded decimal. 
	0x4F,		// wTotalLength - Byte 1 - Total number of bytes returned for the class-specific VideoControl interface descriptor.
	0x00,		// wTotalLength - Byte 2 - Total number of bytes returned for the class-specific VideoControl interface descriptor.
	0x80,		// dwClockFrequency Byte 1 - Use of this field has been deprecated. The device clock frequency in Hz.
	0xC3,		// dwClockFrequency Byte 2 - Use of this field has been deprecated.
	0xC9,		// dwClockFrequency Byte 3 - Use of this field has been deprecated.
	0x01,		// dwClockFrequency Byte 4 - Use of this field has been deprecated.
	0x01,		// bInCollection - The number of VideoStreaming interfaces in the Video Interface Collection to which this VideoControl interface belongs: n
	0x01,		// baInterfaceNr(1) - Interface number of the first VideoStreaming interface in the Collection

	// Input Terminal Descriptor
	//  Descriptor type: CS_INTERFACE,
	//  Descriptor subtype: VC_INPUT_TERMINAL type
	//  ID of this terminal: 1
	//  Terminal type: ITT_CAMERA
	0x12,		// bLength - Size of this descriptor, in bytes: 8 (+x)
	0x24,		// bDescriptorType - CS_INTERFACE descriptor type
	0x02,		// bDescriptorSubtype - VC_INPUT_TERMINAL descriptor subtype
	0x01,		// bTerminalID - A non-zero constant that uniquely identifies the Terminal within the video function.
	0x01,		// wTerminalType - Byte 1 Constant that characterizes the type of Terminal. This is set to the ITT_CAMERA value.
	0x02,		// wTerminalType - Byte 2 Constant that characterizes the type of Terminal. This is set to the ITT_CAMERA value.
	0x00,		// bAssocTerminal - ID of the Output Terminal to which this Input Terminal is associated.
	0x00,		// iTerminal - Index of a string descriptor that describes the Camera Terminal.
	0x00,		// Depending on the Terminal type, certain Input 
	0x00,		// Terminal descriptors have additional fields.    
	0x00,		// The descriptors for these special 
	0x00,		// Terminal types are described in 
	0x00,		// separate sections specific to 
	0x00,		// those Terminals, 
	0x03,		// and in 
	0x0A,		// accompanying 
	0x00,		// documents.
	0x00,		//

	// Processing Unit	
	// Descriptor type: CS_INTERFACE
	// Descriptor subtype: VC_PROCESSING_UNIT
	// ID ofthis unit: 2
	0x0B,		// bLength - Size of this descriptor, in bytes: 9 (+x)
	0x24,		// bDescriptorType - CS_INTERFACE descriptor type
	0x05,		// bDescriptorSubType- VC_PROCESSING_UNIT descriptor subtype
	0x02,		// bUnitID - A non-zero constant that uniquely identifies the Terminal within the video function.
	0x01,		// bSourceID - ID of the Unit or Terminal to which this Unit is connected.
	0x00,		// wMaxMultiplier - Byte 1 If the Digital Multiplier control is supported, this field indicates the maximum digital magnification, multiplied by 100.
	0x00,		// wMaxMultiplier - Byte 2 If the Digital Multiplier control is supported, this field indicates the maximum digital magnification, multiplied by 100.
	0x02,		// bControlSize - Size of the bmControls field, in bytes: 3
	0x7F,		// bmControls - A bit set to 1 indicates that the mentioned Control is supported for the video stream.
	0x17,		// iProcessing - Index of a string descriptor that describes this processing unit.
	0x00,	  // bmVideoStandards - A bitmap of all analog video standards supported by the Processing Unit.

	// Output Terminal Descriptor
	// Descriptor type: CS_INTERFACE
	// Descriptor subtype: VC_OUTPUT_TERMINAL type
	// ID ofthis unit: 3
	// Terminal type: TT_STREAMING
	0x09,	  // bLength - Size of this descriptor, in bytes: 18
	0x24,	  // bDescriptorType - CS_INTERFACE descriptor type
	0x03,	  // bDescriptorSubtype - VC_OUTPUT_TERMINAL descriptor subtype
	0x03,	  // bTerminalID - A non-zero constant that uniquely identifies the Terminal within the video function.
	0x01,	  // wTerminalType - Byte 1 Constant that characterizes the type of Terminal.
	0x01,	  // wTerminalType - Byte 2 Constant that characterizes the type of Terminal.
	0x00,	  // bAssocTerminal - Constant, identifying the Input Terminal to which this Output Terminal is associated, or zero (0) if no such association exists.
	0x02,	  // bSourceID - ID of the Unit or Terminal to which this Terminal is connected.
	0x00,	  // iTerminal - Index of a string descriptor, describing the Output Terminal.
	
	
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
#endif
};
 
 

