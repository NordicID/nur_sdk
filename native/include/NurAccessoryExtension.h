/*
 * NurAccessoryExtension.h
 *
 *  Created on: Oct 5, 2016
 *      Author: mikko
 */

#ifndef _NURACCESSORYEXTENSION_H_
#define _NURACCESSORYEXTENSION_H_ 1

#include "NurAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup ACCESSORYAPI NUR Accessory device extension API.
 *  API for NUR accessory devices, such as EXA51
 *  @{
 */
	
/** Constant signature value used by the module and API extension. */
#define APP_CTX_SIG (0x21039807)

/** Byte size of the application context. */
#define APP_CTX_SIZE sizeof(NUR_ACC_CONFIG)

/** The notification number associated with accessory device events. First byte with notification NUR_NOTIFICATION_ACCESSORY is event type. */
#define NUR_NOTIFICATION_ACCESSORY		0x90

/** Barcode read event. First byte with notification NUR_NOTIFICATION_ACCESSORY is event type. */
#define NUR_ACC_EVENT_BARCODE			0x1

/** NUR_NOTIFICATION_IOCHANGE source is set to this when accessory device trigger is pressed/released. */
#define NUR_ACC_TRIGGER_SOURCE			100

/** A configuration (or any other class') value indicating that this integer value is either not initialized, not used or both. */
#define INTVALUE_NOT_VALID				-1
	
/** A configuration (or any other class') value indicating that this short value is either not initialized, not used or both. */
#define SHORTVALUE_NOT_VALID			-1

/** The protocol's value that the extension handler captures. */
#define NUR_CMD_ACC_EXT					0x55

/** BLE FW version. */
#define ACC_EXT_GET_FWVERSION			0

/** Get configuration command. */
#define ACC_EXT_GET_CFG					1

	/** Set configuration command. */
#define ACC_EXT_SET_CFG					2

/** System restart command. */
#define ACC_EXT_RESTART					5

/** Instruct the restart command to enter the DFU mode. */
#define RESET_BOOTLOADER_DFU_START	0xB1

/** Instruct the restart command to actually power off the device. */
#define RESET_POWEROFF				0xF1

/** Asynchronous barcode scan (non-blocking). */
#define ACC_EXT_READ_BARCODE_ASYNC	6
/** Set external LED. */
#define ACC_EXT_SET_LED_OP			7
/** Asynchronous beep operation. */
#define ACC_EXT_BEEP_ASYNC			8

/** Battery information ("extended information"). */
#define ACC_EXT_GET_BATT_INFO		9

/** Imager base Command */
#define ACC_EXT_IMAGER				13

/** Imager configuration Command */
#define ACC_EXT_IMAGER_CMD			4

/** Imager power on/off */
#define ACC_EXT_IMAGER_POWER		5

/** Imager aiming on/off */
#define ACC_EXT_IMAGER_AIM			6

/** Get HW status (imager, NUR module etc.). */
#define ACC_EXT_GET_HEALTHSTATE		11

/** Set/ get wireless charging. */
#define ACC_EXT_WIRELESS_CHARGE		12

/** Use device's vibra. */
#define ACC_EXT_VIBRATE				14

/** Clear device pairing information. */
#define ACC_EXT_CLEAR_PAIRS			15

/** Get connection info. */
#define ACC_EXT_GET_CONNECTION_INFO	18

/** Constant indicating battery level being "good". */
#define BATT_GOOD_mV		3900
/** Constant indicating battery level being "moderate". */
#define BATT_MODERATE_mV	3700
/** Constant indicating battery level being "low". Under this value the level is "critical". */
#define BATT_LOW_mV			3500	

/** Configuration flags. */
#define APP_FL_HID_BARCODE 		(1<<0)
#define APP_FL_HID_RFID 		(1<<1)
// Do not use imager central read - no "aiming" for imager.
#define APP_FL_NO_CENTRAL_READ	(1<<2)
//If set, no battery indication. led is GREEN constantly when power up.
#define APP_FL_NO_BATT_IND		(1<<3) 
#define APP_FL_ACDE				(1<<4)
#define APP_FL_USE_PEERMGR		(1<<5)

#pragma pack(push, 1)

/**
 * Nur accessory device (such as EXA51) configuration.
 * @sa NurAccGetConfig(), NurAccSetConfig()
 */
typedef struct __NUR_ACC_CONFIG {
	DWORD signature;				/**< Signature of config struct. Use value read (NurAccGetConfig) from device. */
	DWORD config;					/**< Device configuration flags. Currently NUR_ACC_CFG_ACD or NUR_ACC_CFG_WEARABLE */
	DWORD flags;					/**< Device operation flags. Currently used to enable HID modes. NUR_ACC_FL_HID_BARCODE and/or NUR_ACC_FL_HID_RFID */

	char device_name[32];			/**< NULL terminated string of device name. This used used as BLE visible name. */

    WORD hid_barcode_timeout;		/**< HID mode barcode scanner read timeout in milliseconds. */
    WORD hid_rfid_timeout;			/**< HID mode RFID read timeout in milliseconds. */
    WORD hid_rfid_maxtags;			/**< HID mode RFID read max tags. When max tags reached, reading will stop. */
} NUR_ACC_CONFIG;

/**
 * Nur accessory device (such as EXA51) battery information.
 * @sa NurAccGetBattInfo()
 */
typedef struct __NUR_ACC_BATT_INFO
{
	WORD flags;					/**< Battery flags. Currently 0 or NUR_ACC_BATT_FL_CHARGING */
	char percentage;			/**< Battery percentage level 0-100. -1 if unknown. */
	short volt_mV;				/**< Battery voltage level in mV. -1 if unknown. */
	short curr_mA;				/**< Current battery current draw in mA. */
	short cap_mA;				/**< Battery capacity in mAh. -1 if unknown. */
} NUR_ACC_BATT_INFO;

#pragma pack(pop)

/** NUR_ACC_BATT_INFO.flags value. If set device is currently charging. */
#define NUR_ACC_BATT_FL_CHARGING (1<<0)

/** NUR_ACC_CONFIG.flags value. HID-bit for the barcode scan. */
#define NUR_ACC_FL_HID_BARCODE 	(1<<0)
/** NUR_ACC_CONFIG.flags value. HID-bit for the RFID scan / inventory. */
#define NUR_ACC_FL_HID_RFID 	(1<<1)

/** NUR_ACC_CONFIG.config value. If set, device has ACD antenna. */
#define NUR_ACC_CFG_ACD 		(1<<0)
/** NUR_ACC_CONFIG.config value. If set, device has wearable antenna. */
#define NUR_ACC_CFG_WEARABLE 	(1<<1)

/**
 * Nur accessory device (such as EXA51) programmable LED operating mode.
 * @sa NurAccSetLedOpMode()
 */
enum NUR_ACC_LED_MODE {
	NUR_ACC_LED_UNSET = 0,	/**< Return device default mode. */
	NUR_ACC_LED_OFF,		/**< Programmable LED constantly off. */
	NUR_ACC_LED_ON,			/**< Programmable LED constantly on. */
	NUR_ACC_LED_BLINK		/**< Programmable LED constantly blinking. */
};

/**
* Wireless charging status
* @sa NurAccSetWirelessCharge()
*/
enum WIRELESS_CHARGE_STATUS
{	
	WIRELESS_CHARGE_OFF = 0,		/** The accessory stated that wireless charging is currently off. */	
	WIRELESS_CHARGE_ON = 1,		/** The accessory stated that wireless charging is currently on. */	
	WIRELESS_CHARGE_REFUSED = -1,	/** The accessory stated that wireless charging is currently not available. */	
	WIRELESS_CHARGE_FAIL = -2,		/** There was an unexpected error in the communications (no reply, timeout etc.). */	
	WIRELESS_CHARGE_NOT_SUPPORTED = -3	/** The accessory device replied with HW mismatch error. */
};

/**
* Imager type in used
* @sa NurAccImagerCmd()
*/
enum IMAGER_TYPE
{	
	IMAGER_OPTICON = 0		/** Opticon Imager. */		
};


/**
* HID modes of accessory device
* @sa NurAccSetHIDMode()
* @sa NurAccGetHIDMode()
*/
enum HID_MODE
{	
	HID_DISABLED = 0,			/** HID disabled. */	
	HID_ENABLE_BARCODE,			/** HID Barcode only */	
	HID_ENABLE_RFID,			/** HID RFID only */	
	HID_ENABLE_RFID_BARCODE		/** HID Barcode + RFID */		
};

/**
* Pairing modes of accessory device
* @sa NurAccSetPairingMode()
* @sa NurAccGetPairingMode()
*/
enum PAIRING_MODE
{	
	PAIRING_DISABLE = 0,	/** Pairing disabled */
	PAIRING_ENABLE			/** Pairing enabled. */				
};

/** @fn int NurAccGetFwVersion(HANDLE hApi, TCHAR *fwVer, int fwVerSizeChars)
 * Get accessory device firmware version.
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	fwVer			Pointer to a buffer that receives firmware version string.
 * @param	fwVerSizeChars	The size of <i>fwVer</i> buffer in TCHARs.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurAccGetFwVersion(HANDLE hApi, TCHAR *fwVer, int fwVerSizeChars);

/**
 * Returns detailed connection info from accessory device.
 *
 * @throws Exception Can throw exception if an error occurred int APi's transport.
 */

/** @fn int NurAccGetConnectionInfo(HANDLE hApi, TCHAR *fwVer, int fwVerSizeChars)
 * Get accessory connection info.
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	info			Pointer to a buffer that receives firmware version string.
 * @param	charSize	The size of <i>fwVer</i> buffer in TCHARs.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurAccGetConnectionInfo(HANDLE hApi, TCHAR *info, int charSize);


/** @fn int NurAccReadBarcodeAsync(HANDLE hApi, WORD timeoutInMs)
 * Start asynchronous barcode reading operation in device.
 * Result will be reported via notification callback, NUR_NOTIFICATION_ACCESSORY with first byte set to NUR_ACC_EVENT_BARCODE.
 *
 * If barcode reading succeeded, notification status is set to NUR_SUCCESS (0) and barcode result is stored in notification data starting from byte index 1.
 * If barcode reading is cancelled, notification status is set to NUR_ERROR_NOT_READY (13).
 * If barcode reader is not present, notification status is set to NUR_ERROR_HW_MISMATCH (8).
 *
 * NOTE: While barcode reading is active, it is not allowed to send any other command than NurAccCancelBarcode() to device.
 * NOTE2: Pressing device trigger will cancel barcode reading automatically.
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	timeoutInMs		Reading timeout in milliseconds. Range 500 - 20000
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @sa NurAccCancelBarcode(), NUR_NOTIFICATION_ACCESSORY, NUR_ACC_EVENT_BARCODE
 */
NUR_API
int NURAPICONV NurAccReadBarcodeAsync(HANDLE hApi, WORD timeoutInMs);

/** @fn int NurAccCancelBarcode(HANDLE hApi)
 * Cancel asynchronous barcode reading operation in device.
 *
 * If barcode reading is in progress, NUR_NOTIFICATION_ACCESSORY notification status is set to NUR_ERROR_NOT_READY (13).
 *
 * NOTE: While barcode reading is active, it is not allowed to send any other command than NurAccCancelBarcode() to device.
 * NOTE2: Pressing device trigger will cancel barcode reading automatically.
 *
 * @param	hApi			Handle to valid NurApi object instance.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @sa NurAccReadBarcodeAsync(), NUR_NOTIFICATION_ACCESSORY, NUR_ACC_EVENT_BARCODE
 */
NUR_API
int NURAPICONV NurAccCancelBarcode(HANDLE hApi);

/** @fn int NurAccVibrate(HANDLE hApi, WORD length_ms, BYTE nTimes)
* Use device's vibra.
* @param length_ms Vibration on  time in ms; pause in between will be the same.
* @param nTimes Number of times to repeat. Total time must not exceed 2000ms.
*
* @throws Exception Can throw exception if an error occurred int APi's transport.
 */
NUR_API
int NURAPICONV NurAccVibrate(HANDLE hApi, WORD length_ms, BYTE nTimes);

/** @fn int NurAccBeepAsync(HANDLE hApi, WORD timeoutInMs)
 * Generate beep sound in device.
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	timeoutInMs		Beep time in milliseconds. Range 1 - 5000
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurAccBeepAsync(HANDLE hApi, WORD timeoutInMs);

/** @fn int NurAccSetLedOpMode(HANDLE hApi, BYTE mode)
 * Set accessory device LED operating mode.
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	mode			Mode, one the enum NUR_ACC_LED_MODE. NUR_ACC_LED_UNSET, NUR_ACC_LED_OFF, NUR_ACC_LED_ON, NUR_ACC_LED_BLINK
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @sa enum NUR_ACC_LED_MODE
 */
NUR_API
int NURAPICONV NurAccSetLedOpMode(HANDLE hApi, BYTE mode);

/** @fn int NurAccRestart(HANDLE hApi)
 * Restart accessory device.
 *
 * @param	hApi			Handle to valid NurApi object instance.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurAccRestart(HANDLE hApi);

/** @fn int NurAccPowerOff(HANDLE hApi)
 * Power off accessory device.
 *
 * @param	hApi			Handle to valid NurApi object instance.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurAccPowerOff(HANDLE hApi);

/** @fn int NurAccGetConfig(HANDLE hApi, NUR_ACC_CONFIG *cfg, DWORD cfgSize)
 * Get accessory device configuration.
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	cfg				Pointer to config.
 * @param	cfgSize			Size of 'cfg' struct in bytes.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @sa NurAccSetConfig(), NUR_ACC_CONFIG
 */
NUR_API
int NURAPICONV NurAccGetConfig(HANDLE hApi, NUR_ACC_CONFIG *cfg, DWORD cfgSize);

/** @fn int NurAccSetConfig(HANDLE hApi, NUR_ACC_CONFIG *cfg, DWORD cfgSize)
 * Set accessory device configuration.
 * NOTE: First get configuration with NurAccGetConfig() function and then change needed params.
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	cfg				Pointer to new config.
 * @param	cfgSize			Size of 'cfg' struct in bytes.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @sa NurAccGetConfig(), NUR_ACC_CONFIG
 */
NUR_API
int NURAPICONV NurAccSetConfig(HANDLE hApi, NUR_ACC_CONFIG *cfg, DWORD cfgSize);

/** @fn int NurAccGetBattInfo(HANDLE hApi, NUR_ACC_BATT_INFO *info, DWORD infoSize)
 * Get accessory device battery information.
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	info			Pointer to info struct.
 * @param	infoSize		Size of 'info' struct in bytes.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @sa NUR_ACC_BATT_INFO
 */
NUR_API
int NURAPICONV NurAccGetBattInfo(HANDLE hApi, NUR_ACC_BATT_INFO *info, DWORD infoSize);

/** @fn int NurAccGetWirelessChargeStatus(HANDLE hApi,int *status)
* Get wireless charging status.
*
* @param	status	Pointer for variable to receive status information of charging.
* @return	Zero when succeeded, On error non-zero error code is returned. 
* @sa enum WIRELESS_CHARGE_STATUS  
* @throws Exception In case of any error in communication an exception is thrown.
*/
NUR_API
int NURAPICONV NurAccGetWirelessChargeStatus(HANDLE hApi,int *status);

/** @fn int NurAccSetWirelessCharging(HANDLE hApi,bool on, int *status)
 * Set the wireless charging on or off.
 *
 * @param	on		Set to true to turn on the wireless charging.
 * @param	status	Pointer for variable to receive status information of charging.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned. 
 * @sa enum WIRELESS_CHARGE_STATUS  
 */
NUR_API
int NURAPICONV NurAccSetWirelessCharge(HANDLE hApi,BOOL on, int *status);

/** @fn int NurAccClearPairingData(HANDLE hApi)
 * Clear remote device's pairing information.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned. 
 */
NUR_API
int NURAPICONV NurAccClearPairingData(HANDLE hApi);

/** @fn int NurAccGetHIDMode(HANDLE hApi,int *hidmode)
 * Get current HID mode @sa enum HID_MODE
 * @param	hidmode	Pointer for variable to receive HID mode.
 * @return	Zero when succeeded, On error non-zero error code is returned. 
 * @sa enum HID_MODE
 * @sa NurAccSetHIDMode()
 */
NUR_API
int NURAPICONV NurAccGetHIDMode(HANDLE hApi,int *hidmode);

/** @fn int NurAccSetHIDMode(HANDLE hApi,int hidmode)
 * Set current HID mode @sa enum HID_MODE
 * @param	hidmode	Pointer for variable to receive HID mode.
 * @return	Zero when succeeded, On error non-zero error code is returned. 
 * @sa enum HID_MODE
 * @sa NurAccGetHIDMode()
 */
NUR_API
int NURAPICONV NurAccSetHIDMode(HANDLE hApi,int hidmode);

/** @fn int NurAccGetPairingMode(HANDLE hApi,int *mode)
 * Get current pairing mode @sa enum PAIRING_MODE
 * @param	mode	Pointer for variable to receive pairing mode.
 * @return	Zero when succeeded, On error non-zero error code is returned. 
 * @sa enum PAIRING_MODE
 * @sa NurAccSetPairingMode()
 */
NUR_API
int NURAPICONV NurAccGetPairingMode(HANDLE hApi,int *mode);

/** @fn int NurAccSetPairingMode(HANDLE hApi,int mode)
 * Set current HID mode @sa enum PAIRING_MODE
 * @param	mode	Pointer for variable to receive HID mode.
 * @return	Zero when succeeded, On error non-zero error code is returned. 
 * @sa enum PAIRING_MODE
 * @sa NurAccGetPairingMode()
 */
NUR_API
int NURAPICONV NurAccSetPairingMode(HANDLE hApi,int mode);

/** @fn int NurAccImagerAIM(BOOL aim)
* Imager aimer on/off.
*
* @param aim true=aiming on false=aiming off
*
* @throws Exception Can throw I/O, timeout or API related exception based on the occurred error.
*/
NUR_API
int NURAPICONV NurAccImagerAIM(HANDLE hApi,BOOL aim);

/** @fn int NurAccImagerCmd(HANDLE hApi,char *cmd, int cmdLen,int imagerType, BYTE *resp, int *respLen)
 * Imager configuration command.
 *
 * @param cmd Configuration command string. See Imager manual for details of commands
 * @param cmdLen length of command string in bytes
 * @param *resp Pointer to buffer receiving reply from Imager
 * @param *respLen Pointer to variable receiving response length in bytes
 * @param imagerType of imager 0=Opticon
 * @return	Zero when succeeded, On error non-zero error code is returned. 
 * @sa enum IMAGER_TYPE
 */
NUR_API
int NURAPICONV NurAccImagerCmd(HANDLE hApi,BYTE *cmd, int cmdLen,int imagerType, BYTE *resp, int *respLen);

/** @fn int NurAccImagerSaveConfig(HANDLE hApi,int imagerType);
 * Save configuration to Imager memeory.  
 * @param imagerType of imager 0=Opticon
 * @return	Zero when succeeded, On error non-zero error code is returned. 
 * @sa enum NurAccImagerCmd
 * @sa enum IMAGER_TYPE
 */
NUR_API
int NURAPICONV NurAccImagerSaveConfig(HANDLE hApi,int imagerType);
/** @} */ // end of ACCESSORYAPI

#ifdef __cplusplus
};
#endif

#endif
