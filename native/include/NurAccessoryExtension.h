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

/** First byte with notification NUR_NOTIFICATION_ACCESSORY is event type NUR_ACC_EVENT_TYPE */
enum NUR_ACC_EVENT_TYPE
{
	NUR_ACC_EVENT_BARCODE = 1,		/** Barcode read event */
	NUR_ACC_EVENT_SENSOR_CHANGED,		/** Sensor added/removed event.
						  * Followed by a NUR_ACC_SENSOR_CHANGED struct describing the event */
	NUR_ACC_EVENT_SENSOR_RANGE_DATA,	/** Sensor range data stream event. Sent when NUR_ACC_SENSOR_MODE_STREAM mode
						  * is enabled for a NUR_ACC_SENSOR_FEATURE_RANGE sensor. Followed by a
						  * NUR_ACC_SENSOR_RANGE_DATA struct describing the event */
	NUR_ACC_EVENT_SENSOR_TOF_FR_BFA_RAW_DATA, /** Raw data from FR BFA ToF sensor. Followed by a NUR_ACC_SENSOR_TOF_FR_BFA_RAW_DATA struct describing the event. */
};

/** NUR_NOTIFICATION_IOCHANGE source is set to this when accessory device trigger is pressed/released. */
#define NUR_ACC_TRIGGER_SOURCE			100

/** A configuration (or any other class') value indicating that this integer value is either not initialized, not used or both. */
#define INTVALUE_NOT_VALID				-1
	
/** A configuration (or any other class') value indicating that this short value is either not initialized, not used or both. */
#define SHORTVALUE_NOT_VALID			-1

/** The protocol's value that the extension handler captures. */
//#define NUR_CMD_ACC_EXT					0x55


/******************************************************************************/
/* EXTERNAL ACCESSORY SUB COMMANDS - START                                    */
/******************************************************************************/

/** Please Note here is the main reference for all list of sub commands 
  * used in various devices like EXA31,EXA51,EXA21,EXA81 etc.
  * 
  * This list has to used for any reference in future and all other old implementations
  * should be corrected whenever they are updated.
  */

#define ACC_EXT_GET_FWVERSION           0  /** BLE FW version. */
#define ACC_EXT_GET_CFG                 1  /** Get configuration command. */
#define ACC_EXT_SET_CFG                 2  /** Set configuration command. */
#define ACC_EXT_GET_BATT                3  /** Get Batter Voltage(in mV) command. */
#define ACC_EXT_READ_BARCODE            4  /** Read Imager/barcode command.(blocking) */
#define ACC_EXT_RESTART                 5  /** System restart command. */
#define ACC_EXT_READ_BARCODE_ASYNC      6  /** Asynchronous barcode scan (non-blocking). */
#define ACC_EXT_SET_LED_OP              7  /** Set external LED. */
#define ACC_EXT_BEEP_ASYNC              8  /** Asynchronous beep operation. */
#define ACC_EXT_GET_BATT_INFO           9  /** Battery information ("extended information"). */
#define ACC_EXT_ENTER_TESTMODE          10 /** NUR/BLE Radio TestMode settings */
#define ACC_EXT_GET_HEALTHSTATE         11 /** Get HW status (imager, NUR module etc.). */
#define ACC_EXT_WIRELESS_CHARGE         12 /** Set/ get wireless charging. */
#define ACC_EXT_IMAGER                  13 /** Imager base Command */
#define ACC_EXT_VIBRATE                 14 /** Use device's vibra. */
#define ACC_EXT_CLEAR_PAIRS             15 /** Clear device pairing information. */
#define ACC_EXT_GET_MODEL_INFORMATION   16 /** Get Device Model Information. */
#define ACC_EXT_BLE_PACKET_SEND_TEST    17 /** BLE Packet Test via NUR_NOTIFY_BLE_READER */
#define ACC_EXT_GET_CONNECTION_INFO     18 /** Get connection info. */
#define ACC_EXT_SENSOR_ENUMERATE        19 /** Enumerate sensors. */
#define ACC_EXT_SENSOR_SET_CONFIG       20 /** Set sensor config. */
#define ACC_EXT_SENSOR_GET_CONFIG       21 /** Get sensor config. */
#define ACC_EXT_SENSOR_SET_FILTER       22 /** Set sensor filter. */
#define ACC_EXT_SENSOR_GET_FILTER       23 /** Get sensor filter. */
#define ACC_EXT_SENSOR_GET_VALUE        24 /** Get sensor value. */
#define ACC_EXT_SENSOR_SET_SETTINGS     25 /** Set sensor specific settings. */
#define ACC_EXT_SENSOR_GET_SETTINGS     26 /** Get sensor specific settings. */
#define ACC_EXT_CONFLICTED_27           27 /** CONFLICTED. Remove if used */
#define ACC_EXT_MCUMGR                  28 /** Mcumgr base Command */
#define ACC_EXT_HID                     29 /** HID Base command */
/******** Next Free Commands for future use. ******/

#define ACC_EXT_PRODUCTION              99 /** Production base Command */
#define ACC_EXT_IMAGER_DIAGNOTICS      100 /** Imager Diagnotics */
/******** Next Free Commands for future use. ******/

/** List of Names to be removed to avoid usage conflicts.*/
//#define BLE_EXT_SET_BLENAME             19   /** CONFLICTED. Remove if used */
//#define ACC_EXT_SET_BLE_PASSKEY         25   /** CONFLICTED. Remove if used */
//#define ACC_EXT_GET_BLE_PASSKEY         26   /** CONFLICTED. Remove if used */
//#define ACC_EXT_CLEAR_BLE_PASSKEY       27   /** CONFLICTED. Remove if used */

/******************************************************************************/
/* EXTERNAL ACCESSORY SUB COMMANDS - END                                      */
/******************************************************************************/

/** Instruct the restart command to enter the DFU mode. */
#define RESET_BOOTLOADER_DFU_START	0xB1

/** Instruct the restart command to actually power off the device. */
#define RESET_POWEROFF				0xF1

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


/** IMAGER COMMANDS [sub commands for ACC_EXT_IMAGER] */
#define IMAGER_CMD_TRIGGER_PRE_SET      1
#define IMAGER_CMD_TRIGGER_CANCEL       2
#define IMAGER_CMD_CENTRAL_READING      3
#define IMAGER_CMD_RAW_CMD              4   /** To send Imager commands */
#define IMAGER_CMD_POWER                5   /** Imager power on/off  */
#define IMAGER_CMD_AIM                  6   /** Imager aiming on/off */

/** ALIAS Names to avoid dependency builds failures. */
#define ACC_EXT_IMAGER_CMD      IMAGER_CMD_RAW_CMD
#define ACC_EXT_IMAGER_POWER    IMAGER_CMD_POWER            
#define ACC_EXT_IMAGER_AIM      IMAGER_CMD_AIM        

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

/**
 * Nur accessory sensor changed description.
 */
typedef struct __NUR_ACC_SENSOR_CHANGED
{
	BYTE source;			/**< Source number for the sensor that was added/removed */
	BYTE removed;			/**< Sensor added(0) or removed(1) */
} NUR_ACC_SENSOR_CHANGED;

/**
 * Nur accessory range sensor stream data.
 */
typedef struct __NUR_ACC_SENSOR_RANGE_DATA
{
	BYTE source;
	INT range;				/**< Unit: mm */
} NUR_ACC_SENSOR_RANGE_DATA;

/**
 * Nur accessory FR BFA ToF raw data for one ToF sensor.
 */
typedef struct __NUR_ACC_SENSOR_TOF_FR_BFA_RAW_DATA_ITEM
{
	WORD dist_cm;		/**< Unit: cm */
	BYTE status;		/**< A status with status 6 or 9 can be considered with a confidence value of 50%. All other status are below 50% confidence level.
						  * Measurement validity status meanings:
						  * 0	Ranging data are not updated
						  * 1	Signal rate too low on SPAD array
						  * 2	Target phase error
						  * 3	Sigma estimator too high
						  * 4	Target consistency failed (Wrap Around fail 1/2)
						  * 5	Range valid
						  * 6	Wrap around not performed (Typically the first range)
						  * 7	Rate consistency failed (Wrap Around fail 2/2)
						  * 8	Signal rate too low for the current target
						  * 9	Range valid with large pulse (may be due to a merged target)
						  * 10	Range valid, but no target detected at previous range
						  * 11	-
						  * 12	Target blurred by another one, due to sharpener
						  * 15	No target detected (only if nb of target detected is enabled)
						  */
} NUR_ACC_SENSOR_TOF_FR_BFA_RAW_DATA_ITEM;

/**
 * Nur accessory FR BFA ToF raw stream data.
 */
typedef struct __NUR_ACC_SENSOR_TOF_FR_BFA_RAW_DATA
{
	BYTE source;
	NUR_ACC_SENSOR_TOF_FR_BFA_RAW_DATA_ITEM items[16];
} NUR_ACC_SENSOR_TOF_FR_BFA_RAW_DATA;

/**
 * Nur accessory sensor generic configuration.
 *
 * Source definitions:
 *   0: GPIO pin 1
 *   1: GPIO pin 2
 *   2: GPIO pin 3
 *   3: GPIO pin 4
 * 128: Device tap sensor
 * 130: USB sensor (connected directly or to USB hub port 1)
 * 131: USB sensor (connected to USB hub port 2)
 * 132: USB sensor (connected to USB hub port 3)
 * 133: USB sensor (connected to USB hub port 4)
 * 134: Time-of-Flight sensor
 * 135: FR BFA Time-of-Flight sensor
 *
 * @sa NurAccSensorSetConfig(), NurAccSensorEnumerate()
 */
typedef struct __NUR_ACC_SENSOR_CONFIG
{
	BYTE source;			/**< Assigned by reader; Sensor/GPIO pin number changes will be reported on */
	BYTE type;			/**< Assigned by reader; NUR_ACC_SENSOR_TYPE */
	WORD feature;			/**< Assigned by reader; features supported by this sensor; see NUR_ACC_SENSOR_FEATURE */
	BYTE mode;			/**< Bitmask specifying how sensor changes are reported; NUR_ACC_SENSOR_MODE */
} NUR_ACC_SENSOR_CONFIG;

typedef struct __NUR_ACC_SENSOR_FILTER
{
	WORD flags;			/**< Enabled filters flags; see NUR_ACC_SENSOR_FILTER_FLAG */

	/**< Valid when flags contain NUR_ACC_SENSOR_FILTER_FLAG_RANGE */
	struct
	{
		WORD lo;		/**< Goes low->high when sensors reads less than this (unit: mm) */
		WORD hi;		/**< Goes high->low when sensors reads more than this (unit: mm) */
	} range_threshold;	/**< hi must be equal or greater to lo */

	/**< Valid when flags contain NUR_ACC_SENSOR_FILTER_FLAG_TIME */
	struct
	{
		WORD lo;		/**< Triggers when level going high->low for this amount of time (unit: ms) */
		WORD hi;		/**< Triggers when level going low->high for this amount of time (unit: ms) */
	} time_threshold;
} NUR_ACC_SENSOR_FILTER;


typedef struct __NUR_ACC_SENSOR_SETTINGS_FRBFA
{
	BYTE freq_hz;		/**< Sampling frequency in hertz. Value 1 - 60 */
} NUR_ACC_SENSOR_SETTINGS_FRBFA;

/**
 * Nur accessory sensor sensor specici settings.
 *
 *
 * @sa NurAccSensorGetSettings(), NurAccSensorSetSettings()
 */
typedef struct __NUR_ACC_SENSOR_SETTINGS
{
	union {
		/** NUR_ACC_SENSOR_TYPE_EXT_TOF_FR_BFA */
		NUR_ACC_SENSOR_SETTINGS_FRBFA fr_bfa;
	};
} NUR_ACC_SENSOR_SETTINGS;

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
/** NUR_ACC_CONFIG.config value. If set, device has an imager. */
#define DEV_FEATURE_IMAGER      (1<<2)

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

/**
 * Nur accessory sensor types.
 */
enum NUR_ACC_SENSOR_TYPE
{
	NUR_ACC_SENSOR_TYPE_ULTRASONIC_MAXSONAR,
	NUR_ACC_SENSOR_TYPE_DEVICE_GPIO,
	NUR_ACC_SENSOR_TYPE_DEVICE_TAP,
	NUR_ACC_SENSOR_TYPE_DEVICE_TOF,
	NUR_ACC_SENSOR_TYPE_EXT_TOF_FR_BFA,
};

/**
 * Features supported by sensor.
 */
enum NUR_ACC_SENSOR_FEATURE
{
	NUR_ACC_SENSOR_FEATURE_RANGE	 	= (1<<0),	/**< GPIO changes is triggered based on limit of read values */
	NUR_ACC_SENSOR_FEATURE_STREAM_VALUE 	= (1<<1),	/**< Supports streaming of raw sensor values */
};

/**
 * Event streams used to report changes.
 */
enum NUR_ACC_SENSOR_MODE
{
	NUR_ACC_SENSOR_MODE_GPIO			= (1<<0),		/**< Report changes as GPIO events */
	NUR_ACC_SENSOR_MODE_STREAM			= (1<<1),		/**< Stream raw sensor values via the accessory sensor event API (feature flag requirement: NUR_ACC_SENSOR_FEATURE_STREAM_VALUE) */
};

/**
 * Sensor filters in use.
 */
enum NUR_ACC_SENSOR_FILTER_FLAG
{
	NUR_ACC_SENSOR_FILTER_FLAG_RANGE 	= (1<<0),	/**< Range threshold filter enabled. */
	NUR_ACC_SENSOR_FILTER_FLAG_TIME 	= (1<<1),	/**< Time threshold filter enabled. */
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

/** @fn int NurAccRestartToDFU(HANDLE hApi)
 * Restart the BLE module to DFU (Device Firmware Upgrade) mode.
 * After the call, BLE module's FW can be upgraded using DFU protocol.
 *
 * @param	hApi			Handle to valid NurApi object instance.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurAccRestartToDFU(HANDLE hApi);

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

/** Callback function type for NurAccSensorEnumerate() function */
typedef void (NURAPICALLBACK *NurAccSensorEnumerateCallback)(HANDLE hApi, NUR_ACC_SENSOR_CONFIG *cfg, DWORD cfgSize);

/** @fn int NurAccSensorEnumerate(HANDLE hApi, NurAccSensorEnumerateCallback *callback)
 *
 * Get sensor configuration from reader.
 *
 * Needs to be called at startup and after a NUR_ACC_EVENT_SENSOR_CONFIG_CHANGED
 * event is received.
 *
 * @param callback	Callback function that will be called for each sensor attached to the reader.
 * @return		Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurAccSensorEnumerate(HANDLE hApi, NurAccSensorEnumerateCallback callback);

/** @fn int NurAccSensorSetConfig(HANDLE hApi, NUR_ACC_SENSOR_CONFIG *cfg, DWORD cfgSize)
 *
 * Change the configuration for a sensor.  cfg->source needs to be set to
 * identify the sensor. Currently only cfg->enabled can be set by this function.
 *
 * @param cfg          Sensor description (current configuration also returned here on success)
 * @param cfgSize      Size of type specific derived structure
 * @return		Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurAccSensorSetConfig(HANDLE hApi, NUR_ACC_SENSOR_CONFIG *cfg, DWORD cfgSize);

/** @fn int NurAccSensorGetConfig(HANDLE hApi, NUR_ACC_SENSOR_CONFIG *cfg, DWORD cfgSize)
 *
 * Get the configuration for a sensor. cfg->source needs to be set to identify
 * the sensor.
 *
 * @param cfg		Sensor description
 * @param cfgSize	Size of cfg
 * @return		Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurAccSensorGetConfig(HANDLE hApi, NUR_ACC_SENSOR_CONFIG *cfg, DWORD cfgSize);

/** @fn int NurAccSensorSetFilter(HANDLE hApi, BYTE source, NUR_ACC_SENSOR_FILTER *filter, DWORD filterSize)
 *
 * Set triggers and filters for a sensor. GPIO events also support NUR_ACC_SENSOR_FILTER_FLAG_TIME.
 *
 * @param source	Value identifying the sensor/GPIO
 * @param filter	Filter to take into use (current filter is also returned here on success)
 * @param filterSize	Size of filter struct
 * @return		Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurAccSensorSetFilter(HANDLE hApi, BYTE source, NUR_ACC_SENSOR_FILTER *filter, DWORD filterSize);

/** @fn int NurAccSensorGetFilter(HANDLE hApi, BYTE source, NUR_ACC_SENSOR_FILTER *filter, DWORD filterSize)
 *
 * Get triggers and filters for a sensor. GPIO events also support NUR_ACC_SENSOR_FILTER_FLAG_TIME.
 *
 * @param source	Value identifying the sensor/GPIO
 * @param filter	Current filter will be returned here
 * @param filterSize	Size of filter struct
 * @return		Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurAccSensorGetFilter(HANDLE hApi, BYTE source, NUR_ACC_SENSOR_FILTER *filter, DWORD filterSize);

/** @fn int NurAccSensorGetValue(HANDLE hApi, BYTE source, BYTE *dataType, void *value, DWORD valueSize)
 *
 * Get the latest reading from sensor.
 *
 * The format of the value is sensor dependant; the format is returned in dataType:
 * - NUR_ACC_EVENT_SENSOR_RANGE_DATA => value is a struct of type NUR_ACC_SENSOR_RANGE_DATA
 *
 * @param source	Value identifying the sensor/GPIO
 * @param dataType	Data type will be returned here
 * @param value		Current value will be returned here
 * @param valueSize	Size of value struct
 * @return		Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurAccSensorGetValue(HANDLE hApi, BYTE source, BYTE *dataType, void *value, DWORD valueSize);

/** @fn int NurAccSensorSetSettings(HANDLE hApi, BYTE source, byte type, NUR_ACC_SENSOR_SETTINGS *settings, DWORD settingsSize)
 *
 * Get sensor specifc settings for a sensor.
 *
 * @param source	Value identifying the sensor/GPIO
 * @param type		Type of sensor (NUR_ACC_SENSOR_TYPE); determines NUR_ACC_SENSOR_SETTINGS union layout
 * @param settings	Current settings will be returned here
 * @param settingsSize	Size of settings struct
 * @return			Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurAccSensorSetSettings(HANDLE hApi, BYTE source, BYTE type, NUR_ACC_SENSOR_SETTINGS* settings, DWORD settingsSize);

/** @fn int NurAccSensorGetSettings(HANDLE hApi, BYTE source, byte type, NUR_ACC_SENSOR_SETTINGS *settings, DWORD settingsSize)
 *
 * Get sensor specifc settings for a sensor.
 *
 * @param source	Value identifying the sensor/GPIO
 * @param type		Type of sensor (NUR_ACC_SENSOR_TYPE); determines NUR_ACC_SENSOR_SETTINGS union layout
 * @param settings	Current settings will be returned here
 * @param settingsSize	Size of settings struct
 * @return			Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurAccSensorGetSettings(HANDLE hApi, BYTE source, BYTE type, NUR_ACC_SENSOR_SETTINGS* settings, DWORD settingsSize);

/** @} */ // end of ACCESSORYAPI

#ifdef __cplusplus
};
#endif

#endif
