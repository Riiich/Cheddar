
// USB return codes
#define	USB_STATE_CODE		(0x07) // one of the following
#define	USB_STALL			(0)
#define	USB_DONE			(1)
#define USB_CONTINUE_READ	(2)
#define USB_CONTINUE_WRITE	(3)
#define USB_ABORT			(4)


// bmRequestType: Bitfield Definition
#define		BMR_RECIPIENT				0x1F
#define		BMR_REC_DEVICE			0x00
#define		BMR_REC_INTERFACE		0x01
#define		BMR_REC_ENDPOINT		0x02
#define		BMR_REC_OTHER				0x03

// bRequest	Standard Request Codes
#define	BR_GET_STATUS					0
#define	BR_CLEAR_FEATURE			1
#define	BR_RESERVED_2					2
#define	BR_SET_FEATURE				3
#define BR_RESERVED_4					4
#define	BR_SET_ADDRESS				5
#define	BR_GET_DESCRIPTOR			6
#define	BR_SET_DESCRIPTOR			7
#define	BR_GET_CONFIGURATION	8
#define	BR_SET_CONFIGURATION	9
#define	BR_GET_INTERFACE			10
#define	BR_SET_INTERFACE			11
#define	BR_SYNCH_FRAME				12

// Feature Selectors
#define		FTR_ENDPOINT_HALT					0
#define		FTR_DEVICE_REMOTE_WAKEUP	1
#define		FTR_DEVICE_TEST_MODE			2 // SUPPORT in HIGH SPEED ONLY.

// Descriptor Types
#define		DS_DEVICE							1
#define		DS_CONFIGURATION			2
#define		DS_STRING							3
#define		DS_INTERFACE					4
#define		DS_ENDPOINT						5
#define		DS_DEVICE_QUALIFIER		6
#define		DS_OTHER_SPEED_CONFIG	7
#define		DS_INTERFACE_POWER		8

/*******************
				UVC
*******************/

 /* USB Video Class Request codes */
#define USB_UVC_RC_UNDEFINED		0x00
#define USB_UVC_SET_CUR					0x01
#define USB_UVC_GET_CUR					0x81
#define USB_UVC_GET_MIN					0x82
#define USB_UVC_GET_MAX					0x83
#define USB_UVC_GET_RES					0x84
#define USB_UVC_GET_LEN					0x85
#define USB_UVC_GET_INFO				0x86
#define USB_UVC_GET_DEF					0x87

 /* USB Video Class Request types */
#define UVCSetVideoControl      0x2100
#define UVCSetVideoStreaming    0x2200
#define UVCGetVideoControl      0xA100
#define UVCGetVideoStreaming    0xA200





/*
extern code unsigned char 
       Ide_Dvc_D[],
	   Ide_Dvc_D_USB3[],
       Ide_Dvc_D_2570[],
       Ide_Con_Std_D[],
       Ide_Con_Oth_D[],
       Ide_Int_HS_D[],
       Ide_Int_FS_D[],
       Ide_Int_HS_D_02[],
       Ide_Int_FS_D_02[],
       Ide_Int_HS_D_05[],
       Ide_Int_FS_D_05[],
       New_Ide_Str3_D[],
       Ide_Str0_D[],
       Ide_Str1_D[],
       Ide_Str2_D[],
       Ide_Str3_D[],
       Ide_Str3f_D[],
       Ide_Str3S_D[],
       Ide_MaxLUN_D[],
       Ide_Dvc_QF_D[],
       Ide_BOS_D[],
       CFReadInquiry[],
       UVC_Std_D[];
			 */
			 
#define UVC_Std_Dlen		587

#define USB3_Con_Std_Dlen	121
#define USB3_BOS_Std_Dlen	42
#define	Ide_Dvc_Dlen		18
#define	UVC_Std_D_len		587
#define	Ide_Con_Dlen		9
#define	Ide_Int_TDlen		23
#define Ide_Int_Dlen		9
#define	Ide_Enp_Dlen		14
#define	Ide_Enp_Offset		9
#define	Ide_Str0_Dlen		4
#define	Ide_Str1_Dlen		0x28
#define	Ide_Str2_Dlen		0x2C
#define	Ide_Str3_Dlen		0x04
#define	Ide_Str3f_Dlen		6
#define	Ide_Str3S_Dlen		38
#define	Ide_Dvc_QF_Dlen		10