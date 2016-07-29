#ifndef _NURAPI_H_
#define _NURAPI_H_ 1

#include "NurOS.h"

#include "NurAPIConstants.h"
#include "NurAPIExport.h"
#include "NurAPIErrors.h"

#ifdef __cplusplus 
extern "C" { 
#endif

///////////////////////////////////////////////
// Public functions

/** @defgroup API NurApi
 *  C/C++ API for NUR module based RFID devices.
 *  @{
 */


/**
 * Tag tracking routine function parameters
 * @sa NurApiStartTagTracking()
 */
struct NUR_TAGTRACKING_CONFIG
{
	/** Configuration flags for tag tracking. @sa NUR_TAGTRACKING_FLAGS */
	DWORD flags;
	/** Tag tracking routine configuration flags. @sa NUR_TAGTRACKING_EVENTS */
	DWORD events;
	/** When NUR_TTEV_RSSI is enabled, this delta value defines how much the RSSI needs to change to  */
	int rssiDeltaFilter;
	/** When NUR_TTEV_POSITION is enabled, this delta value defines how much the X.  Position is normalized 0.0-1.0 value.*/
	float positionDeltaFilter;
	/** Tag tracking continues inventory as long as there's more new tags found than scanUntilNewTagsCount*/
	int scanUntilNewTagsCount;
	/** When TTEV_VISIBILITY is enabled, change event is generated when tag has been out of view more than visibilityTimeout milliseconds */
	int visibilityTimeout;

	/** Memory bank used for simple filter mask. Used when NUR_TTFL_USESIMPLEFILTER is set in flags. @sa enum NUR_BANK */
	BYTE selectBank;
	/** Simple filter mask data address in <b>bits</b>. Used when NUR_TTFL_USESIMPLEFILTER is set in flags. */
	DWORD selectAddress;
	/** Simple f mask data length <b>bits</b>. Used when NUR_TTFL_USESIMPLEFILTER is set in flags. */
	WORD selectMaskBitLength;
	/** Simple filter mask data buffer. Used when NUR_TTFL_USESIMPLEFILTER is set in flags. */
	BYTE selectMask[NUR_MAX_SELMASK];

	/** Antennamask to be used as the gate-functionality "in"-antennas */
	DWORD inAntennaMask;
	/** Antennamask to be used as the gate-functionality "out"-antennas */
	DWORD outAntennaMask;

	/** Number of filter structs in complexFilters array. Used when NUR_TTFL_USECOMPLEXFILTER is set in flags. */
	int complexFilterCnt;
	/** Pointer to filter array. Used when NUR_TTFL_USECOMPLEXFILTER is set in flags. @sa NUR_INVEX_FILTER */
	struct NUR_INVEX_FILTER *complexFilters;
	/** Pointer to inventory parameters. Used when NUR_TTFL_USECOMPLEXFILTER is set in flags. @sa NUR_INVEX_PARAMS */
	struct NUR_INVEX_PARAMS *complexFilterParams;
};

/**
 * Tag tracking tag
 */
struct NUR_TT_TAG
{
	/** Tag tracking tag EPC data. */
	BYTE epc[NUR_MAX_EPC_LENGTH_EX];
	/** Number of bytes stored in epc field. */
	BYTE epcLen;
	/** TTEV_* bit mask of changed events */
	DWORD changedEvents;
	/**  Epoch time of tag last seen */
	ULONGLONG lastUpdateTime;
	/**  Epoch time of last event change */
	ULONGLONG lastSeenTime;
	/** Array containing RSSI values for each antenna */
	signed char rssi[NUR_MAX_ANTENNAS_EX];
	/** Maximum scaled RSSI */
	int maxScaledRssi;
	/** Maximum RSSI */
	int maxRssi;
	/** Antenna ID where maxRssi and maxScaledRssi was detected */
	int maxRssiAnt;
	/**  Set to 0 if tag is not in view at the moment, otherwise 1 */
	BYTE visible;
	/** Normalized X position of tag; Only valid is TTEV_POSITION is enabled. */
	float X;
	/** Normalized Y position of tag; Only valid is TTEV_POSITION is enabled. */
	float Y;
	/** Previous sector tag was detected; Only valid is TTEV_SECTOR is enabled. */
	int prevSector;
	/** Current sector tag was detected; Only valid is TTEV_SECTOR is enabled */
	int sector;
	/** First antennaId where the tag was seen. */
	int firstTTIOReadSource;
	/** Second antennaId where the tag was seen.  */
	int secondTTIOReadSource;
	/** If NUR_TTEV_INOUT set, this implies whether the tag has moved in-to-out or out-to-in */
	int directionTTIO;
	/** Array containing scaled RSSI values for each antenna */
	int scaledRssi[NUR_MAX_ANTENNAS_EX];
	/** Array containing the tag seen count for each antenna */
	DWORD seenCnt[NUR_MAX_ANTENNAS_EX];
};
/**
 * Data sent within tag tracking change event.
*/
struct NUR_TTCHANGED_DATA
{	
	/** Antenna or beam that generated this event. If TTEV_POSITION or TTEV_SECTOR is set this indicated beam, otherwise this indicates antenna id.*/
	int readSource;
	/** Number of changed tags */
	int changedCount;
	/** TTEV_* bit mask of changed events */ 
	int changedEventMask;
	/** If set true, tag tracking has finished scanning all enabled antennas and must be restarted*/
	BOOL stopped;
};

/**
 * Data sent within scan start/stop event.
*/
struct NUR_TTSCAN_DATA
{
	/** Antenna or beam that generated this event. If TTEV_POSITION or TTEV_SECTOR is set this indicated beam, otherwise this indicates antenna id.*/
	int readSource;
	/** Indicates whether inventory is starting or stopping on this readSource */
	BOOL started;
};

/**
 * Data sent with notification NUR_NOTIFICATION_PERIODIC_INVENTORY.
 * @sa NurApiSetNotificationCallback(), NUR_NOTIFICATION_PERIODIC_INVENTORY, enum NUR_NOTIFICATION
 */
struct NUR_PERIODIC_INVENTORY_DATA
{
	int numTagsFound;	/**< Number of tags found */
	int numTagsMem;		/**< Number of tags in NUR module memory */
	int roundsDone;		/**< Number of full Q rounds done in this inventory */
	int collisions;		/**< Number of possible collisions or reception errors in this inventory */
	int Q;				/**< Q used in this inventory */
	int error;			/**< Error code returned from inventory round */
};

/**
 * Data sent with notification NUR_NOTIFICATION_PRGPRGRESS.
 * @sa NurApiSetNotificationCallback(), NUR_NOTIFICATION_PRGPRGRESS, enum NUR_NOTIFICATION
 */
struct NUR_PRGPROGRESS_DATA
{
	int error;			/**< Error code returned from page programming. */
	int curPage;		/**< Current page, this is -1 in first notification before programming started. */
	int totalPages;		/**< Total number of pages to program. */
};

/**
 * Data sent with notification NUR_NOTIFICATION_TRACETAG.
 * @sa NurApiTraceTag32(), NUR_NOTIFICATION_TRACETAG, enum NUR_NOTIFICATION
 */
struct NUR_TRACETAG_DATA
{
	int rssi;						/**< Tag RSSI, -127 if tag could not be found. */
	int scaledRssi;					/**< Tag RSSI scaled to 0% - 100%. */
	int antennaId;					/**< Antenna ID where tag was found. */
	int epcLen;						/**< Number of bytes stored in epc field. */
	BYTE epc[NUR_MAX_EPC_LENGTH];	/**< Traced tag EPC data. */
};

/**
 * Data sent with notification NUR_NOTIFICATION_IOCHANGE.
 * @sa NUR_NOTIFICATION_IOCHANGE, enum NUR_NOTIFICATION
 */
struct NUR_IOCHANGE_DATA
{
	BOOL sensor;	/**< TRUE if notification source is sensor, otherwise FALSE and source is GPIO. */
	int source;		/**< Sensor/GPIO source number. */
	int dir;		/**< IO change direction; 1 = Low to high transition (Rising edge), 0 = High to low transition (Falling edge). */
};

/**
 * Data sent with notification NUR_NOTIFICATION_TRIGGERREAD.
 * @sa NUR_NOTIFICATION_TRIGGERREAD, enum NUR_NOTIFICATION
 */
struct NUR_TRIGGERREAD_DATA
{
	BOOL sensor;	/**< TRUE if notification source is sensor, otherwise FALSE and source is GPIO. */
	int source;		/**< Sensor/GPIO source number. */
	int antennaID;	/**< ID of antenna where tag was read. */
	int rssi;		/**< Tag RSSI, -127 if tag could not be found. */
	int scaledRssi; /**< Tag RSSI scaled to 0% - 100%. */
	int epcLen;		/**< Number of bytes stored in epc field, zero if tag could not be found. */
	BYTE epc[NUR_MAX_EPC_LENGTH];	/**< Tag EPC data. */
};

/**
 * Data sent with notification NUR_NOTIFICATION_INVENTORYSTREAM.
 * Use NurApiGetTagCount(), NurApiGetTagData(), NurApiClearTags() functions to access tag data.
 * 
 * @sa NUR_NOTIFICATION_INVENTORYSTREAM, enum NUR_NOTIFICATION
 */
struct NUR_INVENTORYSTREAM_DATA
{
	int tagsAdded;		/**< Number of tags added to NurApi internal tag storage. */
	BOOL stopped;		/**< Streaming has stopped. If you need continuous streaming, you need to restart streaming. */
	int roundsDone;		/**< Number of full Q rounds done in this inventory. */
	int collisions;		/**< Number of possible collisions or reception errors in this inventory. */
	int Q;				/**< Q used in this inventory. */
};

/**
 * Data sent with notification NUR_NOTIFICATION_EASALARM.
 *
 * @note Not supported in firmware version 3.1-A or older.
 *
 * @sa NUR_NOTIFICATION_EASALARM, enum NUR_NOTIFICATION, NurApiNXPStartAlarmStream(), NurApiNXPStopAlarmStream(), NurApiIsNXPAlarmStreamRunning()
 */
struct NUR_EASALARMSTREAM_DATA
{
	BOOL stopped;		/**< Streaming has stopped. If you need continuous streaming, you need to restart streaming. */
	BOOL armed;			/**< If TRUE, EAS Alarm is active. */
};

/**
 * Notification about the last programmed EPC during the EPC enumeration based on TID contents.
*/
struct NUR_EPCENUMSTREAM_DATA
{
	int epcLen;						/**< Length of the EPC data. This is the last programmed EPC. */
	BYTE lastEPC[MAX_EE_EPCLEN];	/**< Content of the last programmed EPC. */
	int tidwAddr;					/**< This is the word address where the TID data was read from. */
	int tidLen;						/**< Length of the TID data. This is the TID data with which the EPC was assigned by. */
	BYTE tidData[MAX_EE_TIDLEN];	/**< Content of the TID data. */
};

/**
 * NUR_INVENTORY_RESPONSE holds last inventory result.
 * @sa NurApiInventory(), NurApiInventorySelect32(), NurApiInventorySelectByEPC()
 */
struct NUR_INVENTORY_RESPONSE
{
	int numTagsFound;	/**< Number of tags found in this inventory. */
	int numTagsMem;		/**< Total number of tags in module memory. */
	int roundsDone;		/**< Number of full Q rounds done in this inventory. */
	int collisions;		/**< Number of possible collisions or reception errors in this inventory. */
	int Q;				/**< Q used in this inventory. */
};

/**
 * Contains information about the current inventory read configuration.
 * @sa NurApiGetInventoryRead, NUR_IRTYPE
 */
struct NUR_IRINFORMATION
{	
	BOOL active;	/**< Active / not. */		
	DWORD type;		/**< Current inventory + read type. */	
	DWORD bank;		/**< Bank to read from. 0 means invalid. */	
	DWORD wAddress;	/**< Word address to read from. */	
	DWORD wLength;	/**< Word length. 0 means invalid. */
};

/**
 * NUR_REGIONINFO holds information about specific region.
 * @sa NurApiGetRegionInfo()
 */
struct NUR_REGIONINFO
{
	TCHAR name[64];			/**< Region name. */
	DWORD regionId;			/**< Region ID. */
	DWORD baseFrequency;	/**< RF base frequency in kHz. */
	DWORD channelSpacing;	/**< RF channel spacing in kHz. */
	DWORD channelCount;		/**< RF channel count. */
	DWORD channelTime;		/**< Maximum RF channel on time. */
};

/**
 * NUR_READERINFO holds information about connected module.
 * @sa NurApiGetReaderInfo()
 */
struct NUR_READERINFO
{
	TCHAR serial[32];		/**< Module serial number. */
	TCHAR altSerial[32];	/**< Alternative manufacturer serial number. */
	TCHAR name[32];			/**< Module name. */
	TCHAR fccId[48];		/**< FCC ID. */
	TCHAR hwVersion[16];	/**< Hardware version identifier. */
	int swVerMajor;			/**< Software major version number. */
	int swVerMinor;			/**< Software minor version number. */
	char devBuild;			/**< Software development build identifier. */
	int numGpio;			/**< Number of GPIO's available on module. */
	int numSensors;			/**< Number of sensors available on module. */
	int numRegions;			/**< Number of regions available on module. */
	int numAntennas;		/**< Number of enabled antennas on module. */
	int maxAntennas;		/**< Number of maximum antennas on reader. */
};

/**
 * NUR_IMAGE_INFO holds information about NUR module's image file.
 * @sa NurApiGetImageInfo, NUR_BINFILEFORMAT
 */
struct NUR_IMAGE_INFO
{
	DWORD dwType;		/**< TRUE if the contents of the file were guessed. */
	TCHAR desc[32];		/**< Application / bootloader */
	TCHAR modArch[32];	/**< NUR05W, xNUR, NUR05WL2 */
	DWORD binSize;		/**< Actual size of the image. */	
};

/**
 * NUR_DEVICECAPS holds information about the connected module's capabilities.
 * @sa NurApiGetDeviceCaps(), enum NUR_CHIPVER, enum NUR_MODULETYPE
 */
struct NUR_DEVICECAPS
{
	DWORD dwSize;		/**< Size of the structure */
	DWORD flagSet1;		/**< Flag set 1 */
	DWORD flagSet2;		/**< Flag set 2 */
	
	int maxTxdBm;		/**< Maximum TX level in dBm. */
	int txAttnStep;		/**< TX level attenuation pre step in dBm. */
	WORD maxTxmW;		/**< Maximum TX level in mW. */
	WORD txSteps;		/**< Number of TX attenuation levels available. */
	
	WORD szTagBuffer;	/**< Number of 96-bit EPCs that the module tag buffer can currently hold. */
	WORD curCfgMaxAnt;	/**< Maximum number of antennas possible with current configuration. */
	WORD curCfgMaxGPIO;	/**< Maximum number of GPIO pins possible with current configuration. */
	WORD chipVersion;	/**< RFID chip version. (enum NUR_CHIPVER) */
	WORD moduleType;	/**< Module type. (enum NUR_MODULETYPE) */
	DWORD moduleConfigFlags; /**< Internal module config flags */
	WORD v2Level;		/**< Version 2 commands' support level. */
	BYTE res[SZ_NUR_DEVCAPS - 4*sizeof(DWORD) - 2*sizeof(int) - 8*sizeof(WORD)];	/**< The size left over of 128 bytes. */
};

/**
 * Module sensor configuration.
 * @sa NurApiSetSensorConfig(), NurApiGetSensorConfig(), NUR_GPIO_ACTION, NUR_NOTIFICATION_IOCHANGE, NUR_NOTIFICATION_TRIGGERREAD
 */
struct NUR_SENSOR_CONFIG
{
	BOOL tapEnabled;	/**< TRUE if tap sensor is enabled. */
	int  tapAction;		/**< Action for tap sensor. GPIO_ACT_NOTIFY or GPIO_ACT_SCANTAG. */
	BOOL lightEnabled;	/**< TRUE if light sensor is enabled. */
	int  lightAction;	/**< Action for light sensor. GPIO_ACT_NOTIFY or GPIO_ACT_SCANTAG. */
};

/**
 * Single GPIO configuration.
 * @sa NUR_GPIO_CONFIG, NurApiSetGPIOConfig(), NurApiGetGPIOConfig(), NUR_NOTIFICATION_IOCHANGE, NUR_NOTIFICATION_TRIGGERREAD, NUR_GPIO_ACTION, NUR_GPIO_EDGE
 */
struct NUR_GPIO_ENTRY
{
	BOOL available;		/**< TRUE if configuration is available. When writing new configuration set to FALSE if no need to configure this GPIO. */
	BOOL enabled;		/**< TRUE GPIO is enabled. */
	int type;			/**< Type of GPIO. see enum NUR_GPIO_TYPE. */
	int edge;			/**< Trigger edge, falling, rising or both. see enum NUR_GPIO_EDGE. */
	int action;			/**< Trigger action. see enum NUR_GPIO_ACTION. */
};

/**
 * Module GPIO configuration.
 * @sa NurApiSetGPIOConfig(), NurApiGetGPIOConfig(), NUR_GPIO_ENTRY
 */
struct NUR_GPIO_CONFIG
{
	int count;										/**< Number of entries filled. */
	struct NUR_GPIO_ENTRY entries[NUR_MAX_GPIO];	/**< Configuration entries. Each entry represents one GPIO in module. */
};

/**
 * Single GPIO status.
 * @sa NurApiGetGPIOStatus(), enum NUR_GPIO_TYPE
 */
struct NUR_GPIO_STATUS
{
	BOOL enabled;	/**< TRUE if GPIO is enabled. */
	int  type;		/**< Type of the GPIO. see enum NUR_GPIO_TYPE */
	BOOL state;		/**< State of the GPIO. Only valid if GPIO is configured as input (NUR_GPIO_TYPE_INPUT). */
};

/**
 * Tag RSSI filter.
 * @sa NurApiGetModuleSetup(), NurApiSetModuleSetup()
 */
struct NUR_RSSI_FILTER
{
	char min; /**< Minimum accepted RSSI in dBm. Use 0 to disable filtering. */
	char max; /**< Maximum accepted RSSI in dBm. Use 0 to disable filtering. */
};

/**
 * Module's autotune setup. Supported from FW 5.0-A in L2 module.
 * 
 * @sa AUTOTUNE_MODE_ENABLE, AUTOTUNE_MODE_THRESHOLD_ENABLE
*/
struct NUR_AUTOTUNE_SETUP
{
	BYTE mode;			/**< Autotune mode setting, bits: @sa AUTOTUNE_MODE_ENABLE, AUTOTUNE_MODE_THRESHOLD_ENABLE. */
	char threshold_dBm;	/**< Low level threshold in dBm. Do autotune if current reflected power worse than 'threshold_dBm' */
};

/**
 * Backward compatibility definition 
*/
#define AUTOTUNE_SETUP NUR_AUTOTUNE_SETUP

/**
 * Nur module setup structure.
 * @sa NurApiGetModuleSetup(), NurApiSetModuleSetup(), NUR_MODULESETUP_FLAGS, NurApiStoreCurrentSetup(), NurApiStoreCurrentSetupEx()
 */
struct NUR_MODULESETUP
{
	/** NUR module tag backscatter link frequency in Hz.
	 *
	 * Currently there are 3 link frequencies available:
	 * @li 160000 Hz
	 * @li 256000 Hz
	 * @li 320000 Hz
	 */
	int linkFreq;

	/** NUR module RX coding (miller).
	 *
     * @sa enum NUR_RXDECODING 
     *
	 * The reader supports FM=0, Miller-2, Miller-4 and Miller-8 coding. 
	 * @li 0 = FM-0
	 * @li 1 = Miller-2
	 * @li 2 = Miller-4
	 * @li 3 = Miller-8
	 */
	int rxDecoding;

	/** NUR module transmission level attenuation in 1dB steps.
	 *
	 * The transmission level has a range of 0 - 19.
	 * 
	 * TX level value is attenuation from maximum level: TX level(dBm) = (27 - txLevel)dBm.
	 * 
	 * 500mW reader values:
	 * @li 0 = 27 dBm, 500mW
	 * @li 1 = 26 dBm, 398mW
	 * @li 2 = 25 dBm, 316mW
	 * @li 3 = 24 dBm, 251mW
	 * @li 4 = 23 dBm, 200mW
	 * @li 5 = 22 dBm, 158mW
	 * @li 6 = 21 dBm, 126mW
	 * @li 7 = 20 dBm, 100mW
	 * @li 8 = 19 dBm, 79mW
	 * @li 9 = 18 dBm, 63mW
	 * @li 10 = 17 dBm, 50mW
	 * @li 11 = 16 dBm, 40mW
	 * @li 12 = 15 dBm, 32mW
	 * @li 13 = 14 dBm, 25mW
	 * @li 14 = 13 dBm, 20mW
	 * @li 15 = 12 dBm, 16mW
	 * @li 16 = 11 dBm, 13mW
	 * @li 17 = 10 dBm, 10mW
	 * @li 18 = 9 dBm, 8mW
	 * @li 19 = 8 dBm, 6mW
	 *
	 * 1000mW reader values:
	 * @li 0 = 30 dBm, 1000mW
	 * @li 1 = 29 dBm, 794mW
	 * @li 2 = 28 dBm, 631mW
	 * @li 3 = 27 dBm, 501mW
	 * @li 4 = 26 dBm, 398mW
	 * @li 5 = 25 dBm, 316mW
	 * @li 6 = 24 dBm, 251mW
	 * @li 7 = 23 dBm, 200mW
	 * @li 8 = 22 dBm, 158mW
	 * @li 9 = 21 dBm, 126mW
	 * @li 10 = 20 dBm, 100mW
	 * @li 11 = 19 dBm, 79mW
	 * @li 12 = 18 dBm, 63mW
	 * @li 13 = 17 dBm, 50mW
	 * @li 14 = 16 dBm, 40mW
	 * @li 15 = 15 dBm, 32mW
	 * @li 16 = 14 dBm, 25mW
	 * @li 17 = 13 dBm, 20mW
	 * @li 18 = 12 dBm, 16mW
	 * @li 19 = 11 dBm, 13mW
	 */
	int txLevel;

	/** NUR module TX modulation.
	 *
     * @sa enum NUR_TXMODULATION 
     *
	 * The transmitter can be set to use ASK or PR-ASK modulation.
	 * @li 0 = ASK
	 * @li 1 = PR-ASK
	 */
	int txModulation;

	/** NUR module region.
	 *
     * Currently there are 10 regions available:
	 * @li 0 = EU
	 * @li 1 = FCC (North-America)
	 * @li 2 = PRC (China)
	 * @li 3 = Malaysia
	 * @li 4 = Brazil
	 * @li 5 = Australia
	 * @li 6 = New Zealand
	 * @li 7 = Japan 250mW LBT (NUR Fw 2.4-A, later or NURL2)
	 * @li 8 = Japan 500mW DRM (NUR Fw 2.4-A, later or NURL2)
	 * @li 9 = Korea LBT (NUR Fw 2.7-A, later or NURL2)
	 * @li 10 = India (NUR Fw 2.7-A, later or NURL2)
	 * @li 11 = Russia (NUR Fw 3.0-A, later or NURL2)
	 * @li 12 = Vietnam (NUR Fw 3.0-A, later or NURL2)
	 * @li 13 = Singapore (NUR Fw 3.0-A, later or NURL2)
	 * @li 14 = Thailand (NUR Fw 3.0-A, later or NURL2)
	 * @li 15 = Philippines (NUR Fw 3.0-A, later or NURL2)
	 * @li 16 = Morocco (NUR Fw 5.0-A or later)
	 * @li 17 = Peru (NUR Fw 5.0-A or later)
	 * @li 254 = Custom
	 */
	int regionId;

	/** Default Q used for inventory. Value 0 - 15, where value 0 means automatic Q selection. */
	int inventoryQ;

	/** Default session for inventory. Value 0 - 3. */
	int inventorySession;

	/** Default rounds for inventory. Value 0 - 10, where 0 means automatic rounds selection. 
	  * This value specifies full G2 query rounds performed in one inventory command.
	  */
	int inventoryRounds;

	/** Bitmask of enabled antennas. Value 0x1 - 0xF.
	 * DEPRECATED: Do not use for new designs, use antennaMaskEx instead.
	 * Example: Value 0x4 means that only antenna 3 is enabled.
	 * @sa enum NUR_ANTENNAMASK
	 */
	int antennaMask;

	/** Triggered SingleScan scanning timeout in milliseconds. Value 50 - 500. */
	int scanSingleTriggerTimeout;

	/** Triggered Inventory timeout in milliseconds. Value 0 - 60000. 
	  * Device will scan new tags specified timeout when no new tags are found.
	  * If no new tags found within timeout, triggred inventory is considered done and results are sent to the host.
	  */
	int inventoryTriggerTimeout;

	/** Zero based index of selected antenna logical id. NUR_ANTENNAID_AUTOSELECT (-1) means auto switch between enabled antennas. 
	 * NOTE: Selected antenna must be enabled before set.
	 */
	int selectedAntenna;

	/** Operation flags. @sa enum NUR_OPFLAGS */
	DWORD opFlags;

	/** Inventory target, select whether tags whose inventoried flag is A or B participate in the inventory. @sa enum NUR_INVENTORY_TARGET */
	int inventoryTarget;

	/** Desired EPC Length filter in bytes. All other EPC's will be discarded during inventory. Use 255 to disable EPC length filter. */
	int inventoryEpcLength;

	/** RSSI Filters used for read operations. @sa struct NUR_RSSI_FILTER */
	struct NUR_RSSI_FILTER readRssiFilter;

	/** RSSI Filters used for write operations. @sa struct NUR_RSSI_FILTER */
	struct NUR_RSSI_FILTER writeRssiFilter;

	/** RSSI Filters used for inventory operations. @sa struct NUR_RSSI_FILTER */
	struct NUR_RSSI_FILTER inventoryRssiFilter;
	
	/** Read timeout in ms. */
	int readTO;

	/** Write timeout in ms. */
	int writeTO;
	
	/** Lock timeout in ms. */
	int lockTO;
	
	/** Kill timeout in ms. */
	int killTO;
	
	/** Defines how the periodic auto-inventory power saving is configured.
	 * When in use, reader will be power save mode while no tags in view.
	 * @sa enum NUR_AUTOPERIOD
	 */
	int periodSetup;

	/** DEPRECATED! Per antenna specific power levels. Use antPowerEx instead. */
	int antPower[NUR_MAX_ANTENNAS];

	/** Modify power levels' offset values (-1, 0, 1) NOTE: First entry in array used for all antennas! Rest of the entries are discarded. */
	int powerOffset[NUR_MAX_ANTENNAS];

	/** Bitmask of enabled antennas, support up to 32 antennas. Value 0x1 - 0xFFFFFFFF.
	 * Example: Value 0x4 means that only antenna 3 is enabled.
	 * Example: Value 0x30300 means that antennas 9,10,17,18 are enabled.
	 * @sa enum NUR_ANTENNAMASK
	 */
	DWORD antennaMaskEx;

	/** Runtime auto tuning settings. */
	struct NUR_AUTOTUNE_SETUP autotune;

	/** Per antenna specific tx levels. Array of the per antenna tx level values. Range is 0..19; Defaults to -1. If set to -1 default tx level is used from the module setup. */
	int antPowerEx[NUR_MAX_ANTENNAS_EX];

	/** The receiver sensitivity field. 0 = Nominal, 1 = Low, 2 = High */
	int rxSensitivity;
};

/**
 * Contains single tag inventoried information.
 * @sa NurApiGetTagData(), NurApiGetAllTagData()
 */
struct NUR_TAG_DATA
{
	WORD timestamp;		  /**< Timestamp when tag was found in milliseconds; Only valid when meta data is fetched. */
	signed char rssi;			/**< Tag RSSI; Only valid when meta data is fetched. */
	char scaledRssi;	  /**< Tag RSSI scaled to 0% - 100%; Only valid when meta data is fetched. */
	DWORD freq;			    /**< Frequency in kHz where tag was found; Only valid when meta data is fetched. */
	WORD pc;			      /**< Tag PC word; Only valid when meta data is fetched. */
	BYTE channel;		    /**< Channel index where tag was found; Only valid when meta data is fetched. */
	BYTE antennaId;		  /**< Antenna ID where tag was found; Only valid when meta data is fetched. */
	BYTE epcLen;		    /**< Number of bytes stored in epc field. */
	BYTE epc[NUR_MAX_EPC_LENGTH]; /**< Tag EPC data. */
};

/**
 * Contains single tag inventoried information added with possible read data.
 * @sa NurApiGetTagDataEx(), NurApiGetAllTagDataEx()
 */
struct NUR_TAG_DATA_EX
{
	WORD timestamp;			/**< Timestamp when tag was found in milliseconds; Only valid when meta data is fetched. */
	signed char rssi;		/**< Tag RSSI; Only valid when meta data is fetched. */
	char scaledRssi;		/**< Tag RSSI scaled to 0% - 100%; Only valid when meta data is fetched. */
	DWORD freq;			    /**< Frequency in kHz where tag was found; Only valid when meta data is fetched. */
	WORD pc;			    /**< Tag PC word; Only valid when meta data is fetched. */
	BYTE channel;		    /**< Channel index where tag was found; Only valid when meta data is fetched. */
	BYTE antennaId;			/**< Antenna ID where tag was found; Only valid when meta data is fetched. */
	BYTE epcLen;		    /**< Number of bytes stored in epc field. */
	BYTE epc[NUR_MAX_EPC_LENGTH_EX]; /**< Tag EPC data. Same length as data because the read data may be placed into the EPC field. */
	BYTE dataLen;			/**< Number of data bytes assiciated w/ tag. */
	BYTE data[NUR_MAX_IRDATA_LENGTH]; /**< Associated tag inventory read data. */
	WORD xpc_w1;			/**< Possible XPC word 1 */
	WORD xpc_w2;			/**< Possible XPC word 2 */
};

/**
 * Contains the custom hoptable configuration.
 * @sa NurApiGetCustomHoptable()
 */
struct NUR_CUSTOMHOP_PARAMS
{
  DWORD count;        /**< Number of channels in this table. */
  DWORD chTime;       /**< Channel time in milliseconds. */
  DWORD silentTime;   /**< Silent time in milliseconds, if any, between channel change. */
  DWORD maxBLF;       /**< Maximum link frequency. */
  DWORD Tari;         /**< Tari: 1=12.5 and 2 = 25. */
  DWORD freqs[NUR_MAX_CUSTOM_FREQS];  /**< Frequencies in kHz. Number of populated antries is stated with the 'count'. */
};

/**
 * Contains the custom hoptable configuration, extended version: + LBT + max TX level.
 * @sa NurApiGetCustomHoptableEx()
 */
struct NUR_CUSTOMHOP_PARAMS_EX
{
	DWORD count;        /**< Number of channels in this table. */
	DWORD chTime;       /**< Channel time in milliseconds. */
	DWORD silentTime;   /**< Silent time in milliseconds, if any, between channel change. */
	DWORD maxBLF;       /**< Maximum link frequency. */
	DWORD Tari;         /**< Tari: 1=12.5 and 2 = 25. */
	int lbtThresh;      /**< LBT threshold; minimum value is -90. */
	DWORD maxTxLevel;   /**< Maximum TX level; range is 0...19. */
	DWORD freqs[NUR_MAX_CUSTOM_FREQS];  /**< Frequencies in kHz. Number of populated antries is stated with the 'count'. */
};
/**
 * Extended inventory function parameters
 * @sa NurApiInventoryEx(), NurApiStartInventoryEx()
 */
struct NUR_INVEX_PARAMS
{
	/** Q value used for inventory. Value 0 - 15, where value 0 means automatic Q selection. The Q is number of tag slots in single G2 query round. */
	int Q;
	
	/** Session for inventory. Value 0 - 3. */
	int session;

	/** Default rounds for inventory. Value 0 - 10, where 0 means automatic rounds selection. 
	  * This value specifies full G2 query rounds performed in one inventory command.
	  * NOTE: If transitTime is specified, this value is ignored.
	  */
	int rounds;

	/** Total inventory time specified in milliseconds. Time is limited by maximum region channel time. Use 0 to disable. */
	int transitTime;

	/** Inventory target, select whether tags whose inventoried flag is A or B participate in the inventory. @sa enum NUR_INVENTORY_TARGET */
	int inventoryTarget;

	/** Chooses which tags respond to the inventory. @sa enum NUR_INVENTORY_SELSTATE */
	int inventorySelState;
};

/**
 * Custom bit stream / exchange parameters.
 */
struct NUR_CUSTEXCHANGE_PARAMS
{
	/** Number of bits to transmit. */
	WORD txLen;
	/** Expected number of bits in response. */
	WORD rxLen;
	/** RX timeout (20...100). */
	DWORD timeout;
	/** If true, the custom bit stream behaves like a write operation. */
	BOOL asWrite;
	/** The handle received from the tag's singulation is appended to the bit stream. */
	BOOL appendHandle;
	/** If TX length is 16 then XOR the singulation response's handle with the TX data. */
	BOOL xorRN16;
	/** Transmit only; to response expected. */
	BOOL txOnly;
	/** The transmission shall contain no CRC. */
	BOOL noTxCRC;
	/** The receiver shall not decode a CRC; it is returned 'as is'. */
	BOOL noRxCRC;
	/** If TRUE then the transmission CRC type is CRC-5. */
	BOOL txCRC5;
	/** If TRUE then the rxLen parameter is ignored. */
	BOOL rxLenUnknown;  
	/** If TRUE then the last 16 bits of the response are stripped (appended handle is removed). */
	BOOL rxStripHandle;
	/** The raw bit buffer to send. */
	BYTE bitBuffer[NUR_MAX_BITS_IN_STREAM / 8];
};

/**
 * Extended inventory function filter parameters. Represents G2 Select command, see UHF G2 standard section 6.3.2.11.1.1
 * @sa NurApiInventoryEx(), NurApiStartInventoryEx()
 */
struct NUR_INVEX_FILTER
{
	/** Set to FALSE always. NOT SUPPORTED AT THE MOMENT */
	BOOL truncate;

	/** Target session or SL. Indicates whether G2 select command modifies tag's SL flag or inventoried (A/B) flag. @sa enum NUR_INVENTORY_SESSION */
	BYTE target;

	/** Indicates whether matching tags assert or deassert SL, or set their inventoried flag to A or B. @sa enum NUR_FILTER_ACTION */
	BYTE action;

	/** Memory bank used for filter mask. Bank must not be passwd bank. @sa enum NUR_BANK */
	BYTE bank;

	/** Filter mask data address in <b>bits</b>. */
	DWORD address;

	/** Length of the mask data in <b>bits</b>. Maximum length is 255 bits */
	int maskBitLength;

	/** Mask data buffer. */
	BYTE maskData[NUR_MAX_SELMASK];
};


/**
 * Sampo Ethernet device configuration parameters. 
 * @sa NurApiGetEthConfig(), NurApiSetEthConfig()
 */
struct NUR_ETHDEV_CONFIG
{
	/** Transport method used with NurApiSetEthConfig().
	 *  if transport = 0 then ethernet settings will be set to Sampo via current connection.(USB,serial or TCP)
	 *	if transport = 1 then ethernet settings will be set to Sampo using broadcast message.
	 *	This method requires that correct 'mac' address has been set to structure. MAC address is used for filtering that only certain device receives new configurations.
	 */	
	BYTE transport;		

	/** (Get/Set) Title of Sampo device: Default: "Nordic ID Sampo S1" */
	TCHAR title[32];	

	/** (Get) Current Revision of ethernet CPU firmware */
	int version;	

	/** (Get) Current IP address of Sampo device */
	BYTE ip[4];			

	/** (Get/Set) Subnet mask (used in static IP mode) */
	BYTE mask[4];

	/** (Get/Set) Gateway	(used in static IP mode) */
	BYTE gw[4];			

	/** (Get/Set) Address type 0=DHCP(default) 1=STATIC */
	BYTE addrType;		
	
	/** (Get/Set) Static IP. When using static IP address, set addrType to 1=STATIC */
	BYTE staticip[4];

	/** (Get/(Set only if using transport as broadcast)) MAC address of device. Used for addressing when sending new settings via Broadcast. */
	BYTE mac[6];		

	/** (Get/Set) Server port where client connects to when hostmode=Server. default 1300. */
	int serverPort;		

	/** (Get/Set) hostmode 0=Server (default) 1=Client.
	 *	If server, Sampo listens port 'serverPort' for client connections (default).
	 *  If Client, Sampo automatically tries to initialize connection to 'hostip' : 'hostPort' server.
	 */
	BYTE hostmode;		

	/** (Get/Set) Client host IP (if Mode=Client) */
	BYTE hostip[4];		

	/** (Get/Set) Client host port (if Mode=Client) */
	int hostPort;		

	/** (Get/Set) reserved for future usage */
	BYTE reserved[8];
};

/**
 * Contains Ethernet Sampo device information.
 * @sa NUR_NOTIFICATION_DEVSEARCH
 */
struct NUR_NETDEV_INFO
{	
	/** Current device status 0=Disconnected 1=Connected */
	BYTE status;	

	/** Board ID (NOT USED) */
	BYTE boardID;

	/** Board type (NOT USED) */
	BYTE boardType;	

	/** Internal temperature of Sampo Ethernet CPU (Celsius) */
	double temp;

	/** Current ADC value of light sensor. Max 1023 */
	int lightADC;

	/** Current ADC value of tap sensor. Max 1023 */
	int tapADC;	

	/** Current I/O state (NOT USED) */
	BYTE IOstate;

	/** Up time in days since last reset */
	int upTime;	

	/** NUR SW version like: 2.5.B */
	BYTE nurVer[3]; 
	
	/** NUR Serial number */
	char serial[16];

	/** NUR alternative serial number */
	char altSerial[16];

	/** NUR Name */
	char nurName[16];

	/** NUR Hardware version */
	char hwVer[8];

	/** NUR Number of antennas used */
	BYTE nurAntNum;

	/** Ethernet configuration */
	struct NUR_ETHDEV_CONFIG eth;
};

/**
 * Contains information about connected client
 * @sa NUR_NOTIFICATION_CLIENTCONNECTED
 */
struct NUR_CLIENT_INFO
{		
	/** Handle of client API */	
	HANDLE clientApi;
	/** IP address of Client */	
	TCHAR ip[32];
	/** Port where client connected */	
	int port;	
};

/**
 * Contains module's internal information.
 * @sa NurApiGetSystemInfo
 */
struct NUR_SYSTEMINFO
{
	/** Bootloader address. */	
	DWORD blAddr;
	/** Application address */
	DWORD appAddr;	
	/** Interrupt vector base address. */
	DWORD vectorBase;
	/** Application size. */
	DWORD appSzWord;	
	/** Application CRC. */
	DWORD appCRCWord;
	/** Flash size. */
	DWORD szFlash;
	/** SRAM size. */
	DWORD szSram;
	/** Stack top address. */
	DWORD stackTop;	
	/** Non-volatile settings' address. */
	DWORD nvSetAddr;
	/** Size of non-volatile settings region. */
	DWORD szNvSettings;
	/** Stack usage when entered app main(). */
	DWORD mainStackUsage;
	/** Used SRAM. */
	DWORD szUsedSram;
	/** Szie of internal tag buffer in bytes. */
	DWORD szTagBuffer;
};

/**
 * Contains EPC enumeration information.
 * @sa NurApiStartEPCEnumeration, NurApiStopEPCEnumeration
 */
struct NUR_EPCENUMINFO
{
	int ant;			/**< Selected antenna */
	int twAddr;			/**< Word address of the TID data i.e. where to read the TID info */
	int twLen;			/**< TID data word length. */
	int useBlWrite;		/**< Block write type if any: 0 = no, 1 = one word, 2 = two words. */
	BYTE startVal[8];	/**< Start value of the enumerated value in the EPC : equals to "unsigned long long" i.e. 64-bit unsigned integer. */
	int epcLen;			/**< EPC length in words; 4...8 */
	BYTE baseEPC[MAX_EE_EPCLEN];	/**< Base EPC value to be used and modified during the enumeration. */
	int modAddr;		/**< Bit address where the enumerated value is located in the EPC memory */
	int bitLen;			/**< Bit length of the modified value: 1...64 */
	BOOL bReset;		/**< If non-zero then the current TID information is cleared */	
};

/**
 * Contains permalock read information.
 * @sa 
*/
struct NUR_PERMALOCK_RD_PARAM
{
	BYTE bank;			/**< Allowed range is 1...3. */
	DWORD startAddr;	/**< Starting address for permalock. Start address is multiplied by 16 in order to get the first block number. */
	BYTE range;			/**< Number of blocks to lock. Also multiplied by 16 to get the number of lock WORDs. */
};

/**
 * Contains block permalocking information.
 * @sa NurApiReadPermalock, NurApiReadPermalockByEPC, NurApiReadPermalockSingulated
*/
struct NUR_PERMALOCK_PARAM
{
	DWORD bank;			/**< Allowed range is 1...3. */
	DWORD startAddr;	/**< Starting address for permalock. Start address is multiplied by 16 in order to get the first block number. */
	DWORD range;		/**< Number of blocks to lock. Also multiplied by 16 to get the number of lock WORDs. */
	WORD wMask[255];	/**< Mask bits. Little-endian format. */
};

/**
 * The Gen2 version 2 "Untraceable" command parameters.
 * @sa NurApiGen2v2Untraceable, NurApiGen2v2Untraceable32, NurApiGen2v2UntraceableByEPC
 * @sa NUR_UTRACE_RANGE_NORMAL, NUR_UTRACE_RANGE_REDUCE
*/
struct NUR_UNTRACEABLE_PARAM
{
	BOOL setU;			/**< U-bit value; TRUE for '1', FALSE for '0'. */
	BOOL rxAttn;		/**< RX attenuation for response reception; TRUE causes "write-like" operation thus reduces operation's range. */
	BOOL hideEPC;		/**< If TRUE then the EPC is hidden according to other rules. */
	DWORD epcWordLen;	/**< New length of the EPC in 16-bit words. Allowed range is 0..31. */
	DWORD tidPolicy;	/**< TID hiding policy: 0 = none, 1 = hide some, 2 = hide all. */
	BOOL hideUser;		/**< User memory hiding policy: TRUE = hide, FALSE = don't hide. */
	DWORD rangePolicy;	/**< Only "normal" and "reduced" are supported at the moment. */
};

/**
 * The Gen2 version 2 "Authenticate" parameter.
 * @sa NUR_GEN2V2_MAX_AUTHBYTES
*/
struct NUR_AUTHENTICATE_PARAM
{
	DWORD csi;		/**< Cryptographic Suite Indicator. */
	DWORD rxLength;	/**< Set to 0 if reception length is not known, otherwise this is the bit length of the reception ranging in 1...1008. */
	BOOL rxAttn;	/**< RX attenuation for response reception; causes "write-like" operation thus reduces operation's range. */
	BOOL reSelect;	/**< If module needs to internally use the ReadBuffer command then setting this to TRUE causes the tag to be re-selected between operations. */
	DWORD timeout;	/**< Response timeout similar to read or write; unit is milliseconds ranging from 20...50. */
	DWORD preTxWait;	/**< Additional wait time before executing the command in microseconds (0...50000); may be used to energize the tag properly (carrier is on). */
	DWORD msgBitLength;	/**< The CSI message's length in bits. */	
	BYTE message[NUR_GEN2V2_MAX_AUTHBYTES];	/**< Must be taken care by the host application. */
};

/**
 * The ISO29167-10 authentication parameters for methods 1 and 2.
*/
struct NUR_TAM_PARAM
{
	BOOL tam2;		/**< If TRUE then the method is 2 (TAM2 with custom data). */	
	BOOL decrypt;	/**< If TRUE then the contents are decrypted using the key. With TAM2 the contents are decrypted with key and using an IV (->CBC) from the first data block. */
	DWORD keyNum;	/**< Key number to use. */	
	DWORD mpi;		/**< TAM2: Memory Profile Indicator. Basic indicator are: 0 = EPC, 1 = TID and 2 = user memory. */	
	DWORD protMode;	/**< Encipherment mode. */
	DWORD offset;	/**< The block data's offset value in TAM2. */	
	DWORD blockCount;	/**< Number of custom data blocks. Allowed range is 1...4 (8...32 bytes as a result). */
	BYTE key[NUR_TAM_KEYLEN];	/**< Key to use if response is decrypted. */	
};

/**
 * The ISO29167-10 authentication response for methods 1 and 2.
*/
struct NUR_TAM_RESP
{
	BOOL response;	/**< There is a response present. */
	BOOL ok;		/**< The response is decrypted, C_TAM matches and challenge contents was OK. */
	DWORD C_TAM;		/**< The 16-bit constant in the first block. */
	DWORD TRnd32;	/**< The following 32-bit random value in the first block. */		
	DWORD szBlocks;		/**< Bytes actually in the block data. */
	BYTE challenge[TAM_CHALLENGE_BYTELEN]; /**< The challenge used in TAM1 or 2. */
	BYTE firstBlock[NUR_TAM_KEYLEN]; /**< Decrypted or raw contents of the first block. */
	BYTE blockData[NUR_TAM_MAXBLOCKS * NUR_SZ_TAM2_BLOCK];	/**< Block data if custom data was requested. */
};

/**
 * Contains permalock command response when the lock status is read.
 * @sa NurApiPermalock, NurApiPermalockByEPC, NurApiPermalockSingulated
*/
struct NUR_PERMALOCK_RD_RESP 
{
	int bank;				/**< From bank */	
	DWORD start;			/**< First block chunk address. */
	int nBlocks;			/**< Number of 16-words actually that the  tag actually responded with. */
	WORD lockStates[255];	/**< Plain data from the tag. Note that the most significant bit (0x8000 as a 16-bit value) in this case is always the first one in the chuck of blocks (0...15, 16...31, ...). */
};

/**
 * Contains authentication command response to the authentication (tag's reply if present).
 * @ NurApiGen2v2Authenticate32
*/
struct NUR_AUTHENTICATE_RESP
{
	int status;			/**< The operation status: 0 = OK, 1 = no response, 2 = tag error reply, -1 = not present (API error returned). */
	int tagError;		/**< Initilaized to -1 = not present. Other values indicate tag's error reply code. */
	int bitLength;		/**< Number of bits received from the tag. */
	BYTE reply[NUR_MAX_AUTHRESPBITS / 8];	/**< The tag's reply in whole. */
};

/**
 * NUR_NOTIFICATION_HOPEVENT data.
 */
struct NUR_HOPEVENT_DATA
{
	BYTE hopTableId;	/**< Current hop table region id */
	BYTE freqIdx;		/**< Index of frequency in hop table */
	DWORD freqKhz;		/**< Frequency in kHz */
};

/**
 * NUR_NOTIFICATION_TUNEEVENT data.
 */
struct NUR_TUNEEVENT_DATA
{
	BYTE cap1;				/**< Tuning capacitor 1 value */
	BYTE cap2;				/**< Tuning capacitor 2 value */
	int reflPower_dBm;		/**< Reflected power in dBm*1000 */
	BYTE antenna;			/**< Antenna ID */
	DWORD freqKhz;			/**< Frequency in kHz */
};

/**
 *	Antenna mapping that maps logical antenna number to an antenna name.
 */
struct NUR_ANTENNA_MAPPING
{
	int antennaId;		/**< Logical number of the antenna. */
	TCHAR name[MAX_MAPPINGLEN + 1];	/**< Name of the antenna + 0.*/
};

/**
 * Contains WLAn module's general information and current status.
 * @sa NurApiWlanGetStatus
 */
struct NUR_WLANSTATUS
{	
	DWORD wlan_status;		/**< WLan status 0=disable 1=enable 2=initializing. */	
	/** ROLE_STA = 0,ROLE_AP = 2,ROLE_P2P = 3,
		 ROLE_STA_ERR =  -1, Failure to load MAC/PHY in STA role
		 ROLE_AP_ERR  =  -ROLE_AP,  Failure to load MAC/PHY in AP role
		 ROLE_P2P_ERR =  -ROLE_P2P  Failure to load MAC/PHY in P2P role */
	LONG role;
	DWORD wlan_connection_status;	/**< see enum WLAN_STATUS */
	DWORD profile_count;		/**< Profile count. */
	LONG lasterror;				/**< last error code */	
	DWORD ChipId;				/**< Chip ID */
    BYTE FwVersion[16];			/**< FW Version */
    BYTE PhyVersion[16];		/**< PHY Version */
	BYTE NwpVersion[16];		/**< NWP Version */
	BYTE HostDrvVer[16];		/**< Host driver Version */
    DWORD RomVersion;			/**< ROM Version */	
	DWORD ip;					/**< IP */	
	DWORD gwip;					/**< GW IP */
	BYTE mac[6];				/**< MAC address. */	
	BYTE ssid[32];				/**< SSID. */
	BYTE connection_ssid[32];	/**< SSID where connected to. */
	BYTE connection_mac[6];		/**< MAC address where connected to. */	
	LONG scantime;				/**< Time in milliseconds how long network scan enabled */	
	SHORT rssi_mgmnt;			/**< Holds the average management frames RSSI as dBm */
	SHORT rssi_datactrl;		/**< Holds the average data + ctrl frames RSSI as dBm */
	BYTE reserved[10];			/**< Reserved */
};

/**
 * Contains WLAN connection profile information.
 * @sa NurApiWlanGetProfile, NurApiWlanSetProfile, NurApiWlanDeleteProfile
 */
struct NUR_WLANPROFILE
{
	DWORD index;		/**< Index of profile (1-7) */
	BYTE ssid[32];		/**< SSID of target AP */
	BYTE mac[6];		/**< MAC of target AP */
	BYTE secType;		/**< Security type: Open,WEP WPA1, WPA2 */
	BYTE secKey[32];	/**< Security key: */
	BYTE extUser[32]; 	/**< ExtUser */
	BYTE anonUser[32]; 	/**< AnonUser */
	BYTE certIndex;		/**< certIndex */
	DWORD eapMethod;	/**< EAP Method */
	DWORD priority;		/**< Priority. Priority. 0-7 (0=Highest) */
};

/**
 * Contains WLAN network scan result information. (single item)
 * @sa NurApiWLanScanEnable
 */
struct NUR_WLAN_SCAN_RESULT
{	
	BYTE index;			/**< Index. 0=first item. */
	TCHAR ssid[32];		/**< SSID of target AP */
	BYTE mac[6];		/**< MAC of target AP */
	BYTE secType;		/**< Security type: Open,WEP WPA1, WPA2 */	
	BYTE rssi;			/**< RSSI */
	BYTE reserved[10];	/**< Reserved */	
};

/** 
 * Type of NurApi notification callback. Used for receiving events from NurApi.
 */
typedef void (NURAPICALLBACK *NotificationCallback)(HANDLE hApi, DWORD timestamp, int type, LPVOID data, int dataLen);

/** @fn HANDLE NurApiCreate()
 * Creates new instance of NurApi object.
 *
 * @return	Handle to NurApi object or NULL on error.
 */
NUR_API
HANDLE NURAPICONV NurApiCreate();

/** @fn int NurApiFree(HANDLE hApi)
 * Free allocated NurApi object.
 * @param	hApi			Handle to valid NurApi object instance.
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiFree(HANDLE hApi);

/** @fn DWORD NurApiGetTimestamp(HANDLE hApi)
 * Get NurApi internal timestamp. This is time in milliseconds since NurApi was created.
 * @param	hApi		Handle to valid NurApi object instance. 
 * @return	Time in milliseconds since NurApi was created.
 */
NUR_API
DWORD NURAPICONV NurApiGetTimestamp(HANDLE hApi);

/** @fn BOOL NurApiGetFileVersion(TCHAR *version, int versionSize)
 * Get NurApi DLL version number as string.
 * @param	version			Pointer to a buffer that receives NurApi.dll version string.
 * @param	versionSize		The size of <i>version</i> buffer in TCHARs.
 * @return	TRUE on success, otherwise FALSE.
 */
NUR_API
BOOL NURAPICONV NurApiGetFileVersion(TCHAR *version, int versionSize);

/** @fn BOOL NurApiGetImageInfo(HANDLE hApi, TCHAR *fileName, struct NUR_IMAGE_INFO *pInfo)
 * Get NUR module's image file information.
 *
 * @sa NUR_IMAGE_INFO
 * 
 * @param	hApi		Valid NUR API handle or NULL.
 * @param	fileName	The file name to inspect.
 * @param	pInfo		Pointer to a NUR_IMAGE_INFO structure.
 * @return	Zero on success, non-zero error code otherwise.
 */
NUR_API
int NURAPICONV NurApiGetImageInfo(HANDLE hApi, TCHAR *fileName, struct NUR_IMAGE_INFO *pInfo);

/** @fn int NurApiLoadSetupFile(HANDLE hApi, const TCHAR *filename)
 * Load settings from specific configuration file and apply to module.
 * 
 * @sa NurApiSaveSetupFile()
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	filename	Path to configuration file.
 * 							
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiLoadSetupFile(HANDLE hApi, const TCHAR *filename);

/** @fn int NurApiLoadHoptable(HANDLE hApi, const TCHAR *filename, DWORD *errFlags)
 * Load custom hoptable configuration from an .INI-file.
 *
 * @sa NUR_CUSTHOP_ERROR.
 *
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	filename	Path to hoptable .INI-file.
 * @param	errFlags	Pointer to parese error flags. Optional.
 * 							
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiLoadHoptable(HANDLE hApi, const TCHAR *filename, DWORD *errFlags);

/** @fn int NurApiSaveHoptable(HANDLE hApi, const TCHAR *filename, BOOL comment, BOOL apply)
 * Save currently stored hoptable toan .INI-file.
 *
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	filename	Path to hoptable .INI-file.
 * @param	comment		The output file is commented as per setting.
 * @param	apply		If set to TRUE, then the "Apply" value will be set to 1 in the resulting .INI-file.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSaveHoptable(HANDLE hApi, const TCHAR *filename, BOOL comment, BOOL apply);

/** @fn int NurApiReadPermalock(HANDLE hApi, DWORD password, BOOL secured, struct NUR_PERMALOCK_RD_PARAM *pLock, struct NUR_PERMALOCK_RD_RESP *pResp)
 *
 * Read permalock status without singulating the tag.
 * 
 * @sa NurApiReadPermalockByEPC, NurApiReadPermalockSingulated32, NUR_PERMALOCK_PARAM, NUR_PERMALOCK_RD_RESP
 * 
 * @param	password	Passsword for this operation. If secured is FALSE, then this parameter is ignored.
 * @param	secured		When set to TRUE the password parameter is used when accessing the tag.
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	pLock		Pointer to the NUR_PERMALOCK_PARAM structure defining the read conditions.
 * @param	pResp		Pointer to NUR_PERMALOCK_RD_RESP where a successful reponse is decoded into.
 *
 * @return	Zero when succeeded, non-zero error code when failed.
*/
NUR_API
int NURAPICONV NurApiReadPermalock(HANDLE hApi, DWORD password, BOOL secured, struct NUR_PERMALOCK_RD_PARAM *pLock, struct NUR_PERMALOCK_RD_RESP *pResp);

/** @fn int NurApiReadPermalockByEPC(HANDLE hApi, DWORD password, BOOL secured, BYTE *epc, DWORD epcLen, struct NUR_PERMALOCK_RD_PARAM *pLock, struct NUR_PERMALOCK_RD_RESP *pResp)
 *
 * Read permalock status by singulating the tag with given EPC data.
 * 
 * @sa NurApiReadPermalock, NurApiReadPermalockSingulated32, NUR_PERMALOCK_RD_PARAM, NUR_PERMALOCK_RD_RESP
 *
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	password	Passsword for this operation. If secured is FALSE, then this parameter is ignored. 
 * @param	secured		When set to TRUE the password parameter is used when accessing the tag.
 * @param	epc			Pointer to the EPC data.
 * @param	epcLen		Length of the EPC. Maximum length is 64 bytes that correspond to 496 bits. Can be set to 0; in that case @sa NurApiReadPermalockStatus is called.
 * @param	pLock		Pointer to the NUR_PERMALOCK_PARAM structure defining the read conditions.
 * @param	pResp		Pointer to NUR_PERMALOCK_RD_RESP where a successful reponse is decoded into.
 *
 * @return	Zero when succeeded, non-zero error code when failed.
*/
NUR_API
int NURAPICONV NurApiReadPermalockByEPC(HANDLE hApi, DWORD password, BOOL secured, BYTE *epc, DWORD epcLen, struct NUR_PERMALOCK_RD_PARAM *pLock, struct NUR_PERMALOCK_RD_RESP *pResp);

/** @fn int NurApiReadPermalockSingulated32(HANDLE hApi, DWORD password, BOOL secured, 
												BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
												struct NUR_PERMALOCK_RD_PARAM *pLock, struct NUR_PERMALOCK_RD_RESP *pResp)
 *
 * Read permalock status by singulating the tag with given selection data.
 * 
 * @sa NurApiReadPermalock, NurApiReadPermalockByEPC, NUR_PERMALOCK_PARAM, NUR_PERMALOCK_RD_RESP
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	password		Passsword for this operation. If secured is FALSE, then this parameter is ignored. 
 * @param	secured			When set to TRUE the password parameter is used when accessing the tag.
 * @param	sBank			Bank to use in tag selection.
 * @param	sAddress		Bit address of the select data.
 * @param	sMaskBitLength	Bit length of the selection mask (sMask) parameter.
 * @param	sMask			Selection mask data.
 * @param	pLock			Pointer to the NUR_PERMALOCK_PARAM structure defining the read conditions.
 * @param	pResp			Pointer to NUR_PERMALOCK_RD_RESP where a successful reponse is decoded into.
 *
 * @return	Zero when succeeded, non-zero error code when failed.
*/
NUR_API
int NURAPICONV NurApiReadPermalockSingulated32(HANDLE hApi, DWORD password, BOOL secured, 
												BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
												struct NUR_PERMALOCK_RD_PARAM *pLock, struct NUR_PERMALOCK_RD_RESP *pResp);

/** @fn int NurApiPermalock(HANDLE hApi, DWORD password, BOOL secured, struct NUR_PERMALOCK_PARAM *pLock)
 *
 * Permanently lock blocks in a tag without singulating the tag.
 *
 * @sa NurApiPermalockByEPC, NurApiPermalockSingulated32, NUR_PERMALOCK_PARAM, NUR_PERMALOCK_RD_RESP
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	password		Passsword for this operation. If secured is FALSE, then this parameter is ignored. 
 * @param	secured			When set to TRUE the password parameter is used when accessing the tag.
 * @param	pLock			Pointer to a NUR_PERMALOCK_PARAM structure specifying the lock definition.
 *
 * @return	Zero when succeeded, non-zero error code when failed.
*/
NUR_API
int NURAPICONV NurApiPermalock(HANDLE hApi, DWORD password, BOOL secured, struct NUR_PERMALOCK_PARAM *pLock);

/** @fn int NurApiPermalockByEPC(HANDLE hApi, DWORD password, BOOL secured, BYTE *epc, DWORD epcLen, struct NUR_PERMALOCK_PARAM *pLock)
 *
 * Permanently lock block in a tag by selecting it with given EPC data.
 *
 * @sa NurApiPermalock, NurApiPermalockSingulated32, NUR_PERMALOCK_PARAM, NUR_PERMALOCK_RD_RESP
 *
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	password	Passsword for this operation. If secured is FALSE, then this parameter is ignored. 
 * @param	secured		When set to TRUE the password parameter is used when accessing the tag.
 * @param	epc			Pointer to the EPC data.
 * @param	epcLen		Length of the EPC. Maximum length is 64 bytes that correspond to 496 bits. 
 * @param	pLock		Pointer to a NUR_PERMALOCK_PARAM structure specifying the lock definition.
 *
 * @return	Zero when succeeded, non-zero error code when failed.
*/
NUR_API
int NURAPICONV NurApiPermalockByEPC(HANDLE hApi, DWORD password, BOOL secured, BYTE *epc, DWORD epcLen, struct NUR_PERMALOCK_PARAM *pLock);


/** @fn int NurApiPermalockSingulated32(HANDLE hApi, DWORD password, BOOL secured, 
												BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
												struct NUR_PERMALOCK_PARAM *pLock)
 *
 * Permanently lock block in a tag by selecting it with given selection data.
 *
 * @sa NurApiPermalock, NurApiPermalockByEPC, NUR_PERMALOCK_PARAM, NUR_PERMALOCK_RD_RESP
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	password		Passsword for this operation. If secured is FALSE, then this parameter is ignored. 
 * @param	secured			When set to TRUE the password parameter is used when accessing the tag.
 * @param	sBank			Bank to use in tag selection.
 * @param	sAddress		Bit address of the select data.
 * @param	sMaskBitLength	Bit length of the selection mask (sMask) parameter.
 * @param	sMask			Selection mask data.
 * @param	pLock			Pointer to a NUR_PERMALOCK_PARAM structure specifying the lock definition.
 *
 * @return	Zero when succeeded, non-zero error code when failed.
*/
int NURAPICONV NurApiPermalockSingulated32(HANDLE hApi, DWORD password, BOOL secured, 
												BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
												struct NUR_PERMALOCK_PARAM *pLock);

/** @fn int NurApiSaveSetupFile(HANDLE hApi, const TCHAR *filename)
 * Read all module settings and save them to specific configuration file.
 *
 * @sa NurApiLoadSetupFile()
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	filename	Path to configuration file.
 * 							
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSaveSetupFile(HANDLE hApi, const TCHAR *filename);

/** @fn int NurApiSetModuleSetup(HANDLE hApi, DWORD setupFlags, struct NUR_MODULESETUP *setup, DWORD setupSize)
 *
 * Set new setup to NUR module. This will store new setup in volatile memory.
 * If you need persistent setup, you'll need to call NurApiStoreCurrentSetup() or NurApiStoreCurrentSetupEx()
 *
 * @sa NUR_MODULESETUP, NurApiGetModuleSetup(), NUR_MODULESETUP_FLAGS, NurApiStoreCurrentSetup(), NurApiStoreCurrentSetupEx()
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	setupFlags	Defines which flags are valid in <i>setup</i> structure. see enum NUR_MODULESETUP_FLAGS.
 * @param	setup		Pointer to a new module setup. <i>setupFlags</i> defined fields must be valid.
 * @param	setupSize	Size of the <i>setup</i> structure in bytes. Usually: sizeof(struct NUR_MODULESETUP).
 * 
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSetModuleSetup(HANDLE hApi, DWORD setupFlags, struct NUR_MODULESETUP *setup, DWORD setupSize);

/** @fn int NurApiGetModuleSetup(HANDLE hApi, DWORD setupFlags, struct NUR_MODULESETUP *setup, DWORD setupSize)
 *
 * Get current setup from NUR module. 
 * Set <i>setupFlags</i> parameter to wanted setup options to get, use flag NUR_SETUP_ALL to get all.
 *
 * @sa NUR_MODULESETUP, NurApiSetModuleSetup(), enum NUR_MODULESETUP_FLAGS
 * 	   
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	setupFlags	Defines which flags are fetched to <i>setup</i> structure. see enum NUR_MODULESETUP_FLAGS.
 * @param	setup		Pointer to a buffer that receives module current setup defined in <i>setupFlags</i>.
 * @param	setupSize	Size of the <i>setup</i> structure in bytes. Usually: sizeof(struct NUR_MODULESETUP).
 * 
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetModuleSetup(HANDLE hApi, DWORD setupFlags, struct NUR_MODULESETUP *setup, DWORD setupSize);

/** @fn int NurApiStoreCurrentSetup(HANDLE hApi)
 * Store current module full setup, exluding baudrate, to module's non-volatile memory.
 * This is same as NurApiStoreCurrentSetupEx(hApi, NUR_STORE_ALL & ~NUR_STORE_BAUDRATE).
 * 
 * @sa NurApiStoreCurrentSetupEx()
 * 
 * @param	hApi	Handle to valid NurApi object instance
 * 
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStoreCurrentSetup(HANDLE hApi);

/** @fn int NurApiStoreCurrentSetupEx(HANDLE hApi, BYTE flags)
 * Store selected module setup to module's non-volatile memory.
 *
 * @sa enum NUR_STORE_FLAGS
 * 
 * @param	hApi	Handle to valid NurApi object instance.
 * @param	flags   Defines which settings to store. see NUR_STORE_FLAGS
 * 
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStoreCurrentSetupEx(HANDLE hApi, BYTE flags);

/** @fn void NurApiSetLogLevel(HANDLE hApi, int logMask)
 * Set NurApi logging level mask.
 * 
 * @sa enum NUR_LOGS, NurApiSetLogToFile()
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	logMask		Logging level mask to set. see enum NUR_LOGS
 * 						
 * @return	None.
 */
NUR_API
void NURAPICONV NurApiSetLogLevel(HANDLE hApi, int logMask);

/** @fn int NurApiGetLogLevel(HANDLE hApi)
 * Get current NurApi logging level mask.
 * 
 * @sa enum NUR_LOGS, NurApiSetLogToFile()
 * 
 * @param	hApi	Handle to valid NurApi object instance.
 * @return	Current logging level mask. see enum NUR_LOGS
 */
NUR_API
int NURAPICONV NurApiGetLogLevel(HANDLE hApi);

/** @fn void NurApiSetLogToFile(HANDLE hApi, BOOL enable)
 * Enable NurApi log to file.
 * 
 * @sa NurApiSetLogLevel()
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	enable		set TRUE to enable, FALSE to disable.
 * @return	None.
 */
NUR_API
void NURAPICONV NurApiSetLogToFile(HANDLE hApi, BOOL enable);

/** @fn BOOL NurApiGetLogToFile(HANDLE hApi)
 * Get log to file enabled state.
 * 
 * @sa NurApiSetLogLevel()
 * 
 * @param	hApi		Handle to valid NurApi object instance
 * @return	TRUE if enabled, FALSE if disabled.
 */
NUR_API
BOOL NURAPICONV NurApiGetLogToFile(HANDLE hApi);

/** @fn const TCHAR *NurApiSetLogFilePath(HANDLE hApi, const TCHAR *filename)
 * Set log file path.
 * 
 * @sa NurApiGetLogFilePath(), NurApiSetLogToFile(), NurApiSetLogLevel()
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	filename	Path to log file.
 * @return	None.
 */
NUR_API
void NURAPICONV NurApiSetLogFilePath(HANDLE hApi, const TCHAR *filename);

/** @fn const TCHAR *NurApiGetLogFilePath(HANDLE hApi)
 * Get log file path.
 * 
 * @sa NurApiSetLogFilePath(), NurApiSetLogToFile(), NurApiSetLogLevel()
 * 
 * @param	hApi		Handle to valid NurApi object instance
 * @return	Full path to log file.
 */
NUR_API
const TCHAR *NURAPICONV NurApiGetLogFilePath(HANDLE hApi);

/** @fn void NurApiSetContext(HANDLE hApi, LPVOID ctx)
 * Set application specific context to NurApi object.
 *
 * @sa NurApiGetContext()
 * 
 * @param	hApi	Handle to valid NurApi object instance
 * @param	ctx		Pointer to application specific context. Context can be retrieved with NurApiGetContext() function.
 * @return	None.
 */
NUR_API
void NURAPICONV NurApiSetContext(HANDLE hApi, LPVOID ctx);

/** @fn LPVOID NurApiGetContext(HANDLE hApi)
 * Get application specific context from NurApi object.
 *
 * @sa NurApiSetContext()
 * 
 * @param	hApi	Handle to valid NurApi object instance
 * @return	Application specific context set by NurApiSetContext() function.
 */
NUR_API
LPVOID NURAPICONV NurApiGetContext(HANDLE hApi);

/** @fn int NurApiSetNotificationCallback(HANDLE hApi, NotificationCallback nFunc)
 * Set notification receive function.
 * Set function that receives all notification events specific to this NurApi instance. 
 * 
 * @sa enum NUR_NOTIFICATION, NotificationCallback
 * 
 * @param	hApi	Handle to valid NurApi object instance.
 * @param	nFunc	Pointer to a function.
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSetNotificationCallback(HANDLE hApi, NotificationCallback nFunc);

/** @fn void NurApiLog(HANDLE hApi, int level, const TCHAR *txt)
 * Send log string through NurApi.
 * Application can send log string through NurApi and catch them on notification callback.
 * 
 * @param	hApi	Handle to valid NurApi object instance.
 * @param	level	Logging level for this log string.
 * @param	txt		Text to send.
 * @return	None.
 */
NUR_API
void NURAPICONV NurApiLog(HANDLE hApi, int level, const TCHAR *txt);

/** @fn int NurApiIsConnected(HANDLE hApi)
 * Get NUR module connection state.
 * 
 * @remarks This function will return only connection state of the transport layer. 
 * 			In order to know that module is responding, you'll need to communicate with module by calling e.g. NurApiPing().
 *
 * @sa NurApiConnectSerialPort(), NurApiConnectSocket(), NurApiConnectTransport(), NurApiConnect(), NurApiConnectTransportSpec(), 
 * 	   NurApiDisconnect(), NurApiPing()
 *
 * @param	hApi	Handle to valid NurApi object instance.
 * @return	0 if transport is connected, non-zero error code on failure.
 */
NUR_API
int NURAPICONV NurApiIsConnected(HANDLE hApi);

/** @fn int NurApiConnect(HANDLE hApi)
 * Reconnect transport layer with previously set tranport/connect spec.
 *
 * @sa NurApiConnectSerialPort(), NurApiConnectSocket(), NurApiConnectTransport(), NurApiConnect(), NurApiConnectTransportSpec(), 
 * 	   NurApiIsConnected(), NurApiDisconnect()
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @return	0 if connection succeeded, non-zero error code on failure.
 */
NUR_API
int NURAPICONV NurApiConnect(HANDLE hApi);

/** @fn int NurApiConnectSerialPort(HANDLE hApi, int port, int baudRate)
 * Connect to NUR module using serial port transport.
 *
 * @sa NurApiConnectSerialPort(), NurApiConnectSocket(), NurApiConnectTransport(), NurApiConnect(), NurApiConnectTransportSpec(), 
 * 	   NurApiIsConnected(), NurApiDisconnect()
 *
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	port		Serial port (COM) port number to connect.
 * @param	baudRate	Connection port baud rate. Default is 115200 (NUR_DEFAULT_BAUDRATE)
 * @return	0 if connection succeeded, non-zero error code on failure.
 */
NUR_API
int NURAPICONV NurApiConnectSerialPort(HANDLE hApi, int port, int baudRate);

/** @fn int NurApiConnectSerialPortEx(HANDLE hApi, const TCHAR *portName, int baudRate)
 * Connect to NUR module using serial port transport.
 *
 * @sa NurApiConnectSerialPort(), NurApiConnectSocket(), NurApiConnectTransport(), NurApiConnect(), NurApiConnectTransportSpec(), 
 * 	   NurApiIsConnected(), NurApiDisconnect()
 *
 * @param	hApi		  Handle to valid NurApi object instance.
 * @param	portName	Pointer to the serial port's name e.g "COM2:", "\\\\.\\COM10"
 * @param	baudRate	Connection port baud rate. Default is 115200 (NUR_DEFAULT_BAUDRATE)
 * @return	0 if connection succeeded, non-zero error code on failure.
 */
NUR_API
int NURAPICONV NurApiConnectSerialPortEx(HANDLE hApi, const TCHAR *portName, int baudRate);

/** @fn int NurApiConnectSocket(HANDLE hApi, const TCHAR *ip, int port)
 * Connect to NUR module using TCP/IP transport.
 *
 * @sa NurApiConnectSerialPort(), NurApiConnectSocket(), NurApiConnectTransport(), NurApiConnect(), NurApiConnectTransportSpec(), 
 * 	   NurApiIsConnected(), NurApiDisconnect()
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	ip			IP address of module.
 * @param	port		TCP/IP port number.
 * @return	Zero if connection succeeded, non-zero error code on failure.
 */
NUR_API
int NURAPICONV NurApiConnectSocket(HANDLE hApi, const TCHAR *ip, int port);

/** @fn int NurApiStopServer(HANDLE hApi)
 * Stop TCP/IP server. Server thread exit. Client connections will remains if any.
 *
 * @sa NurApiStartServer(),NUR_NOTIFICATION_CLIENTCONNECTED, NUR_NOTIFICATION_CLIENTDISCONNECTED
 *
 * @param	hApi		Handle to valid NurApi object instance.
 * @return	Zero if server stopped succesfully, non-zero error code on failure.
 */
NUR_API
int NURAPICONV NurApiStopServer(HANDLE hApi);

/** @fn int NurApiStartServer(HANDLE hApi,int port,int maxClients)
 * Start TCP/IP server for listening incoming connections. 
 * When client connects, "NUR_NOTIFICATION_CLIENTCONNECTED" notification sent
 * When client disconnects "NUR_NOTIFICATION_CLIENTDISCONNECTED" notification sent
 * @sa NurApiStopServer(), NUR_NOTIFICATION_CLIENTCONNECTED, NUR_NOTIFICATION_CLIENTDISCONNECTED
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	port		Listening port
 * @param	maxClients	Maximum number of clients allowing to connect. If value 0, then no restrictions how many clients will connect.
 * @return	Zero if server started succesfully, non-zero error code on failure.
 */
NUR_API
int NURAPICONV NurApiStartServer(HANDLE hApi,int port,int maxClients);

/** @fn int NurApiConnectUsb(HANDLE hApi, const TCHAR *devpath)
 *
 * Connect to NUR module using USB transport.
 *
 * @sa NurApiConnectSerialPort(), NurApiConnectSocket(), NurApiConnectTransport(), NurApiConnect(), NurApiConnectTransportSpec(), 
 * 	   NurApiIsConnected(), NurApiDisconnect(), NurUSBEnumerateDevices()
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	devpath		Device path returned by NurUSBEnumerateDevices() function.
 *
 * @return	0 if connection succeeded, non-zero error code on failure.
 */
NUR_API
int NURAPICONV NurApiConnectUsb(HANDLE hApi, const TCHAR *devpath);

/** @fn int NurApiSetUsbAutoConnect(HANDLE hApi, BOOL useAutoConnect)
 * Enable USB auto connect functionality. 
 * When enabled NurApi will automatically connect NUR module device when USB is connected.
 * 
 * @sa NurApiGetUsbAutoConnect()
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	useAutoConnect	TRUE to enable, FALSE to disable.
 * 						
 * @return	0 if succeeded, non-zero error code on failure.
 */
NUR_API
int NURAPICONV NurApiSetUsbAutoConnect(HANDLE hApi, BOOL useAutoConnect);

// DWORD NURAPICONV NurUSBEnumerateDevices(NurUSBEnumDeviceFunction funcPtr, LPVOID arg)

/** @fn BOOL NurApiGetUsbAutoConnect()
 *
 * Get state of the USB auto connect.
 * 
 * @sa NurApiSetUsbAutoConnect()
 * 	   
 * @return	TRUE if USB autoconnect is enabled, FALSE if disabled.
 */
NUR_API
BOOL NURAPICONV NurApiGetUsbAutoConnect();

/** @fn int NurApiConnectIntegratedReader(HANDLE hApi)
 * 
 * Connect to a integrated reader in supported mobile devices.
 * Currently Nordic ID Merlin and Nordic ID Morphic are supported.
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * 							
 * @return	0 if succeeded, non-zero error code on failure.
 */
NUR_API
int NURAPICONV NurApiConnectIntegratedReader(HANDLE hApi);

/** @fn int NurApiConnectTransport(HANDLE hApi, const TCHAR *transportType, LPVOID connectSpec)
 * Connect to NUR module with named transport and connect spec.
 *
 * @sa NurApiConnectSerialPort(), NurApiConnectSocket(), NurApiConnectTransport(), NurApiConnect(), NurApiConnectTransportSpec(), 
 * 	   NurApiIsConnected(), NurApiDisconnect()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	transportType	Type of transport to create.
 * @param	connectSpec		Transport specific connect spec.
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiConnectTransport(HANDLE hApi, const TCHAR *transportType, LPVOID connectSpec);

/** @fn int NurApiConnectTransportSpec(HANDLE hApi, LPVOID connectSpec)
 * Connect transport layer with new connect spec.
 * NOTE: This will cause current transport to disconnect, if connected.
 *
 * @sa NurApiConnectSerialPort(), NurApiConnectSocket(), NurApiConnectTransport(), NurApiConnect(), NurApiConnectTransportSpec(), 
 * 	   NurApiIsConnected(), NurApiDisconnect()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	connectSpec		New transport specific connect spec.
 * @return	0 if connection succeeded, non-zero error code on failure.
 */
NUR_API
int NURAPICONV NurApiConnectTransportSpec(HANDLE hApi, LPVOID connectSpec);

/** @fn int NurApiDisconnect(HANDLE hApi)
 * Disconnect module connection.
 *
 * @sa NurApiConnectSerialPort(), NurApiConnectSocket(), NurApiConnectTransport(), NurApiConnect(), NurApiConnectTransportSpec(), 
 * 	   NurApiIsConnected(), NurApiDisconnect()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiDisconnect(HANDLE hApi);

/** @fn int NurApiPing(HANDLE hApi, TCHAR *resp)
 * Test module connectivity.
 * @param	hApi	Handle to valid NurApi object instance.
 * @param	resp	Pointer to a buffer that receives ping response. This parameter can be NULL. <b>NOTE: If not NULL, buffer must be atleast 16 TCHARs long</b>.
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiPing(HANDLE hApi, TCHAR *resp);

/** @fn int NurApiSetTitle(HANDLE hApi, TCHAR *title)
 * Set Title string to NUR module. Title can be used for giving frienly name to reader device. Support NUR Fw 2.6.0, later or NURL2
 * @param	hApi	Handle to valid NurApi object instance.
 * @param	title	Pointer to a buffer containing title string to NUR module. <b>NOTE: Set buffer size at least 32 TCHARs long</b>
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSetTitle(HANDLE hApi, TCHAR *title);

/** @fn int NurApiGetTitle(HANDLE hApi, TCHAR *title)
 * Get Title string from NUR module. Title can be used for giving frienly name to reader device. Support NUR Fw 2.6.0, later or NURL2
 * @param	hApi	Handle to valid NurApi object instance.
 * @param	title	Pointer to a buffer that receives title string from NUR module. <b>NOTE: Set buffer size at least 32 TCHARs long</b>
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetTitle(HANDLE hApi, TCHAR *title);


/** @fn int NurApiBeep(HANDLE hApi, int frequency, int time, int duty)
 * This will cause devices with beep functionality to beep.
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	frequency	Beep frequency in Hz. Range 500 - 4000.
 * @param	time		Beep duration in milliseconds. Range 20 - 1000.
 * @param	duty		Beeper duty cycle, 50 is loudest. Range 5 - 95.
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiBeep(HANDLE hApi, int frequency, int time, int duty);

/** @fn int NurApiGetBaudrate(HANDLE hApi, int *setting, int *realBaudrate)
 * Get NUR module baudrate setting.
 *
 * @sa NurApiSetBaudrate(), enum NUR_BAUDRATE
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	setting			Pointer to a integer that receives current NUR module baudrate setting. see enum NUR_BAUDRATE
 * @param	realBaudrate	Pointer to a integer that receives baudrate in bps.
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetBaudrate(HANDLE hApi, int *setting, int *realBaudrate);

/** @fn int NurApiSetCommTimeout(HANDLE hApi, DWORD tmo)
 * Set communication timeout.
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	tmo				Communication timeout in seconds. Range is 3...15.
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSetCommTimeout(HANDLE hApi, DWORD tmo);

/** @fn int NurApiGetCommTimeout(HANDLE hApi, DWORD *tmo)
 * Get communication timeout.
 *
 * @param	hApi			Valid NUR API handle.
 * @param	tmo				Pointer to where the current communication timeout is stored to. Unit is seconds.
 * 					
 * @return	TRUE if pointer parameter is not NULL and API handle is valid.
 */
NUR_API
BOOL NURAPICONV NurApiGetCommTimeout(HANDLE hApi, DWORD *tmo);

/** @fn int NurApiBuildCustomHoptable(HANDLE hApi, DWORD base, DWORD nChan, DWORD chSpace, DWORD chTime, DWORD pauseTime, DWORD lf, DWORD Tari, BOOL shuffle)
 * Build a custom hoptable and store it into the module.
 *
 * @param	hApi		Handle to valid NurApi object instance.
 * @param base      	1st channel's center frequency in kHz
 * @param nChan     	Number of channels 
 * @param chSpace   	Channel width in kHz
 * @param chTime    	Channel time
 * @param pauseTime  	Idle time between channel change if any.
 * @param lf        	Maximum link frequency.
 * @param Tari      	Tari definition; 1 = 12.5 and 2 = 25us.
 * @param shuffle   	API shuffles the resulting table.
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiBuildCustomHoptable(HANDLE hApi, DWORD base, DWORD nChan, DWORD chSpace, DWORD chTime, DWORD pauseTime, DWORD lf, DWORD Tari, BOOL shuffle);

/** @fn int NurApiSetCustomHoptable(HANDLE hApi, DWORD *freqs, DWORD nChan, DWORD chTime, DWORD pauseTime, DWORD lf, DWORD Tari)
 * Set pre-built custom hop table.
 *
 * @param hApi			Handle to valid NurApi object instance.
 * @param freqs			Pointer to the frequencies.
 * @param nChan			Number of channels 
 * @param chTime		Channel time
 * @param pauseTime		Idle time between channel change if any.
 * @param lf			Maximum link frequency.
 * @param Tari			Tari definition; 1 = 12.5 and 2 = 25us.

 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSetCustomHoptable(HANDLE hApi, DWORD *freqs, DWORD nChan, DWORD chTime, DWORD pauseTime, DWORD lf, DWORD Tari);

/** @fn int NurApiSetCustomHoptableEx(HANDLE hApi, DWORD *freqs, DWORD nChan, DWORD chTime, DWORD pauseTime, DWORD lf, DWORD Tari, int lbtThresh, DWORD maxTxLevel)
 *
 * Set pre-built custom hop table.
 *
 * @param hApi			Handle to valid NurApi object instance.
 * @param freqs			Pointer to the frequencies.
 * @param nChan			Number of channels 
 * @param chTime		Channel time
 * @param pauseTime		Idle time between channel change if any.
 * @param lf			Maximum link frequency.
 * @param Tari			Tari definition; 1 = 12.5 and 2 = 25us.
 * @param lbtThresh		LBT threshold if used (minimum = -90).
 * @param maxTxLevel	Maximum TX level. Range is 0...19.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSetCustomHoptableEx(HANDLE hApi, DWORD *freqs, DWORD nChan, DWORD chTime, DWORD pauseTime, DWORD lf, DWORD Tari, int lbtThresh, DWORD maxTxLevel);


/** @fn int NurApiGetCustomHoptable(HANDLE hApi, struct NUR_CUSTOMHOP_PARAMS *chp);
 * Get current custom hoptable stored to the module if any.
 *
 * @sa NUR_CUSTOMHOP_PARAMS, NurApiSetCustomHoptable()
 * 
 * @param	hApi	Handle to valid NurApi object instance.
 * @param	chp		Pointer to the NUR_CUSTOMHOP_PARAMS structure.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetCustomHoptable(HANDLE hApi, struct NUR_CUSTOMHOP_PARAMS *chp);

/** @fn int NurApiGetCustomHoptableEx(HANDLE hApi, struct NUR_CUSTOMHOP_PARAMS_EX *chp);
 * Get current custom hoptable stored to the module if any.
 *
 * @sa NUR_CUSTOMHOP_PARAMS_EX, NurApiSetCustomHoptable()
 * 
 * @param	hApi	Handle to valid NurApi object instance.
 * @param	chp		Pointer to the NUR_CUSTOMHOP_PARAMS_EX structure.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetCustomHoptableEx(HANDLE hApi, struct NUR_CUSTOMHOP_PARAMS_EX *chp);

/** @fn int NurApiSetBaudrate(HANDLE hApi, int setting)
 * Set NUR module baudrate setting.
 *
 * @sa NurApiGetBaudrate(), enum NUR_BAUDRATE
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	setting		New NUR module baudrate setting. see enum NUR_BAUDRATE
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSetBaudrate(HANDLE hApi, int setting);

/** @fn int NurApiSetSensorConfig(HANDLE hApi, struct NUR_SENSOR_CONFIG *sensors, DWORD sensorsSize)
 * Set NUR module sensor configuration.
 *
 * @sa NurApiSetSensorConfig()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	sensors			Pointer to a sensor configuration structure.
 * @param	sensorsSize		Size of <i>sensors</i> structure in bytes. Usually: sizeof(struct NUR_SENSOR_CONFIG).
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSetSensorConfig(HANDLE hApi, struct NUR_SENSOR_CONFIG *sensors, DWORD sensorsSize);

/** @fn int NurApiGetSensorConfig(HANDLE hApi, struct NUR_SENSOR_CONFIG *sensors, DWORD sensorsSize)
 * Get NUR module sensor configuration.
 *
 * @sa NurApiGetSensorConfig()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	sensors			Pinter to a sensor configuration structure that receives module current GPIO configuration.
 * @param	sensorsSize		Size of <i>sensors</i> structure in bytes. Usually: sizeof(struct NUR_SENSOR_CONFIG).
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetSensorConfig(HANDLE hApi, struct NUR_SENSOR_CONFIG *sensors, DWORD sensorsSize);

/** @fn int NurApiSetGPIOConfig(HANDLE hApi, struct NUR_GPIO_CONFIG *gpio, DWORD gpioSize)
 * Set NUR module GPIO configuration.
 *
 * @sa NurApiGetGPIOConfig()
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	gpio		Pointer to the GPIO configuration.
 * @param	gpioSize	Size of <i>gpio</i> structure in bytes. Usually: sizeof(struct NUR_GPIO_CONFIG).
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSetGPIOConfig(HANDLE hApi, struct NUR_GPIO_CONFIG *gpio, DWORD gpioSize);

/** @fn int NurApiGetGPIOConfig(HANDLE hApi, struct NUR_GPIO_CONFIG *gpio, DWORD gpioSize)
 * Get NUR module GPIO configuration.
 *
 * @sa NurApiSetGPIOConfig()
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	gpio		Pointer to the GPIO configuration.
 * @param	gpioSize	Size of <i>gpio</i> structure in bytes. Usually: sizeof(struct NUR_GPIO_CONFIG).
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetGPIOConfig(HANDLE hApi, struct NUR_GPIO_CONFIG *gpio, DWORD gpioSize);

/** @fn int NurApiSetGPIOStatus(HANDLE hApi, int gpio, BOOL state)
 * Set single GPIO pin state.
 * <b>NOTE: GPIO must be configured as output</b>
 *
 * @sa NurApiSetGPIOStatus(), NUR_GPIO
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	gpio		Zero based GPIO number. see NUR_GPIO
 * @param	state		State to set. TRUE = high, FALSE = low.
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSetGPIOStatus(HANDLE hApi, int gpio, BOOL state);

/** @fn int NurApiGetGPIOStatus(HANDLE hApi, int gpio, struct NUR_GPIO_STATUS *status, DWORD statusSize)
 * Get single GPIO pin state.
 *
 * @sa NurApiGetGPIOStatus(), NUR_GPIO_STATUS, NUR_GPIO
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	gpio		Zero based GPIO number. see NUR_GPIO
 * @param	status		Status of the GPIO pin including enable, direction and low/high information.
 * @param	statusSize	Size of <i>status</i> structure in bytes. Usually: sizeof(struct NUR_GPIO_STATUS).
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetGPIOStatus(HANDLE hApi, int gpio, struct NUR_GPIO_STATUS *status, DWORD statusSize);

/** @fn int NurApiGetRegionInfo(HANDLE hApi, int regionId, struct NUR_REGIONINFO *ri, DWORD riSize)
 * Get NUR module specific region's information.
 *
 * @sa NurApiGetReaderInfo()
 * 	   
 * @param	hApi		Handle to valid NurApi object instance
 * @param	regionId	Region ID. If set to -1, module current region ID is used.
 * @param	ri			Pointer to a NUR_REGIONINFO structure that receives region information.
 * @param	riSize		Size of <i>ri</i> structure in bytes. Usually: sizeof(struct NUR_REGIONINFO).	
 * 					  
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetRegionInfo(HANDLE hApi, int regionId, struct NUR_REGIONINFO *ri, DWORD riSize);

/** @fn int NurApiGetReaderInfo(HANDLE hApi, struct NUR_READERINFO *ri, DWORD riSize)
 * Get general information about the reader.
 * 
 * @sa NurApiGetRegionInfo()
 *
 * @param	hApi	Handle to valid NurApi object instance
 * @param	ri		Pointer to the NUR_READERINFO structure.
 * @param	riSize	Size of <i>ri</i> structure in bytes. Usually: sizeof(struct NUR_READERINFO).	
 * 						
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetReaderInfo(HANDLE hApi, struct NUR_READERINFO *ri, DWORD riSize);

/** @fn int NurApiScanSingle(HANDLE hApi, int timeout, struct NUR_TRIGGERREAD_DATA *resp)
 * The single scan is used to read a single tag's EPC contents using a timeout defined in milliseconds.
 * 
 * @remarks If there's more than one tag in the reader range, this function will most likely fail.
 * 			
 * @sa NUR_TRIGGERREAD_DATA, NurApiSimpleInventory()
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	timeout		Timeout in milliseconds. Range 50 - 2000.
 * @param	resp		Pointer to a structure that receives read tag information.
 * 			
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiScanSingle(HANDLE hApi, int timeout, struct NUR_TRIGGERREAD_DATA *resp);

/** @fn int NurApiSimpleInventory(HANDLE hApi, struct NUR_INVENTORY_RESPONSE *resp)
 * Perform Inventory command. 
 * This function uses rounds, Q and session values from current module settings.
 * 
 * @sa NurApiInventory(), NurApiInventorySelect32(), NurApiInventorySelectByEPC(), NUR_INVENTORY_RESPONSE
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	resp		Pointer to a structure that receives inventory response.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSimpleInventory(HANDLE hApi, struct NUR_INVENTORY_RESPONSE *resp);

/** @fn int NurApiInventory(HANDLE hApi, int rounds, int Q, int session, struct NUR_INVENTORY_RESPONSE *resp)
 * Perform Inventory command.
 *
 * @sa NurApiSimpleInventory(), NurApiInventorySelect32(), NurApiInventorySelectByEPC(), NUR_INVENTORY_RESPONSE
 * 	   
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	rounds			Full query rounds to perform in inventory. Range 0 - 10. Value 0 means automatic round number.
 * @param	Q				The Q parameter. Range 0 - 15. Value 0 means automatic Q calculation.
 * @param	session			The session parameter. Range 0 - 3.
 * @param	resp			Pointer to a structure that receives inventory response.
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiInventory(HANDLE hApi, int rounds, int Q, int session, struct NUR_INVENTORY_RESPONSE *resp);

/** @fn int NurApiSetInventoryPeriod(HANDLE hApi, int level, BOOL save)
 * Configure the inventory stream's duty cycle.
 *
 * @sa enum NUR_AUTOPERIOD
 * 	   
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	level			Defined by NUR_AUTOPERIOD: 0...3.
 * @param	save			If set to TRUE then the module stores this setting into its non-volatile memory.
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSetInventoryPeriod(HANDLE hApi, int level, BOOL save);

/** @fn int NurApiGetInventoryPeriod(HANDLE hApi, int *level)
 * Get the current duty cycle for inventory stream.
 *
 * @sa enum NUR_AUTOPERIOD
 * 	   
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	level			Pointer to where the currnet level is to be store. Must not be NULL.
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetInventoryPeriod(HANDLE hApi, int *level);

/** @fn int NurApiInventorySelect32(HANDLE hApi, int rounds, int Q, int session, BOOL invertSelect,
							BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							struct NUR_INVENTORY_RESPONSE *resp)
 *
 * Perform Inventory command with specific tag singulation.
 * Tags can be singluted against desired memory bank and data mask.
 *
 * @sa NurApiSimpleInventory(), NurApiInventory(), NurApiInventorySelectByEPC(), NUR_INVENTORY_RESPONSE
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	rounds				Full query rounds to perform. Range 0 - 10. Value 0 means automatic round number.
 * @param	Q					The Q parameter. Range 0 - 15. Value 0 means automatic Q calculation.
 * @param	session				The session parameter. Accepted range is 0 - 3.
 * @param	invertSelect		Invert select. Select tags that do not match the selection criteria.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	resp				Pointer to a inventory response structure.
 * 							
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiInventorySelect32(HANDLE hApi, int rounds, int Q, int session, BOOL invertSelect,
							BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							struct NUR_INVENTORY_RESPONSE *resp);

/** @fn int NurApiInventorySelectByEPC(HANDLE hApi, int rounds, int Q, int session, BOOL invertSelect,
							  BYTE *epcBuffer, DWORD epcBufferLen,
							  struct NUR_INVENTORY_RESPONSE *resp)
 *
 * Perform Inventory command with tag singulated against EPC.
 *
 * @sa NurApiSimpleInventory(), NurApiInventory(), NurApiInventorySelect32(), NUR_INVENTORY_RESPONSE
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	rounds				Full query rounds to perform. Range 0 - 10. Value 0 means automatic round number.
 * @param	Q					The Q parameter. Range 0 - 15. Value 0 means automatic Q calculation.
 * @param	session				The session parameter. Accepted range is 0 - 3.
 * @param	invertSelect		Invert select. Select tags that do not match the selection criteria.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 * @param	resp				Pointer to a inventory response structure.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiInventorySelectByEPC(HANDLE hApi, int rounds, int Q, int session, BOOL invertSelect,
							  BYTE *epcBuffer, DWORD epcBufferLen,
							  struct NUR_INVENTORY_RESPONSE *resp);

/** @fn int NurApiStartInventoryStream(HANDLE hApi, int rounds, int Q, int session)
 *
 * Start inventory streaming on module. 
 * Module will start continuos inventory and sends new tags to host as notification.
 * By default notification is sent only when there's tag(s) available, if you need zero round information
 * it can configured with setup opFlags (enum NUR_OPFLAGS).
 *
 * @sa NurApiStopInventoryStream(), NUR_INVENTORYSTREAM_DATA, NUR_OPFLAGS
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	rounds				Full query rounds to perform. Range 0 - 10. Value 0 means automatic round number.
 * @param	Q					The Q parameter. Range 0 - 15. Value 0 means automatic Q calculation.
 * @param	session				The session parameter. Accepted range is 0 - 3.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStartInventoryStream(HANDLE hApi, int rounds, int Q, int session);

/** @fn int NurApiStartSimpleInventoryStream(HANDLE hApi)
 *
 * Start inventory streaming on module. 
 * Module will start continuos inventory and sends new tags to host as notification.
 * By default notification is sent only when there's tag(s) available, if you need zero round information
 * it can configured with setup opFlags (enum NUR_OPFLAGS).
 * <b>This function uses rounds, Q and session values from current module settings.</b>
 *
 * @sa NurApiStopInventoryStream(), NUR_INVENTORYSTREAM_DATA, NUR_OPFLAGS
 *
 * @param	hApi	Handle to valid NurApi object instance
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStartSimpleInventoryStream(HANDLE hApi);

/** @fn int NurApiStartInventorySelectStream(HANDLE hApi, int rounds, int Q, int session, BOOL invertSelect,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask)
 *
 * Start inventory streaming on module. 
 * Module will start continuos inventory and sends new tags to host as notification.
 * By default notification is sent only when there's tag(s) available, if you need zero round information
 * it can configured with setup opFlags (enum NUR_OPFLAGS).
 * Tags can be singluted against desired memory bank and mask.
 *
 * @sa NurApiStopInventoryStream(), NUR_INVENTORYSTREAM_DATA
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	rounds				Full query rounds to perform. Range 0 - 10. Value 0 means automatic round number.
 * @param	Q					The Q parameter. Range 0 - 15. Value 0 means automatic Q calculation.
 * @param	session				The session parameter. Accepted range is 0 - 3.
 * @param	invertSelect		Invert select. Select tags that do not match the selection criteria.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * 							
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiStartInventorySelectStream(HANDLE hApi, int rounds, int Q, int session, BOOL invertSelect,
							BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask);

/** @fn int NurApiStopInventoryStream(HANDLE hApi)
 *
 * Stop inventory streaming on module. 
 *
 * @param	hApi	Handle to valid NurApi object instance
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStopInventoryStream(HANDLE hApi);

/** @fn BOOL NurApiIsInventoryStreamRunning(HANDLE hApi)
 *
 * Return TRUE if inventory stream routine is running on module.
 *
 * @param	hApi	Handle to valid NurApi object instance
 *
 * @return	TRUE if inventory stream routine is running. FALSE otherwise.
 */
NUR_API
BOOL NURAPICONV NurApiIsInventoryStreamRunning(HANDLE hApi);

/** @fn int NurApiStartPeriodicInventory(HANDLE hApi, int rounds, int Q, int session)
 * Start periodic inventory command.
 * Inventory commands are executed periodically in loop. 
 * Results are sent with the notification NUR_NOTIFICATION_PERIODIC_INVENTORY.
 * 
 * @sa NurApiStopPeriodicInventory(), NurApiIsPeriodicInventoryRunning(), NurApiStartInventoryStream()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	rounds			Full query rounds to perform. Range 0 - 10. Value 0 means automatic round number.
 * @param	Q				The Q parameter. Range 0 - 15. Value 0 means automatic Q calculation.
 * @param	session			The session parameter. Accepted range is 0 - 3.
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStartPeriodicInventory(HANDLE hApi, int rounds, int Q, int session);

/** @fn int NurApiStopPeriodicInventory(HANDLE hApi, BOOL waitExit)
 * Stop periodic inventory command.
 *
 * @sa NurApiStartPeriodicInventory(), NurApiIsPeriodicInventoryRunning()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	waitExit		Wait periodic inventory thread exit.
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStopPeriodicInventory(HANDLE hApi, BOOL waitExit);

/** @fn BOOL NurApiIsPeriodicInventoryRunning(HANDLE hApi)
 * Return TRUE if periodic inventory routine is running.
 *
 * @sa NurApiStartPeriodicInventory(), NurApiStopPeriodicInventory()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @return	TRUE if periodic inventory routine is running. FALSE otherwise.
 */
NUR_API
BOOL NURAPICONV NurApiIsPeriodicInventoryRunning(HANDLE hApi);

/** @fn int NurApiInventoryEx(HANDLE hApi, struct NUR_INVEX_PARAMS *params,
								 struct NUR_INVEX_FILTER *filters, int filtersCount,
								 struct NUR_INVENTORY_RESPONSE *resp)
 * Extended inventory command with multiple filters.
 *
 * @sa struct NUR_INVEX_PARAMS, struct NUR_INVEX_FILTER
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	params			Pointer to inventory parameters.
 * @param	filters			Pointer to filter array.
 * @param	filtersCount	Number of filter structs in filters array.
 * @param	resp			Pointer to a inventory response structure.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiInventoryEx(HANDLE hApi,
								 struct NUR_INVEX_PARAMS *params,
								 struct NUR_INVEX_FILTER *filters, int filtersCount,
								 struct NUR_INVENTORY_RESPONSE *resp);

/** @fn int NurApiRerunInventoryEx(HANDLE hApi, struct NUR_INVENTORY_RESPONSE *resp)
 * 
 * Re-run te extended inventory with last set parameters. Requires at least one successful call to single extended inventory or 
 * extended inventory stream prior to this call.
 *
 * @sa struct NUR_INVENTORY_RESPONSE
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	resp			Pointer to a inventory response structure.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
int NURAPICONV NurApiRerunInventoryEx(HANDLE hApi, struct NUR_INVENTORY_RESPONSE *resp);

/** @fn int NurApiStartInventoryEx(HANDLE hApi, struct NUR_INVEX_PARAMS *params,
								 struct NUR_INVEX_FILTER *filters, int filtersCount)
 * Start extended inventory streaming command with multiple filters.
 *
 * @sa struct NUR_INVEX_PARAMS, struct NUR_INVEX_FILTER
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	params			Pointer to inventory parameters.
 * @param	filters			Pointer to filter array.
 * @param	filtersCount	Number of filter structs in filters array.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStartInventoryEx(HANDLE hApi,
								 struct NUR_INVEX_PARAMS *params,
								 struct NUR_INVEX_FILTER *filters, int filtersCount);

/** @fn int NurApiStopInventoryEx(HANDLE hApi)
 * Stop extended inventory streaming in module.
 *
 * @sa struct NUR_INVEX_PARAMS, struct NUR_INVEX_FILTER
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStopInventoryEx(HANDLE hApi);


/** @fn BOOL NurApiIsInventoryExRunning(HANDLE hApi)
 * Return TRUE if streaming extended inventory routine is running.
 *
 * @sa NurApiStartInventoryEx(), NurApiStopInventoryEx()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @return	TRUE if streaming extended inventory routine is running. FALSE otherwise.
 */
NUR_API
BOOL NURAPICONV NurApiIsInventoryExRunning(HANDLE hApi);


/** @fn BOOL NurApiTIDInventory(HANDLE hApi, DWORD firstWord, DWORD nWords)
 *
 * Configures current inventory so that the TID data is returned instead of EPC in specified manner.
 *
 * @sa NurApiUserMemInventory(), NurApiInventoryRead() 
 * @sa NurApiInventoryRead(), NurApiGetInventoryRead(), NurApiInventoryReadCtl()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	firstWord		First word to read from the TID memory.
 * @param	nWords			Number of 16-bit words to read. Range is 1...32.
 *
 * @return	Zero when successful, error code otherwise.
 */
NUR_API
int NURAPICONV NurApiTIDInventory(HANDLE hApi, DWORD firstWord, DWORD nWords);

/** @fn BOOL NurApiUserMemInventory(HANDLE hApi, DWORD firstWord, DWORD nWords)
 * Configures current inventory so that the user memory data is returned instead of EPC in specified manner.
 *
 * @sa NurApiTIDInventory(), NurApiInventoryRead() 
 * @sa NurApiInventoryRead(), NurApiGetInventoryRead(), NurApiInventoryReadCtl()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	firstWord		First word to read from the user memory.
 * @param	nWords			Number of 16-bit words to read. Range is 1..32.
 *
 * @return	Zero when successful, error code otherwise.
 */
NUR_API
int NURAPICONV NurApiUserMemInventory(HANDLE hApi, DWORD firstWord, DWORD nWords);

/** @fn BOOL NurApiInventoryRead(HANDLE hApi, BOOL on, DWORD dwType, DWORD dwBank, DWORD dwAddress, DWORD nWords)
 * Configures following inventoryies to do an inventory + read operation in a specified manner.
 *
 * @sa NurApiTIDInventory(), NurApiUserMemInventory()
 * @sa NurApiGetInventoryRead(), NurApiInventoryReadCtl()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	on				On / off control; TRUE means on.
 * @param	dwType			Type for inventory + read: 0 = EPC + data, 1 = data only (returned in the EPC field).
 * @param	dwBank			Bank to read from; allowed banks are EPC (1), TID (2) and user memory (3).
 * @param	dwAddress		Word address to start the reading from.
 * @param	nWords			Number of 16-bit words to read. Range is 1..32.
 *
 * @return	Zero when successful, error code otherwise.
 */
NUR_API
int NURAPICONV NurApiInventoryRead(HANDLE hApi, BOOL on, DWORD dwType, DWORD dwBank, DWORD dwAddress, DWORD nWords);

/** @fn BOOL NurApiGetInventoryRead(HANDLE hApi, struct NUR_IRINFORMATION *pIrInfo)
 * Get current inventory + read configuration.
 *
 * @sa NUR_IRINFORMATION
 * @sa NurApiTIDInventory(), NurApiUserMemInventory()
 * @sa NurApiInventoryRead(), NurApiInventoryReadCtl()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	pIrInfo			Pointer to NUR_IRINFORMATION structure.
 *
 * @return	Zero when successful, error code otherwise.
 */
NUR_API
int NURAPICONV NurApiGetInventoryRead(HANDLE hApi, struct NUR_IRINFORMATION *pIrInfo);

/** @fn BOOL NurApiInventoryReadCtl(HANDLE hApi, BOOL on)
 * Control inventory + read; on / off.
 *
 * @sa NurApiTIDInventory(), NurApiUserMemInventory(),
 * @sa NurApiInventoryRead(), NurApiGetInventoryRead()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	on				Set to TRUE to use inventory + read the wayt it is currently confgured.
 *
 * @return	Zero when successful, error code otherwise.
 */
NUR_API
int NURAPICONV NurApiInventoryReadCtl(HANDLE hApi, BOOL on);


/** @fn int NurApiFetchTags(HANDLE hApi, BOOL includeMeta, int *tagsAdded)
 * Fetch all inventoried tags from module.
 *
 * @sa NurApiClearTags(), NurApiGetTagCount(), NurApiGetTagData()
 *
 * @param	hApi	   	Handle to valid NurApi object instance.
 * @param	includeMeta	TRUE to include meta data, FALSE to exclude. If TRUE, NUR_TAG_DATA will contain all tag meta information.
 * @param	tagsAdded	Number of tags added in internal tag buffer by this fetch is stored here.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiFetchTags(HANDLE hApi, BOOL includeMeta, int *tagsAdded);

/** @fn int NurApiFetchTagAt(HANDLE hApi, BOOL includeMeta, int tagNum, struct NUR_TAG_DATA_EX *pData, DWORD szData)
 * Fetch a single tag indicated by the tag number.
 *
 * @sa NurApiGetTagCount()
 *
 * @param	hApi	   	Handle to valid NurApi object instance.
 * @param	includeMeta	TRUE to include meta data, FALSE to exclude. If TRUE, NUR_TAG_DATA will contain all tag meta information.
 * @param	tagNum		Tag number to retrieve (not an actual index).
 * @param	pData		Pointer to the NUR_TAG_DATA_EX structure where the data is stored.
 * @param	szData		Size of the data structure.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiFetchTagAt(HANDLE hApi, BOOL includeMeta, int tagNum, struct NUR_TAG_DATA_EX *pData, DWORD szData);

/** @fn int NurApiClearTags(HANDLE hApi)
 * Clear previously inventoried tags from memory.
 *
 * @sa NurApiClearTags(), NurApiGetTagCount(), NurApiGetTagData()
 *
 * @param	hApi	Handle to valid NurApi object instance.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiClearTags(HANDLE hApi);

/** @fn int NurApiEnableXPCRemoval(HANDLE hApi, BOOL enable)
 *
 * Enables or disables the XPC information removal when the tags are retrieved from the storage.
 * When the removal is enabled, the XPC indicator bit will also be set to '0' in the PC word.
 * 
 * @sa NurApiGetTagMeta(), NurApiGetTagData(), NurApiGetTagDataEx()
 *
 * @param	hApi	  Handle to valid NurApi object instance.
 * @param	enable  Set to TRUE if the XPC removal is to be enabled.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiEnableXPCRemoval(HANDLE hApi, BOOL enable);

/** @fn int NurApiXPCRemovalEnabled(HANDLE hApi, BOOL *enable)
 * 
 * Returns whether the XPC removal is currently enabled or not.
 * 
 * @sa NurApiGetTagMeta(), NurApiGetTagData(), NurApiGetTagDataEx()
 *
 * @param	hApi	Handle to valid NurApi object instance.
 * @param	enable	Pointer to where the state is to be stored.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiXPCRemovalEnabled(HANDLE hApi, BOOL *enable);

/** @fn int NurApiGetTagCount(HANDLE hApi, int *count)
 * Get count of inventoried tags in memory. 
 * You have to call NurApiFetchTags() in order to get fresh tag list from module.
 * 
 * @sa NurApiFetchTags(), NurApiClearTags(), NurApiGetTagMeta(), NurApiGetTagData(), NurApiGetTagDataEx()
 *
 * @param	hApi	Handle to valid NurApi object instance.
 * @param	count	Number of tags fetched from module memory is received in this pointer.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetTagCount(HANDLE hApi, int *count);

/** @fn int NurApiGetTagData(HANDLE hApi, int idx, struct NUR_TAG_DATA *tagData)
 *
 * Get inventoried tag data by index.
 * 
 * @sa NurApiGetTagCount(), NurApiClearTags(), NUR_TAG_DATA, NurApiGetAllTagData()
 * 
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	idx			Tag index in internal tag buffer.
 * @param	tagData		Valid pointer NUR_TAG_DATA structure.
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetTagData(HANDLE hApi, int idx, struct NUR_TAG_DATA *tagData);

/** @fn int NurApiGetTagDataEx(HANDLE hApi, int idx, struct NUR_TAG_DATA_EX *tagDataEx, DWORD szEntry)
 *
 * Get inventoried tag data by index. Includes possible read data.
 * 
 * @sa NurApiGetTagCount(), NurApiClearTags(), NUR_TAG_DATA_EX, NurApiGetAllTagDataEx()
 * 
 * @param	hApi		    Handle to valid NurApi object instance.
 * @param	idx			    Tag index in internal tag buffer.
 * @param	tagDataEx		Valid pointer NUR_TAG_DATA_EX structure.
 * @param	szEntry     	Size of the entry. May change upon new version(s).
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetTagDataEx(HANDLE hApi, int idx, struct NUR_TAG_DATA_EX *tagDataEx, DWORD szEntry);

/** @fn int NurApiGetAllTagData(HANDLE hApi, struct NUR_TAG_DATA *tagDataBuffer, int *tagDataCount)
 *
 * Get all inventoried tags once.
 * 
 * @sa NurApiGetTagCount(), NurApiClearTags(), NUR_TAG_DATA, NurApiGetTagData()
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	tagDataBuffer	Pointer to a NUR_TAG_DATA structures. Must contain at least <i>tagDataCount</i> entries of NUR_TAG_DATA.
 * @param	tagDataCount	Number of entries in <i>tagDataBuffer</i>. On return number of valid entries is received in this pointer.
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetAllTagData(HANDLE hApi, struct NUR_TAG_DATA *tagDataBuffer, int *tagDataCount);

/** @fn int NurApiGetAllTagDataEx(HANDLE hApi, struct NUR_TAG_DATA_EX *tagDataBuffer, int *tagDataCount, DWORD szSingleEntry)
 *
 * Get all inventoried tags once. Include possible intentory+read data per tag basis.
 * 
 * @sa NurApiGetTagCount(), NurApiClearTags(), NUR_TAG_DATA_EX, NurApiGetTagDataEx()
 * 
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	tagDataBuffer		Pointer to a NUR_TAG_DATA_EX structures. Must contain at least <i>tagDataCount</i> entries of NUR_TAG_DATA.
 * @param	tagDataCount		Number of entries in <i>tagDataBuffer</i>. On return number of valid entries is received in this pointer.
 * @param	szSingleEntry		Size fo one NUR_TAG_DATA_EX entry.
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetAllTagDataEx(HANDLE hApi, struct NUR_TAG_DATA_EX *tagDataBuffer, int *tagDataCount, DWORD szSingleEntry);

/** @fn int NurApiLockTagStorage(HANDLE hApi, BOOL lock)
 *
 * Lock the tag storage in order to prevent simultaneous multiple thread access.
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	lock	    TRUE to lock, FALSE to unlock.
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiLockTagStorage(HANDLE hApi, BOOL lock);

/** @fn int NurApiStopContinuous(HANDLE hApi)
 *
 * Force stop all NUR module running continuous functions.
 * 
 * @param	hApi			Handle to valid NurApi object instance.
 * 							
 * @return	0 if succeeded, non-zero error code on failure.
 */
NUR_API
int NURAPICONV NurApiStopContinuous(HANDLE hApi);

/** @fn int NurApiTraceTag32(HANDLE hApi,
							 BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							 BYTE flags, struct NUR_TRACETAG_DATA *resp)
 *
 * Trace singulated single tag.
 * 
 * @sa NurApiTraceTagByEPC(), NUR_TRACETAG
 * 	   
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	flags				Flags for trace. see enum NUR_TRACETAG
 * @param	resp				Pointer to a structure that receives trace response. 
 * 								If continuous tracing is specified in <i>flags</i> response is not received, 
 * 								instead module will send NUR_NOTIFICATION_TRACETAG notification for tracing.
 * 								
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiTraceTag32(HANDLE hApi,
							 BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							 BYTE flags, struct NUR_TRACETAG_DATA *resp);

/** @fn int NurApiTraceTagByEPC(HANDLE hApi, BYTE *epcBuffer, DWORD epcBufferLen, BYTE flags, struct NUR_TRACETAG_DATA *resp)
 *
 * Trace single tag singluated against tag's EPC memory.
 * 
 * @sa NurApiTraceTag32(), NUR_TRACETAG
 * 
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 * @param	flags				Flags for trace. see enum NUR_TRACETAG
 * @param	resp				Pointer to a structure that receives trace response. 
 * 								If continuous tracing is specified in <i>flags</i> response is not received, 
 * 								instead module will send NUR_NOTIFICATION_TRACETAG notification for tracing.
 * 								
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiTraceTagByEPC(HANDLE hApi, BYTE *epcBuffer, DWORD epcBufferLen, BYTE flags, struct NUR_TRACETAG_DATA *resp);

/** @fn BOOL NurApiIsTraceRunning(HANDLE hApi)
 *
 * Return TRUE if tag trace routine is running on module.
 *
 * @param	hApi	Handle to valid NurApi object instance
 *
 * @return	TRUE if tag trace routine is running. FALSE otherwise.
 */
NUR_API
BOOL NURAPICONV NurApiIsTraceRunning(HANDLE hApi);

/** @fn int NurApiReadSingulatedTag32(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							  BYTE rdBank, DWORD rdAddress, int rdByteCount, BYTE *rdBuffer)

 * 	Read data from tag with specific singulation. 
 * 	Tag can be singluted against desired memory bank and mask.
 *
 * @sa NurApiReadTagByEPC(), NurApiReadTag32()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	rdBank				Memory bank for read operation.
 * @param	rdAddress			<b>Word</b> address for read operation.
 * @param	rdByteCount			Number of bytes to read. This must <b>divisible by two</b>.
 * @param	rdBuffer			Pointer to a buffer that received read data. Must be atleast <i>rdByteCount</i> bytes long.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiReadSingulatedTag32(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							  BYTE rdBank, DWORD rdAddress, int rdByteCount, BYTE *rdBuffer);

/** @fn int NurApiCustomReadSingulatedTag32(HANDLE hApi, 
              DWORD rdCmd, BYTE cmdBits, DWORD rdBank, BYTE bankBits,							 
              DWORD passwd, BOOL secured, 
							BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							DWORD rdAddress, int rdByteCount, BYTE *rdBuffer);

 * 	Read data from tag with specific singulation and with custom read command.
 *  The command can be practically anything; the module's action is "act like this was a read operation".
 * 	Tag can be singluted against desired memory bank and mask.
 *
 * @sa NurApiCustomReadTagByEPC(), NurApiCustomReadTag32()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	rdCmd				Custom read command.
 * @param	cmdBits			Number of bits in the 'custom' read command. Must be 1...32.
 * @param	rdBank			Customizable bank parameter for 'custom' read.
 * @param	bankBits		Number of bits in the bank parameter. Can be 0. 
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	rdAddress			<b>Word</b> address for read operation.
 * @param	rdByteCount			Number of bytes to read. This must <b>divisible by two</b>.
 * @param	rdBuffer			Pointer to a buffer that received read data. Must be at least <i>rdByteCount</i> bytes long.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiCustomReadSingulatedTag32(HANDLE hApi, 
              DWORD rdCmd, BYTE cmdBits, DWORD rdBank, BYTE bankBits,							 
              DWORD passwd, BOOL secured, 
							BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							DWORD rdAddress, int rdByteCount, BYTE *rdBuffer);

/** @fn int NurApiReadTagByEPC(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE *epcBuffer, DWORD epcBufferLen,
							  BYTE rdBank, DWORD rdAddress, int rdByteCount, BYTE *rdBuffer)

 * 	Read data from tag singulated by tag's EPC memory.
 * 	
 * @sa NurApiReadSingulatedTag32(), NurApiReadTag32()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 * @param	rdBank				Memory bank for read operation.
 * @param	rdAddress			<b>Word</b> address for read operation.
 * @param	rdByteCount			Number of bytes to read. This must <b>divisible by two</b>.
 * @param	rdBuffer			Pointer to a buffer that received read data. Must be atleast <i>rdByteCount</i> bytes long.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiReadTagByEPC(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE *epcBuffer, DWORD epcBufferLen,
							  BYTE rdBank, DWORD rdAddress, int rdByteCount, BYTE *rdBuffer);

/** @fn int NurApiCustomReadTagByEPC(HANDLE hApi, 
                DWORD rdCmd, BYTE cmdBits, DWORD rdBank, BYTE bankBits,
                DWORD passwd, BOOL secured,
							  BYTE *epcBuffer, DWORD epcBufferLen,
							  DWORD rdAddress, int rdByteCount, BYTE *rdBuffer)

 * 	Read data from tag singulated by tag's EPC memory.
 * 	
 * @sa NurApiCustomReadSingulatedTag32(), NurApiReadTag32()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	rdCmd				Custom read command.
 * @param	cmdBits			Number of bits in the 'custom' read command. Must be 1...32.
 * @param	rdBank			Customizable bank parameter for 'custom' read.
 * @param	bankBits		Number of bits in the bank parameter. Can be 0.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 * @param	rdBank				Memory bank for read operation.
 * @param	rdAddress			<b>Word</b> address for read operation.
 * @param	rdByteCount			Number of bytes to read. This must <b>divisible by two</b>.
 * @param	rdBuffer			Pointer to a buffer that received read data. Must be atleast <i>rdByteCount</i> bytes long.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiCustomReadTagByEPC(HANDLE hApi, 
                DWORD rdCmd, BYTE cmdBits, DWORD rdBank, BYTE bankBits,
                DWORD passwd, BOOL secured,
							  BYTE *epcBuffer, DWORD epcBufferLen,
							  DWORD rdAddress, int rdByteCount, BYTE *rdBuffer);

/** @fn int NurApiReadTag32(HANDLE hApi, DWORD passwd, BOOL secured, BYTE rdBank, DWORD rdAddress, int rdByteCount, BYTE *rdBuffer)
 * Read data from tag without any singulation data.
 * This function can be used for tags that does not have EPC. 
 * 	
 * @note If there's more than one tag in range, this function will most likely fail due to the RF collision.
 * 	
 * @sa NurApiReadSingulatedTag32(), NurApiReadTagByEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	rdBank				Memory bank for read operation.
 * @param	rdAddress			<b>Word</b> address for read operation.
 * @param	rdByteCount			Number of bytes to read. This must <b>divisible by two</b>.
 * @param	rdBuffer			Pointer to a buffer that received read data. Must be atleast <i>rdByteCount</i> bytes long.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiReadTag32(HANDLE hApi, DWORD passwd, BOOL secured, BYTE rdBank, DWORD rdAddress, int rdByteCount, BYTE *rdBuffer);

/** @fn int NurApiCustomReadTag32(HANDLE hApi, DWORD rdCmd, BYTE cmdBits, DWORD rdBank, BYTE bankBits, DWORD passwd, BOOL secured, DWORD rdAddress, int rdByteCount, BYTE *rdBuffer)
 * Read data from tag without any singulation data with custom read command and customizable bank parameter.
 * This function can be used for tags that does not have EPC. 
 * 	
 * @note If there's more than one tag in range, this function will most likely fail due to the RF collision.
 * 	
 * @sa NurApiCustomReadSingulatedTag32(), NurApiCustomReadTagByEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	rdCmd				Custom read command.
 * @param	cmdBits			Number of bits in the 'custom' read command. Must be 1...32.
 * @param	rdBank			Customizable bank parameter for 'custom' read.
 * @param	bankBits		Number of bits in the bank parameter. Can be 0.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	rdBank				Memory bank for read operation.
 * @param	rdAddress			<b>Word</b> address for read operation.
 * @param	rdByteCount			Number of bytes to read. This must <b>divisible by two</b>.
 * @param	rdBuffer			Pointer to a buffer that received read data. Must be atleast <i>rdByteCount</i> bytes long.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiCustomReadTag32(HANDLE hApi, 
            DWORD rdCmd, BYTE cmdBits, DWORD rdBank, BYTE bankBits,
            DWORD passwd, BOOL secured, DWORD rdAddress, int rdByteCount, BYTE *rdBuffer);

/** @fn int NurApiSetUseBlockWrite(HANDLE hApi, BOOL val)
 *
 * Set current block write usage flag.
 * When set to TRUE, all NurApi tag write functions will use UHF C1G2 BlockWrite command (uhf c1g2 spec available at epcglobal) to write data to tag.
 * UHF C1G2 standard section "6.3.2.11.3.7 BlockWrite (optional)".
 * 
 * @sa NurApiGetUseBlockWrite()
 * 
 * @param	hApi	Handle to valid NurApi object instance.
 * @param	val		If TRUE BlockWrite method is used.
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark Not all tags supports BlockWrite. If unsure, DO NOT USE.
 */
NUR_API
int NURAPICONV NurApiSetUseBlockWrite(HANDLE hApi, BOOL val);

/** @fn int NurApiDisableCustomReselect(HANDLE hApi)
 *
 * Call to this will disable the tag re-selection during next custom exchange.
 * Any following command will reset this flag so it is valid only for one round.
 * 
 * @sa NurApiCustomExchangeByEPC(), NurApiCustomExchange32(), NurApiCustomExchangeSingulated32(), NurApiSetExtCarrier()
 * 
 * @param	hApi	Handle to valid NurApi object instance.
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned. 
 */
NUR_API
int NURAPICONV NurApiDisableCustomReselect(HANDLE hApi);


/** @fn int NurApiGetUseBlockWrite(HANDLE hApi, BOOL *val)
 *
 * Get current block write usage flag.
 * When set to TRUE, all NurApi tag write functions will use UHF C1G2 BlockWrite command (uhf c1g2 spec available at epcglobal) to write data to tag.
 * UHF C1G2 standard section "6.3.2.11.3.7 BlockWrite (optional)".
 * 
 * @sa NurApiSetUseBlockWrite()
 * 
 * @param	hApi	Handle to valid NurApi object instance.
 * @param	val		Pointer to a BOOL that receives current setting.
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark Not all tags supports BlockWrite. If unsure, DO NOT USE.
 */
NUR_API
int NURAPICONV NurApiGetUseBlockWrite(HANDLE hApi, BOOL *val);

/** @fn int NurApiWriteSingulatedTag32(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							  BYTE wrBank, DWORD wrAddress, int wrByteCount, BYTE *wrBuffer)

 * 	Write data to tag with specific singulation. 
 * 	Tag can be singluted against desired memory bank and mask.
 * 	
 * @sa NurApiWriteTagByEPC(), NurApiWriteTag32()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	wrBank				Memory bank for write operation.
 * @param	wrAddress			<b>Word</b> address for write operation.
 * @param	wrByteCount			Number of bytes to write. This must <b>divisible by two</b>.
 * @param	wrBuffer			Data to write. Must be atleast <i>wrByteCount</i> bytes long.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiWriteSingulatedTag32(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							  BYTE wrBank, DWORD wrAddress, int wrByteCount, BYTE *wrBuffer);

/** @fn int NurApiWriteTagByEPC(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE *epcBuffer, DWORD epcBufferLen,
							  BYTE wrBank, DWORD wrAddress, int wrByteCount, BYTE *wrBuffer)

 * 	Write data to tag singulated by tag's EPC memory.
 *
 * @sa NurApiWriteSingulatedTag32(), NurApiWriteTag32()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 * @param	wrBank				Memory bank for write operation.
 * @param	wrAddress			<b>Word</b> address for write operation.
 * @param	wrByteCount			Number of bytes to write. This must <b>divisible by two</b>.
 * @param	wrBuffer			Data to write. Must be atleast <i>wrByteCount</i> bytes long.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiWriteTagByEPC(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE *epcBuffer, DWORD epcBufferLen,
							  BYTE wrBank, DWORD wrAddress, int wrByteCount, BYTE *wrBuffer);

/** @fn int NurApiWriteTag32(HANDLE hApi, DWORD passwd, BOOL secured,
					 BYTE wrBank, DWORD wrAddress, int wrByteCount, BYTE *wrBuffer)

 * Write data to tag without any singulation data.
 * This function can be used for tags that does not have EPC. 
 * 	
 * @note If there's more than one tag in range, this function will most likely fail due to the RF collision.
 * 	
 * @sa NurApiWriteSingulatedTag32(), NurApiWriteTagByEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	wrBank				Memory bank for write operation.
 * @param	wrAddress			<b>Word</b> address for write operation.
 * @param	wrByteCount			Number of bytes to write. This must <b>divisible by two</b>.
 * @param	wrBuffer			Data to write. Must be atleast <i>wrByteCount</i> bytes long.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiWriteTag32(HANDLE hApi, DWORD passwd, BOOL secured,
					 BYTE wrBank, DWORD wrAddress, int wrByteCount, BYTE *wrBuffer);

/** @fn int NurApiBlockWriteSingulatedTag32(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							  BYTE wrBank, DWORD wrAddress, int wrByteCount, BYTE *wrBuffer, BYTE blSize);

 * 	Write data to tag with specific singulation. Uses spcific block write.
 * 	Tag can be singluted against desired memory bank and mask.
 * 	
 * @sa NurApiBlockWriteByEPC(), NurApiBlockWriteTag32()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	wrBank				Memory bank for write operation.
 * @param	wrAddress			<b>Word</b> address for write operation.
 * @param	wrByteCount			Number of bytes to write. This must <b>divisible by two and by blSize (if not 0)</b>.
 * @param	wrBuffer			Data to write. Must be atleast <i>wrByteCount</i> bytes long.
 * @param	blSize				Size of a single block. If the data is considered to be single block. 
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiBlockWriteSingulatedTag32(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							  BYTE wrBank, DWORD wrAddress, int wrByteCount, BYTE *wrBuffer, BYTE blSize);

/** @fn int NurApiBlockWriteByEPC(HANDLE hApi, DWORD passwd, BOOL secured, BYTE *epcBuffer, DWORD epcBufferLen,
					 BYTE wrBank, DWORD wrAddress, int wrByteCount, BYTE *wrBuffer, BYTE blSize)

 * 	Write data to tag singulated by tag's EPC memory. Uses spcific block write.
 * 	
 * @sa NurApiBlockWriteSingulatedTag32(), NurApiBlockWriteTag32()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes. 
 * @param	wrBank				Memory bank for write operation.
 * @param	wrAddress			<b>Word</b> address for write operation.
 * @param	wrByteCount			Number of bytes to write. This must <b>divisible by two and by blSize (if not 0)</b>.
 * @param	wrBuffer			Data to write. Must be atleast <i>wrByteCount</i> bytes long.
 * @param	blSize				Size of a single block. If the data is considered to be single block. 
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */					  
NUR_API
int NURAPICONV NurApiBlockWriteByEPC(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE *epcBuffer, DWORD epcBufferLen,
							  BYTE wrBank, DWORD wrAddress, int wrByteCount, BYTE *wrBuffer, BYTE blSize);

/** @fn int NurApiBlockWriteTag32(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE wrBank, DWORD wrAddress, int wrByteCount, BYTE *wrBuffer, BYTE blSize)

 * Write data to tag without any singulation data. Uses spcific block write.
 * 	
 * @note If there's more than one tag in range, this function will most likely fail due to the RF collision.
 *
 * @sa NurApiBlockWriteByEPC(), NurApiBlockWriteSingulatedTag32()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	wrBank				Memory bank for write operation.
 * @param	wrAddress			<b>Word</b> address for write operation.
 * @param	wrByteCount			Number of bytes to write. This must <b>divisible by two and by blSize (if not 0)</b>.
 * @param	wrBuffer			Data to write. Must be atleast <i>wrByteCount</i> bytes long.
 * @param	blSize				Size of a single block. If the data is considered to be single block. 
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiBlockWriteTag32(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE wrBank, DWORD wrAddress, int wrByteCount, BYTE *wrBuffer, BYTE blSize);


/** @fn int NurApiEraseSingulatedTag32(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							  BYTE erBank, DWORD erAddress, BYTE erWordCount)

 * 	Erases tag using the BlockErase command with specific singulation. 
 * 	Tag can be singluted against desired memory bank and mask.
 * 	
 * @sa NurApiEraseTagByEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	erBank				Memory bank for erase operation.
 * @param	erAddress			<b>Word</b> address for erase operation.
 * @param	erWordCount			Number of words to erase.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiEraseSingulatedTag32(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							  BYTE erBank, DWORD erAddress, BYTE erWordCount);
							  
/** @fn int NURAPICONV NurApiEraseTagByEPC(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE *epcBuffer, DWORD epcBufferLen,
							  BYTE erBank, DWORD erAddress, BYTE erWordCount)

 * 	Erases tag using the BlockErase command.
 *
 * @sa NurApiEraseSingulatedTag32()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 * @param	erBank				Memory bank for erase operation.
 * @param	erAddress			<b>Word</b> address for erase operation.
 * @param	erWordCount			Number of words to erase.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiEraseTagByEPC(HANDLE hApi, DWORD passwd, BOOL secured,
							  BYTE *epcBuffer, DWORD epcBufferLen,
							  BYTE erBank, DWORD erAddress, BYTE erWordCount);

/** @fn int NurApiCustomWriteSingulatedTag32(HANDLE hApi, 
                DWORD wrCmd, BYTE cmdBits, DWORD wrBank, BYTE bankBits,
                DWORD passwd, BOOL secured, BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							  DWORD wrAddress, int wrByteCount, BYTE *wrBuffer)

 * 	Write data to tag with specific singulation using customizable command and bank parameters.
 * 	Tag can be singluted against desired memory bank and mask.
 * 	
 * @sa NurApiCustomWriteTagByEPC(), NurApiCustomWriteTag32()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	wrCmd				Custom write command.
 * @param	cmdBits			Number of bits in the 'custom' write command. Must be 1...32.
 * @param	wrBank			Customizable bank parameter for 'custom' write.
 * @param	bankBits		Number of bits in the bank parameter. Can be 0. 
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	wrAddress			<b>Word</b> address for write operation.
 * @param	wrByteCount			Number of bytes to write. This must <b>divisible by two</b>.
 * @param	wrBuffer			Data to write. Must be atleast <i>wrByteCount</i> bytes long.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiCustomWriteSingulatedTag32(HANDLE hApi, 
                DWORD wrCmd, BYTE cmdBits, DWORD wrBank, BYTE bankBits,
                DWORD passwd, BOOL secured, BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							  DWORD wrAddress, int wrByteCount, BYTE *wrBuffer);

/** @fn int NurApiCustomWriteTagByEPC(HANDLE hApi, 
                DWORD wrCmd, BYTE cmdBits, DWORD wrBank, BYTE bankBits,
                DWORD passwd, BOOL secured, BYTE *epcBuffer, DWORD epcBufferLen,
							  DWORD wrAddress, int wrByteCount, BYTE *wrBuffer)

 * 	Write data to tag singulated by tag's EPC memory.
 *
 * @sa NurApiCustomWriteSingulatedTag32(), NurApiCustomWriteTag32()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	wrCmd				Custom write command.
 * @param	cmdBits			Number of bits in the 'custom' write command. Must be 1...32.
 * @param	wrBank			Customizable bank parameter for 'custom' write.
 * @param	bankBits		Number of bits in the bank parameter. Can be 0. 
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 * @param	wrAddress			<b>Word</b> address for write operation.
 * @param	wrByteCount			Number of bytes to write. This must <b>divisible by two</b>.
 * @param	wrBuffer			Data to write. Must be atleast <i>wrByteCount</i> bytes long.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiCustomWriteTagByEPC(HANDLE hApi, 
                DWORD wrCmd, BYTE cmdBits, DWORD wrBank, BYTE bankBits,
                DWORD passwd, BOOL secured, BYTE *epcBuffer, DWORD epcBufferLen,
							  DWORD wrAddress, int wrByteCount, BYTE *wrBuffer);

/** @fn int NurApiCustomWriteTag32(HANDLE hApi, 
            DWORD wrCmd, BYTE cmdBits, DWORD wrBank, BYTE bankBits,
            DWORD passwd, BOOL secured,
					  DWORD wrAddress, int wrByteCount, BYTE *wrBuffer)

 * Write data to tag without any singulation data using customizable command and bank parameters.
 * This function can be used for tags that does not have EPC. 
 * 	
 * @note If there's more than one tag in range, this function will most likely fail due to the RF collision.
 * 	
 * @sa NurApiCustomWriteSingulatedTag32(), NurApiCustomWriteTagByEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	wrCmd				Custom write command.
 * @param	cmdBits			Number of bits in the 'custom' write command. Must be 1...32.
 * @param	wrBank			Customizable bank parameter for 'custom' write.
 * @param	bankBits		Number of bits in the bank parameter. Can be 0. 
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	wrAddress			<b>Word</b> address for write operation.
 * @param	wrByteCount			Number of bytes to write. This must <b>divisible by two</b>.
 * @param	wrBuffer			Data to write. Must be atleast <i>wrByteCount</i> bytes long.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiCustomWriteTag32(HANDLE hApi, 
            DWORD wrCmd, BYTE cmdBits, DWORD wrBank, BYTE bankBits,
            DWORD passwd, BOOL secured,
					  DWORD wrAddress, int wrByteCount, BYTE *wrBuffer);

/** @fn int NurApiCustomExchangeSingulated32(HANDLE hApi, DWORD passwd, BOOL secured, 
 *                BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask, 
 *                struct NUR_CUSTEXCHANGE_PARAMS *pParams, BYTE *resp, int *respLen);
 *
 * Performs a custom bit stream exchange or transmit only with a singulated tag. 
 * 	
 * @sa NurApiCustomExchangeByEPC(), NurApiCustomExchange32(), NUR_CUSTEXCHANGE_PARAMS
 *
 * @param	hApi				    Handle to valid NurApi object instance.
 * @param	passwd				  Password for secured operations.
 * @param	secured				  TRUE if operation is secured, otherwise FALSE.
 * @param	sBank				    Memory bank used for tag singulation.
 * @param	sAddress			  Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  Length of the mask data in <b>bits</b>.
 * @param	sMask   			  Mask data buffer.
 * @param pParams         Pointer to the custom bit stream controls.
 * @param resp            Pointer to the tag's response.
 * @param respLen         Pointer to the tag's response length.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiCustomExchangeSingulated32(HANDLE hApi, DWORD passwd, BOOL secured, 
                BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask, struct NUR_CUSTEXCHANGE_PARAMS *pParams, BYTE *resp, int *respLen);

/** @fn int NurApiCustomExchangeByEPC(HANDLE hApi, DWORD passwd, BOOL secured, 
 *            BYTE *epcBuffer, DWORD epcBufferLen, struct NUR_CUSTEXCHANGE_PARAMS *pParams, BYTE *resp, int *respLen);
 *
 * Performs a custom bit stream exchange or transmit only with a tag. Tag is singulated with the given EPC data.
 * 	
 * @sa NurApiCustomExchangeSingulated32(), NurApiCustomExchange32(), NUR_CUSTEXCHANGE_PARAMS
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	passwd			Password for secured operations.
 * @param	secured			TRUE if operation is secured, otherwise FALSE.
 * @param	epcBuffer		EPC data used for singulation.
 * @param	epcBufferLen	EPC length in bytes.
 * @param 	pParams       	Pointer to the custom bit stream controls.
 * @param 	resp          	Pointer to the tag's response.
 * @param 	respLen       	Pointer to the tag's response length.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */                
NUR_API
int NURAPICONV NurApiCustomExchangeByEPC(HANDLE hApi, DWORD passwd, BOOL secured, 
              BYTE *epcBuffer, DWORD epcBufferLen, struct NUR_CUSTEXCHANGE_PARAMS *pParams, BYTE *resp, int *respLen);

/** @fn int NurApiCustomExchange32(HANDLE hApi, DWORD passwd, BOOL secured, struct NUR_CUSTEXCHANGE_PARAMS *pParams, BYTE *resp, int *respLen);
 *
 * Performs a custom bit stream exchange or transmit only with a tag. 
 * This function can be used with a tag that does not ahve an EPC or in a case where
 * a tag can safely be assumed to be the only on in the range.
 * 	
 * @sa NurApiCustomExchangeSingulated32, NurApiCustomExchangeByEPC(), NUR_CUSTEXCHANGE_PARAMS
 *
 * @param	hApi		Handle to valid NurApi object instance.
 * @param	passwd		Password for secured operations.
 * @param	secured		TRUE if operation is secured, otherwise FALSE.
 * @param 	pParams     Pointer to the cuastom bit stream controls.
 * @param 	resp        Pointer to the tag's response.
 * @param 	respLen     Pointer to the tag's response length.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiCustomExchange32(HANDLE hApi, DWORD passwd, BOOL secured, struct NUR_CUSTEXCHANGE_PARAMS *pParams, BYTE *resp, int *respLen);

/** @fn BOOL NurApiAddDWBitsToBuffer(BYTE *buf, int *curPt, DWORD value, int nBits, int maxLen)
 *
 * Adds a DWORD value to a bit buffer.
 * 	
 * @sa NurApiAddEBV32
 * 
 * @param	buf     Pointer to a byte buffer allocated for bits.
 * @param	curPt   Current pointer i.e. where to add next bits.
 * @param value   Value to append.
 * @param nBits   Number of bits used from the value.
 * @param maxLen  Maximum number of bits in the buf parameter.
 *
 * @return	TRUE when succeeded, FALSE on error (no room, or parameter error). 
 * @remark the curPt parameter will be updated to a new value upon a successful value addition.
 */
NUR_API
BOOL NURAPICONV NurApiAddDWBitsToBuffer(BYTE *buf, int *curPt, DWORD value, int nBits, int maxLen);

/** @fn BOOL NurApiAddEBV32(BYTE *buf, int *curPt, DWORD ebv, int maxLen);
 *
 * Adds a 32-bit EBV (Extensible Bit Vector) value to a bit buffer.
 * 	
 * @sa NurApiAddDWBitsToBuffer
 * 
 * @param	buf     Pointer to a byte buffer allocated for bits.
 * @param	curPt   Current pointer i.e. where to add next bits.
 * @param	ebv     Value to append.
 * @param	maxLen  Maximum number of bits in the buf parameter.
 *
 * @return	TRUE when succeeded, FALSE on error (no room, or parameter error). 
 * @remark the curPt parameter will be updated to a new value upon a successful EBV addition.
 */
NUR_API
BOOL NURAPICONV NurApiAddEBV32(BYTE *buf, int *curPt, DWORD ebv, int maxLen);

/** @fn int NurApiWriteEPC(HANDLE hApi, DWORD passwd, BOOL secured,
				   BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
				   BYTE *newEpcBuffer, DWORD newEpcBufferLen)

 * 	Write new EPC to tag with specific singulation. 
 * 	This function sets also tag PC word correctly for new EPC length.
 * 	Tag can be singluted against desired memory bank and mask.
 * 	
 * @sa NurApiWriteEPCByEPC()
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	newEpcBuffer		New EPC to write.
 * @param	newEpcBufferLen		Number of EPC bytes to write. This must <b>divisible by two</b>.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiWriteEPC(HANDLE hApi, DWORD passwd, BOOL secured,
				   BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
				   BYTE *newEpcBuffer, DWORD newEpcBufferLen);

/** @fn int NurApiWriteEPCByEPC(HANDLE hApi, DWORD passwd, BOOL secured,
				   BYTE *epcBuffer, DWORD epcBufferLen,
				   BYTE *newEpcBuffer, DWORD newEpcBufferLen)

 *  Write new EPC to specific tag singulated by tag's EPC memory. 
 *  This function sets also tag PC bytes correctly for new EPC length.
 * 	
 * @sa NurApiWriteEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 * @param	newEpcBuffer		New EPC to write.
 * @param	newEpcBufferLen		Number of EPC bytes to write. This must <b>divisible by two</b>.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiWriteEPCByEPC(HANDLE hApi, DWORD passwd, BOOL secured,
				   BYTE *epcBuffer, DWORD epcBufferLen,
				   BYTE *newEpcBuffer, DWORD newEpcBufferLen);

/** @fn int NurApiSetAccessPassword(HANDLE hApi, DWORD passwd, BOOL secured, 
							BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							DWORD newPasswd)

 * 	Write new access password to tag with specific singulation. 
 * 	Tag can be singluted against desired memory bank and mask.
 * 	
 * @sa NurApiGetKillPassword(), NurApiGetKillPasswordByEPC(), NurApiSetKillPassword(), NurApiSetKillPasswordByEPC()
 * @sa NurApiGetAccessPassword(), NurApiGetAccessPasswordByEPC(), NurApiSetAccessPassword(), NurApiSetAccessPasswordByEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	newPasswd			New access password to write.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiSetAccessPassword(HANDLE hApi, DWORD passwd, BOOL secured, 
							BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							DWORD newPasswd);

/** @fn int NurApiSetAccessPasswordByEPC(HANDLE hApi, DWORD passwd, BOOL secured, 
						  BYTE *epcBuffer, DWORD epcBufferLen,
						  DWORD newPasswd)

 *	Write new access password to specific tag singulated by tag's EPC memory. 
 * 	
 * @sa NurApiGetKillPassword(), NurApiGetKillPasswordByEPC(), NurApiSetKillPassword(), NurApiSetKillPasswordByEPC()
 * @sa NurApiGetAccessPassword(), NurApiGetAccessPasswordByEPC(), NurApiSetAccessPassword(), NurApiSetAccessPasswordByEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 * @param	newPasswd			New access password to write.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiSetAccessPasswordByEPC(HANDLE hApi, DWORD passwd, BOOL secured, 
						  BYTE *epcBuffer, DWORD epcBufferLen,
						  DWORD newPasswd);

/** @fn int NurApiGetAccessPassword(HANDLE hApi, DWORD passwd, BOOL secured, 
							BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							DWORD *currentPasswd)

 * 	Read access password from tag with specific singulation. 
 * 	Tag can be singluted against desired memory bank and mask.
 * 	
 * @sa NurApiGetKillPassword(), NurApiGetKillPasswordByEPC(), NurApiSetKillPassword(), NurApiSetKillPasswordByEPC()
 * @sa NurApiGetAccessPassword(), NurApiGetAccessPasswordByEPC(), NurApiSetAccessPassword(), NurApiSetAccessPasswordByEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	currentPasswd		Read access password is stored here.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiGetAccessPassword(HANDLE hApi, DWORD passwd, BOOL secured, 
							BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
							DWORD *currentPasswd);

/** @fn int NurApiGetAccessPasswordByEPC(HANDLE hApi, DWORD passwd, BOOL secured, 
						  BYTE *epcBuffer, DWORD epcBufferLen,
						  DWORD *currentPasswd)

 *	Read access password from specific tag singulated by tag's EPC memory. 
 * 	
 * @sa NurApiGetKillPassword(), NurApiGetKillPasswordByEPC(), NurApiSetKillPassword(), NurApiSetKillPasswordByEPC()
 * @sa NurApiGetAccessPassword(), NurApiGetAccessPasswordByEPC(), NurApiSetAccessPassword(), NurApiSetAccessPasswordByEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 * @param	currentPasswd		Read access password is stored here.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiGetAccessPasswordByEPC(HANDLE hApi, DWORD passwd, BOOL secured, 
						  BYTE *epcBuffer, DWORD epcBufferLen,
						  DWORD *currentPasswd);

/** @fn int NurApiSetKillPassword(HANDLE hApi, DWORD passwd, BOOL secured, 
						  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
						  DWORD newPasswd)

 * 	Write new kill password to tag with specific singulation. 
 * 	Tag can be singluted against desired memory bank and mask.
 * 	
 * @sa NurApiGetKillPassword(), NurApiGetKillPasswordByEPC(), NurApiSetKillPassword(), NurApiSetKillPasswordByEPC()
 * @sa NurApiGetAccessPassword(), NurApiGetAccessPasswordByEPC(), NurApiSetAccessPassword(), NurApiSetAccessPasswordByEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	newPasswd			New kill password to write.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiSetKillPassword(HANDLE hApi, DWORD passwd, BOOL secured, 
						  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
						  DWORD newPasswd);

/** @fn int NurApiSetKillPasswordByEPC(HANDLE hApi, DWORD passwd, BOOL secured, 
						  BYTE *epcBuffer, DWORD epcBufferLen,
						  DWORD newPasswd)

 *	Write new kill password to specific tag singulated by tag's EPC memory. 
 * 	
 * @sa NurApiGetKillPassword(), NurApiGetKillPasswordByEPC(), NurApiSetKillPassword(), NurApiSetKillPasswordByEPC()
 * @sa NurApiGetAccessPassword(), NurApiGetAccessPasswordByEPC(), NurApiSetAccessPassword(), NurApiSetAccessPasswordByEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 * @param	newPasswd			New kill password to write.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiSetKillPasswordByEPC(HANDLE hApi, DWORD passwd, BOOL secured, 
						  BYTE *epcBuffer, DWORD epcBufferLen,
						  DWORD newPasswd);

/** @fn int NurApiGetKillPassword(HANDLE hApi, DWORD passwd, BOOL secured, 
						  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
						  DWORD *currentPasswd)

 * 	Read kill password from tag with specific singulation. 
 * 	Tag can be singluted against desired memory bank and mask.
 * 	
 * @sa NurApiGetKillPassword(), NurApiGetKillPasswordByEPC(), NurApiSetKillPassword(), NurApiSetKillPasswordByEPC()
 * @sa NurApiGetAccessPassword(), NurApiGetAccessPasswordByEPC(), NurApiSetAccessPassword(), NurApiSetAccessPasswordByEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	currentPasswd		Read kill password is stored here.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiGetKillPassword(HANDLE hApi, DWORD passwd, BOOL secured, 
						  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
						  DWORD *currentPasswd);

/** @fn int NurApiGetKillPasswordByEPC(HANDLE hApi, DWORD passwd, BOOL secured, 
						  BYTE *epcBuffer, DWORD epcBufferLen,
						  DWORD *currentPasswd)

 * Read kill password from specific tag singulated by tag's EPC memory. 
 * 	
 * @sa NurApiGetKillPassword(), NurApiGetKillPasswordByEPC(), NurApiSetKillPassword(), NurApiSetKillPasswordByEPC()
 * @sa NurApiGetAccessPassword(), NurApiGetAccessPasswordByEPC(), NurApiSetAccessPassword(), NurApiSetAccessPasswordByEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations.
 * @param	secured				TRUE if operation is secured, otherwise FALSE.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 * @param	currentPasswd		Read access password is stored here.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned. 
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiGetKillPasswordByEPC(HANDLE hApi, DWORD passwd, BOOL secured, 
						  BYTE *epcBuffer, DWORD epcBufferLen,
						  DWORD *currentPasswd);

/** @fn int NurApiSetLock(HANDLE hApi, DWORD passwd, 
					  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
					  DWORD memoryMask, DWORD action)

 * 	Set memory lock for tag with specific singulation. 
 * 	Tag can be singluted against desired memory bank and mask.
 * 	
 * @sa NurApiSetLockRaw(), NurApiSetLockByEPC(), NUR_LOCKACTION, NUR_LOCKMEM
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations. Password is always needed.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	memoryMask			Which memories to lock. All flags in enum NUR_LOCKMEM can be combined.
 * @param	action				Action to perform for memories. See enum NUR_LOCKACTION.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiSetLock(HANDLE hApi, DWORD passwd, 
					  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
					  DWORD memoryMask, DWORD action);

/** @fn int NurApiSetLockRaw(HANDLE hApi, DWORD passwd, 
					  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
					  DWORD lockMask, DWORD lockAction)

 * 	Set memory lock for tag with specific singulation. 
 * 	Tag can be singluted against desired memory bank and mask.
 *  Lock mask and action parameter are in raw format defined in UHF C1G2 standard section "6.3.2.11.3.5 Lock (mandatory)".
 * 	
 * @sa NurApiSetLock()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations. Password is always needed.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	lockMask			Standard defined lock payload bits 19-10, starting from bit 0.
 * @param	lockAction			Standard defined lock payload bits 9-0, starting from bit 0.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiSetLockRaw(HANDLE hApi, DWORD passwd, 
					  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask,
					  DWORD lockMask, DWORD lockAction);

/** @fn int NurApiSetLockByEPC(HANDLE hApi, DWORD passwd, 
					  BYTE *epcBuffer, DWORD epcBufferLen,
					  DWORD memoryMask, DWORD action)

 *	Set memory lock for specific tag singulated by tag's EPC memory. 
 * 	
 * @sa NurApiSetLock(), NUR_LOCKACTION, NUR_LOCKMEM
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations. Password is always needed.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 * @param	memoryMask			Which memories to lock. All flags in enum NUR_LOCKMEM can be combined.
 * @param	action				Action to perform for memories. See enum NUR_LOCKACTION
 *
 * @return	Zero when succeeded, On error non-zero error code is returned. 
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiSetLockByEPC(HANDLE hApi, DWORD passwd, 
					  BYTE *epcBuffer, DWORD epcBufferLen,
					  DWORD memoryMask, DWORD action);

/** @fn int NurApiOpenStateLock(HANDLE hApi, BYTE sBank, DWORD sAddress, DWORD sMaskBitLength, BYTE *sMask, DWORD lockMask, DWORD lockAction)
 *
 * 	Set memory lock for tag with specific singulation without using access password. 
 * 	Tag can be singluted against desired memory bank and mask.
 * 	
 * @sa NurApiSetLockRaw(), NurApiSetLockByEPC(), NUR_LOCKACTION, NUR_LOCKMEM
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	lockMask			Which memories to lock. All flags in enum NUR_LOCKMEM can be combined.
 * @param	lockAction		    Action to perform for memories. See enum NUR_LOCKACTION.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiOpenStateLock(HANDLE hApi, BYTE sBank, DWORD sAddress, DWORD sMaskBitLength, BYTE *sMask, DWORD lockMask, DWORD lockAction);

/** @fn int NurApiOpenStateLockRaw(HANDLE hApi, BYTE sBank, DWORD sAddress, DWORD sMaskBitLength, BYTE *sMask, DWORD lockMask, DWORD lockAction)
 *
 * 	Set memory lock for tag with specific singulation without using access password. 
 * 	Tag can be singluted against desired memory bank and mask.
 *  Lock mask and action parameter are in raw format defined in UHF C1G2 standard section "6.3.2.11.3.5 Lock (mandatory)".
 * 	
 * @sa NurApiSetLock()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * @param	lockMask			Standard defined lock payload bits 19-10, starting from bit 0.
 * @param	lockAction			Standard defined lock payload bits 9-0, starting from bit 0.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiOpenStateLockRaw(HANDLE hApi, BYTE sBank, DWORD sAddress, DWORD sMaskBitLength, BYTE *sMask, DWORD lockMask, DWORD lockAction);

/** @fn NurApiOpenStateLockByEPC(HANDLE hApi, BYTE *epcBuffer, DWORD epcBufferLen, DWORD memoryMask, DWORD action)
 *
 *	Set memory lock for specific tag singulated by tag's EPC memory without using access password. 
 * 	
 * @sa NurApiSetLock(), NUR_LOCKACTION, NUR_LOCKMEM
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 * @param	memoryMask			Which memories to lock. All flags in enum NUR_LOCKMEM can be combined.
 * @param	action				Action to perform for memories. See enum NUR_LOCKACTION
 *
 * @return	Zero when succeeded, On error non-zero error code is returned. 
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiOpenStateLockByEPC(HANDLE hApi, BYTE *epcBuffer, DWORD epcBufferLen, DWORD memoryMask, DWORD action);

/** @fn int NurApiKillTag(HANDLE hApi, DWORD passwd, 
				  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask)

 * 	Kill tag with specific singulation. 
 * 	Tag can be singluted against desired memory bank and mask.
 * 	
 * @sa NurApiKillTagByEPC()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations. Password is always needed.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiKillTag(HANDLE hApi, DWORD passwd, 
				  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask);

/** @fn int NurApiKillTagByEPC(HANDLE hApi, DWORD passwd, 
				  BYTE *epcBuffer, DWORD epcBufferLen)

 *	Kill specific tag singulated by tag's EPC memory. 
 * 	
 * @sa NurApiKillTag()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations. Password is always needed.
 * @param	epcBuffer			EPC memory to singulate against.
 * @param	epcBufferLen		Size of epcBuffer in bytes.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned. 
 * @remark This function uses 32-bit tag data addressing.
 */
NUR_API
int NURAPICONV NurApiKillTagByEPC(HANDLE hApi, DWORD passwd, 
				  BYTE *epcBuffer, DWORD epcBufferLen);

/** @fn int NurApiNXPReadProtect(HANDLE hApi, DWORD passwd, BOOL protect,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask);

 *	Set or reset specific tag's read protect with given singulation data.
 * 	
 * @sa NurApiNXPReadProtect()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations. Password is always needed.
 * @param	protect				TRUE to set (password not needed) FALSE to reset (password always used).
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * 							
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag singulation.
 */
NUR_API
int NURAPICONV NurApiNXPReadProtect(HANDLE hApi, DWORD passwd, BOOL protect,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask);

/** @fn int NurApiNXPEAS(HANDLE hApi, DWORD passwd, BOOL set,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask);
 *
 * Set or reset specific tag's EAS alarm with given singulation data.
 * When tag's EAS alarm bit is set to 1, tag will reply to EAS Alarm command.
 * 	
 * @sa NurApiNXPAlarm(), NurApiNXPStartAlarmStream(), NurApiNXPStopAlarmStream()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations. Password is always needed.
 * @param	set					TRUE to set to alarm bit to 1, FALSE to reset alarm bit to 0: Password always used.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * 							
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag singulation.
 */
NUR_API
int NURAPICONV NurApiNXPEAS(HANDLE hApi, DWORD passwd, BOOL set,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask);

/** @fn int NurApiNXPAlarm(HANDLE hApi);
 *
 *	Send NXP EAS Alarm command. Return zero when there's armed EAS tags in range of reader.
 * 	
 * @sa NurApiNXPStartAlarmStream(), NurApiNXPStopAlarmStream(), NurApiNXPEAS()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * 							
 * @return	Zero when there's armed EAS tags in range of reader, NUR_ERROR_NO_TAG when no armed tags in range.
 */
NUR_API
int NURAPICONV NurApiNXPAlarm(HANDLE hApi);

/** @fn int NurApiNXPStartAlarmStream(HANDLE hApi)
 *
 * Start NXP EAS Alarm streaming on module. 
 * Module will start continuos EAS Alarm stream and sends notification to host when there's armed tags in range.
 *
 * @note Not supported in firmware version 3.1-A or older.
 *
 * @sa NurApiNXPStopAlarmStream(), NUR_EASALARMSTREAM_DATA
 *
 * @param	hApi				Handle to valid NurApi object instance.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiNXPStartAlarmStream(HANDLE hApi);

/** @fn int NurApiNXPStopAlarmStream(HANDLE hApi)
 *
 * Stop EAS Alarm streaming on module. 
 *
 * @note Not supported in firmware version 3.1-A or older.
 *
 * @param	hApi	Handle to valid NurApi object instance
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiNXPStopAlarmStream(HANDLE hApi);

/** @fn BOOL NurApiIsNXPAlarmStreamRunning(HANDLE hApi)
 *
 * Return TRUE if EAS Alarm stream routine is running on module.
 *
 * @note Not supported in firmware version 3.1-A or older.
 *
 * @param	hApi	Handle to valid NurApi object instance
 *
 * @return	TRUE if inventory stream routine is running. FALSE otherwise.
 */
NUR_API
BOOL NURAPICONV NurApiIsNXPAlarmStreamRunning(HANDLE hApi);

/** @fn int NurApiMonza4QTWrite(HANDLE hApi, DWORD passwd, BOOL reduce, BOOL pubMem,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask);

 *	Monza4 QT command. Writes the QT_SR and QT_MEM bits.
 * 	
 * @sa NurApiMonza4QTWrite()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations. Password is always needed.
 * @param	reduce				The QT_SR bit (0/1) (range reduction).
 * @param	pubMem				The QT_MEM bit (0/1) (public private memory selection). 
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * 							
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag singulation.
 */
NUR_API
int NURAPICONV NurApiMonza4QTWrite(HANDLE hApi, DWORD passwd, BOOL reduce, BOOL pubMem,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask);

/** @fn int NurApiMonza4QTRead(HANDLE hApi, DWORD passwd, BOOL *reduce, BOOL *pubMem,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask);

 *	Monza4 QT command. Reads the QT_SR and QT_MEM bits.
 * 	
 * @sa NurApiMonza4QTWrite()
 *
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	passwd				Password for secured operations. Password is always needed.
 * @param	*reduce				Pointer to BOOL indicating whether the read range is reduced or not.
 * @param	*pubMem				Pointer to BOOL indicating whether the tag uses public memory or not.
 * @param	sBank				Memory bank used for tag singulation.
 * @param	sAddress			Singulation data address in <b>bits</b>.
 * @param	sMaskBitLength  	Length of the mask data in <b>bits</b>.
 * @param	sMask   			Mask data buffer.
 * 							
 * @return	Zero when succeeded, On error non-zero error code is returned.
 * @remark This function uses 32-bit tag singulation.
 */
NUR_API
int NURAPICONV NurApiMonza4QTRead(HANDLE hApi, DWORD passwd, BOOL *reduce, BOOL *pubMem,
							  BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask);

/** @fn int NurApiFactoryReset(HANDLE hApi, DWORD resetcode)
 * Reset factory defaults.
 *
 * @param	hApi			Handle to valid NurApi object instance
 * @param	resetcode		Set this value to 0x994F6B32.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiFactoryReset(HANDLE hApi, DWORD resetcode);

/** @fn int NurApiModuleRestart(HANDLE hApi)
 *
 * Restart NUR Module.
 *
 * @param	hApi			Handle to valid NurApi object instance
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiModuleRestart(HANDLE hApi);

/** @fn int NurApiResetToTarget(HANDLE hApi, BYTE session, BOOL targetIsA)
 *
 * Reset tags inventoried state to desired target A or B.
 *
 * @param	hApi			Handle to valid NurApi object instance
 * @param	session			Which session to reset. Value 0 - 3
 * @param	targetIsA		TRUE if desired target is A, FALSE for target B
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiResetToTarget(HANDLE hApi, BYTE session, BOOL targetIsA);

/** @fn int NurApiGetEthConfig(HANDLE hApi,struct NUR_ETHDEV_CONFIG *resp,DWORD cfgSize)
 *
 * Get Ethernet configuration settings from conneted device. 
 * Getting ethernet setting from all Sampo devices in network same time, use NurApiSendBroadcast().
   NUR_NOTIFICATION_DEVSEARCH nofication appears every time Sampo device founded from network including configuration info.
 * 
 * @sa NUR_ETHDEV_CONFIG, NurApiSetEthConfig()
 * @param	hApi			Handle to valid NurApi object instance
 * @param	*resp			Pointer to NUR_ETHDEV_CONFIG struct for receiving configuration data
 * @param	cfgSize			Size of NUR_ETHDEV_CONFIG struct
 * @return	Zero when succeeded, On error non-zero error code is returned.
 
 */
NUR_API
int NURAPICONV NurApiGetEthConfig(HANDLE hApi,struct NUR_ETHDEV_CONFIG *resp,DWORD cfgSize);

/** @fn int NurApiSetEthConfig(HANDLE hApi, struct NUR_ETHDEV_CONFIG *vi, DWORD viSize)
 *
 
 * Set Ethernet configuration to currently connected device (USB, serial or TCP/IP)
   1. Set transport to 0 at NUR_ETHDEV_CONFIG struct

 * Set ethernet setting to specified Sampo using Broadcast:
	1. Set transport to 1 at NUR_ETHDEV_CONFIG struct
	2. Specify mac address of target Sampo at NUR_ETHDEV_CONFIG struct
 * @sa NUR_ETHDEV_CONFIG, NurApiGetEthConfig()
 * @param	hApi			Handle to valid NurApi object instance
 * @param	*vi				Pointer to NUR_ETHDEV_CONFIG struct for configuration data
 * @param	viSize			Size of NUR_ETHDEV_CONFIG struct
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSetEthConfig(HANDLE hApi, struct NUR_ETHDEV_CONFIG *vi, DWORD viSize);

/** @fn int NurApiSendBroadcast(HANDLE hApi,int cmd,int filterType,int filterOp, BYTE *filterData, int filterSize, BYTE *data, DWORD dataLength)
 
 * Broadcast messages are used mainly for searching Sampo devices from network.
 * Broadcast message can also be used for Get/Set ethernet configurations of Sampo devices
 * Send Broadcast message using specified command and optional filters.
 * Broadcast message is sent to all Sampo devices in network but only those Sampo devices receives and executes messages which met filter rules.
 * If filter type has been set to NUR_BC_FILTER_TYPE_NONE then all devices recevies and executes broadcast message.
  
 * @sa NUR_BC_CMD, NUR_BC_FILTER_TYPE, NUR_BC_FILTER_OP
 * @param	hApi			Handle to valid NurApi object instance
 * @param	cmd				Broadcast command. see NUR_BC_CMD)
 * @param	filterType		Filter type: see NUR_BC_FILTER_TYPE
 * @param	filterOp		Filter operation: see NUR_BC_FILTER_OP
 * @param	filterData		Filter data. Max 16 bytes. Typically char string.
 * @param	filterSize		Size of filter data in bytes.
 * @param	data			Broadcast command data. cmd specified what kind of data need to send to Sampo device(s)
 * @param	dataLength		Size of Broadcast command data
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSendBroadcast(HANDLE hApi,int cmd,int filterType,int filterOp, BYTE *filterData, int filterSize, BYTE *data, DWORD dataLength);

/** @fn int NurApiDiscoverDevices(HANDLE hApi, DWORD timeout)
 *
 * Runs a one device discovery round. It is possible that during the call information 
 * about the found devices is passed via device search -notification if there is an API callback correctly set up.
 * 
 * @sa MIN_DEVQUERY_TIMEOUT, MAX_DEVQUERY_TIMEOUT, NUR_NOTIFICATION_DEVSEARCH, NurApiSetNotificationCallback
 * @param	hApi			Handle to valid NurApi object instance
 * @param	timeout			This is the response wait timeout. Unit is milliseconds and the range is MIN_DEVQUERY_TIMEOUT...MAX_DEVQUERY_TIMEOUT.
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiDiscoverDevices(HANDLE hApi, DWORD timeout);

/** @fn int NurApiStartDeviceDiscovery(HANDLE hApi, DWORD timeout)
 
 * Starts a background thread that constantly queries Sampo devices from the local network.
  
 * @sa NUR_BC_FILTER_OP, MIN_DEVQUERY_TIMEOUT, MAX_DEVQUERY_TIMEOUT
 * @param	hApi			Handle to valid NurApi object instance
 * @param	timeout			This is the response wait timeout. Unit is milliseconds and the range is MIN_DEVQUERY_TIMEOUT...MAX_DEVQUERY_TIMEOUT.
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStartDeviceDiscovery(HANDLE hApi, DWORD timeout);

/** @fn int NurApiStopDeviceDiscovery(HANDLE hApi)
 
 * Interrupts the current Sampo device query if it is running.
  
 * @sa NurApiStartDeviceDiscovery, NurApiSendBroadcast
 *
 * @param	hApi Handle to valid NurApi object instance
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStopDeviceDiscovery(HANDLE hApi);

/** @fn int NurApiIsDiscovering(HANDLE hApi)
 
 * Used to check whether there is an ongoing device discovery.
  
 * @sa NurApiStartDeviceDiscovery, NurApiStopDeviceDiscovery, NurApiSendBroadcast
 *
 * @param	hApi Handle to valid NurApi object instance
 *
 * @return	Zero when succeeded (discovery running), non-zero error code otherwise.
 */
NUR_API
int NURAPICONV NurApiIsDiscovering(HANDLE hApi);

/** @fn int NurApiGetSystemInfo(HANDLE hApi, struct NUR_SYSTEMINFO *si, DWORD siSize)
 * Return information about the module itnernals.
 *
 * @sa NUR_SYSTEMINFO
 *
 * @param hApi    Handle to valid NurApi object instance
 * @param si      Pointer to the NUR_SYSTEMINFO structure.
 * @param siSize  Minimum size of 'si'.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */

NUR_API
int NURAPICONV NurApiGetSystemInfo(HANDLE hApi, struct NUR_SYSTEMINFO *si, DWORD siSize);

/** @fn int NurApiCustomCmd(HANDLE hApi, BYTE cmd, BYTE *inbuffer, DWORD inbufferLen, BYTE *outbuffer, DWORD outbufferLen, DWORD *bytesRet)
 * executes a freely formatted custom exchange with a tag.
 *
 * @param hApi            Handle to valid NurApi object instance
 * @param cmd             Command that the module's comman handler processes.
 * @param inbuffer        Parameters for this command.
 * @param inbufferLen     Length, in bytes, of the parameters.
 * @param outbuffer       Pointer to the response buffer.
 * @param outbufferLen    Maximum output buffer length.
 * @param bytesRet        Pointer to an unsigned integer where the response's actual length is stored to.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiCustomCmd(HANDLE hApi, BYTE cmd, BYTE *inbuffer, DWORD inbufferLen, BYTE *outbuffer, DWORD outbufferLen, DWORD *bytesRet);

/** @fn int NurApiTuneAntenna(HANDLE hApi, int antenna, BOOL wideTune, BOOL bSaveResults, int *dBmResults)
 * Executes an antenna tune.
 *
 * @param hApi            	Handle to valid NurApi object instance
 * @param antenna       	Antenna to use. Value is 0...max antennas.
 * @param wideTune			If set to TRUE the tuning is done in wider range opposing to FALSE which 'fast tune'
 * @param bSaveResults		If set to TRUE then the tuning results will be stored into the module's non-volatile memory.
 * @param dBmResults		Pointer to 6 integer values. The reflected power will be stored into these values in format dBm * 1000.
 *							This parameter may be NULL.
 *
 * @return	Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiTuneAntenna(HANDLE hApi, int antenna, BOOL wideTune, BOOL bSaveResults, int *dBmResults);

/** @fn int NurApiRestoreTuning(HANDLE hApi, BOOL factoryReset)
 * Executes an antenna tune.
 *
 * @param hApi            	Handle to valid NurApi object instance
 * @param factoryReset      If set to TRUE the tuning is restored from the factory defaults. 
 *							Otherwise it is from the last saved user tuning.
 *
 * @return	Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiRestoreTuning(HANDLE hApi, BOOL factoryReset);

/** @fn int NurApiGetDeviceCaps(HANDLE hApi, struct NUR_DEVICECAPS *pCaps)
 * Gets currently connected dewvice's capabilities.
 *
 * @sa NUR_DEVICECAPS
 *
 * @param hApi            	Handle to valid NurApi object instance
 * @param pCaps       		Pointer to NUR_DEVICECAPS structure.
 *
 * @return	Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetDeviceCaps(HANDLE hApi, struct NUR_DEVICECAPS *pCaps);

/** @fn int NurApiGetVersions(HANDLE hApi, BYTE *pMode, TCHAR *pPrimary, TCHAR *pSecondary)
 * Gets current mode and the versions store to the module.
 *
 * @param hApi            	Handle to valid NurApi object instance
 * @param pMode       		Current mode will be stored here ('A'=app, 'B'=loader) if valid pointer.
 * @param pPrimary			Pointer to the primary version string (in the loader the loader version and in the application the application version). Must be at least 16 characters long.
 * @param pSecondary		Pointer to the secondary version string (in the loader the application version and in the application the loader version). Must be at least 16 characters long.
 *
 * @return	Zero when succeeded, on error non-zero error code is returned. One of the pointer parameters must be non-null.
 */
NUR_API
int NURAPICONV NurApiGetVersions (HANDLE hApi, BYTE *pMode, TCHAR *pPrimary, TCHAR *pSecondary);

/** @fn int NurApiGetReflectedPowerValue(HANDLE hApi, DWORD freq, int *pdBm)
 * Returns the reflected power value for given frequency or current region middle frequency.
 *
 * @note The value stored via the integer pointer should be casted to double and then divided by 1000.0 in order to get the actual value.
 *
 * @param hApi            	Handle to valid NurApi object instance
 * @param freq				Frequency to use for the measurement in kHz. Use 0 to measure current region middle frequency.
 * @param pdBm				Pointer to the integer value of reflectedpower. Unit is dBm and the value is multiplied by 1000.
 *
 * @return	Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetReflectedPowerValue(HANDLE hApi, DWORD freq, int *pdBm);

/** @fn int NurApiStartEPCEnumeration(HANDLE hApi, struct NUR_EPCENUMINFO *pInfo)
 * Starts the TID content based EPC enumeration process (continuous function).
 *
 * @sa NUR_EPCENUMINFO
 *
 * @param hApi      Handle to valid NurApi object instance
 * @param pInfo		Pointer to the NUR_EPCENUMINFO that control how the enumeration is done.
 *
 * @return	Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStartEPCEnumeration(HANDLE hApi, struct NUR_EPCENUMINFO *pInfo);

/** @fn int NurApiStopEPCEnumeration(HANDLE hApi)
 * Stops the currently ongoing TID based EPC enumeration process.
 *
 * @sa NUR_EPCENUMINFO
 *
 * @param hApi      Handle to valid NurApi object instance
 *
 * @return	Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStopEPCEnumeration(HANDLE hApi);

/** @fn int NurApiIsEPCEnumRunning(HANDLE hApi)
 * Returns whether the EPC enumeration is currently running or not.
 *
 * @sa NurApiStartEPCEnumeration, NurApiStopEPCEnumeration
 *
 * @param hApi      Handle to valid NurApi object instance
 *
 * @return	TRUE if API handle is valid and stream running, FALSE otherwise.
 */
NUR_API
BOOL NURAPICONV NurApiIsEPCEnumRunning(HANDLE hApi);

/** @fn int NurApiGetFWINFO(HANDLE hApi, TCHAR *buf, DWORD buflen)
 * Get current FW information string.
 *
 * @param hApi      Handle to valid NurApi object instance
 * @param buf		Pointer to string buffer
 * @param buflen    Length of 'buf' in characters
 *
 * @return	Zero when succeeded, on error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGetFWINFO(HANDLE hApi, TCHAR *buf, DWORD buflen);

/** @fn int NurApiSetExtCarrier(HANDLE hApi, BOOL on)
 * 
 * Causes the module to leave carrier on after a command and not to jump to new frequency.
 * The carrier on time is limited by the maximum channel time.
 *
 * @param hApi      Handle to valid NurApi object instance
 * @param on		If TRUE, then the carrier is left on after next command. When set to FALSE the carrier is shut down immediately.
 *
 * @return			Zero when successful. A non-zero error code otherwise.
*/
NUR_API
int NURAPICONV NurApiSetExtCarrier(HANDLE hApi, BOOL on);

/** @fn int NurApiSetConstantChannelIndex(HANDLE hApi, int channelIdx)
 * 
 * Set channel index to use in current hop table.
 * This prevents NUR from hopping in different channel.
 * Use with care: RF frequency is locked as long as this is set.
 * Restore normal channel hopping with parameter -1
 *
 * @param hApi			Handle to valid NurApi object instance
 * @param channelIdx	Channel index to use in current hop table. Set -1 to restore normal channel hopping.
 *
 * @return			Zero when successful. A non-zero error code otherwise.
*/
NUR_API
int NURAPICONV NurApiSetConstantChannelIndex(HANDLE hApi, int channelIdx);

/** @fn int NurApiGetAntennaMap(HANDLE hApi, struct NUR_ANTENNA_MAPPING *antennaMap, int *nrMappings, int maxnMappings, DWORD szMapping)
 * 
 * This function retrieves the antenna mapping from the device i.e. the physical names of the antennas.
 *
 * @param hApi			Handle to valid NurApi object instance
 * @param antennaMap	Pointer to a struct NUR_ANTENNA_MAPPING array where the results are stored into.
 * @param nrMappings	The number physical antenna mappings retrieved is stored here.
 * @param maxnMappings	Number of mapping to retrieve, the range is 1...NUR_MAX_ANTENNAS_EX.
 * @param szMapping		Size of single antenna mapping, use sizeof(struct NUR_ANTENNA_MAPPING).
 *
 * @return				Zero when successful. A non-zero error code otherwise.
 *
 * @remark				The parameters @sa antennaMap, @sa nrMappings and @sa szMapping can be seto to zero in which case the API refreshes its internal antenna mapping.
 *
 * @sa MAX_MAPPINGLEN, NUR_MAX_ANTENNAS_EX, struct NUR_ANTENNA_MAPPING
*/
NUR_API
int NURAPICONV NurApiGetAntennaMap(HANDLE hApi, struct NUR_ANTENNA_MAPPING *antennaMap, int *nrMappings, int maxnMappings, DWORD szMapping);

/** @fn int NurApiEnablePhysicalAntenna(HANDLE hApi, const TCHAR *commaSeparated, BOOL disableOthers)
 * 
 * This function enables the physical antennas that are specified as comma separated string parameter.
 *
 * @param hApi			Handle to valid NurApi object instance
 * @param commaSeparated	Comma-separated physical antenna specifier.
 * @param disableOthers	If set to TRUE, antennas that are not specified in comma separated string will be disabled. If set to FALSE, antennas that are already enabled will remain enabled.
 *
 * @return				Zero when successful. A non-zero error code otherwise.
 *
 * @remark				The specifier string can be e.g. "Beam1,Beam2.X": specifies that Beam1 both polarities and Beam2 X-polarity are enabled.
 * @remark				If only "ALL" is passed to this function it will then enable all antennas.
 *
 * @sa NurApiDisableAntennasEx
 *
*/
NUR_API
int NURAPICONV NurApiEnablePhysicalAntenna(HANDLE hApi, const TCHAR *commaSeparated, BOOL disableOthers);

/** @fn int NurApiDisablePhysicalAntenna(HANDLE hApi, const TCHAR *commaSeparated)
 * 
 * This function disables the physical antennas that are specified as comma separated string parameter.
 *
 * @param hApi				Handle to valid NurApi object instance
 * @param commaSeparated	Comma-separated physical antenna specifier.
 *
 * @return				Zero when successful. A non-zero error code otherwise.
 *
 * @remark				The specifier string can be e.g. "Beam1,Beam2,Beam3": specifies that Beam1...3 will be disabled.
 * @sa NurApiEnableAntennasEx
 *
*/
NUR_API
int NURAPICONV NurApiDisablePhysicalAntenna(HANDLE hApi, const TCHAR *commaSeparated);

/** @fn int NurApiAntennaIdToPhysicalAntenna(HANDLE hApi, int antennaId, TCHAR *name, int maxNameLen)
 *
 *	This function maps given logical antenna identifier to a physical name of an antenna.
 * 
 * @param hApi			Handle to valid NurApi object instance
 * @param antennaId		Logical antenna number (identifier) to be looked up.
 * @param name			The name of the antenna (if ound) is stored here. Allocate a minimum of MAX_MAPPINGLEN + 1 characters.
 * @param maxNameLen	Maximum number of characters in "name". Minimum required is MAX_MAPPINGLEN.
 *
 * @return				Zero when successful. A non-zero error code otherwise.
 *
 * @remark				When the function return NUR_ERROR_GENERAL it means that the antenna mapping is present, but ID was not found.
 *
 * @sa MAX_MAPPINGLEN, NUR_MAX_ANTENNAS_EX
*/
NUR_API
int NURAPICONV NurApiAntennaIdToPhysicalAntenna(HANDLE hApi, int antennaId, TCHAR *name, int maxNameLen);

/** @fn int NurApiPhysicalAntennaToAntennaId(HANDLE hApi, const TCHAR *name, int *antennaId)
 *
 * This function maps given physical antenna name to an logical antenna identifier.
 * 
 * @param hApi			Handle to valid NurApi object instance
 * @param name			The name of the antenna (physical) to look up.
 * @param antennaId		Logical antenna number will be stored here.
 *
 * @return				Zero when successful. A non-zero error code otherwise.
 *
 * @remark				When the function return NUR_ERROR_GENERAL it means that the antenna mapping is present, but name was not found.
*/
NUR_API
int NURAPICONV NurApiPhysicalAntennaToAntennaId(HANDLE hApi, const TCHAR *name, int *antennaId);

/** @fn int NurApiGetPhysicalAntennaMask(HANDLE hApi, const TCHAR *commaSeparated, DWORD *antMask)
 *
 * This function generates the antennaMask separated by a comma in the parameter commaSeparated and returns NUR_NO_ERROR when generating the mask succeeds
 * 
 * @param hApi				Handle to valid NurApi object instance
 * @param commaSeparated	Comma-separated physical antenna specifier.
 * @param antMask			Pointer to the antennaMask
 *
 * @return			Zero when successfully retrieves the antennaMask for the antennas listed in commaSeparated
 *
 * @remark			The function return NUR_NO_ERROR if all the queried antennas are enabled.
 * @remark			If the connection is OK, the antenna mapping is present and atleast one valid antenna is listed in the commaSeparated parameter
 *
 * @sa NUR_NO_ERROR, NUR_ERROR_GENERAL, NUR_ERROR_NOT_READY, NUR_ERROR_TR_NOT_CONNECTED, NUR_ERROR_INVALID_HANDLE
*/
NUR_API
int NURAPICONV NurApiGetPhysicalAntennaMask(HANDLE hApi, const TCHAR *commaSeparated, DWORD *antMask);

/** @fn int NurApiIsPhysicalAntennaEnabled(HANDLE hApi, const TCHAR *commaSeparated)
 *
 * This function return NUR_NO_ERROR if all physical antennas specified in comma separated string are enabled, otherwise error is returned.
 * 
 * @param hApi				Handle to valid NurApi object instance
 * @param commaSeparated	Comma-separated physical antenna specifier.
 *
 * @return			Zero when successful (and all are enabled). A non-zero error code otherwise.
 *
 * @remark			The function return NUR_NO_ERROR if all the queried antennas are enabled.
 * @remark			If the connection is OK, the antenna mapping is present and one or more of the antennass are not enabled then NUR_ERROR_GENERAL is returned.
 *
 * @sa NUR_SUCCESS, NUR_NO_ERROR, NUR_ERROR_GENERAL
*/
NUR_API
int NURAPICONV NurApiIsPhysicalAntennaEnabled(HANDLE hApi, const TCHAR *commaSeparated);

/** @fn int NurApiWriteScratchArea(HANDLE hApi, int page, int offset, BYTE *scratchData, int szScratchData)
 *
 * Write scratch area data into the module. In the module's non-volatile memory there is 2 x 256 bytes reserved for the user defined data.
 * The user scratch data are behaves like EEPROM i.e. it can be written and read per byte basis.
 * 
 * @param hApi		Handle to valid NurApi object instance
 * @param page		Into what page the data is written to.
 * @param offset	The data offset within the page, byte alignment.
 * @param scratchData	Pointer to the byte data to be written.
 * @param szScratchData	Size of the data to be written.
 *
 * @return			Zero when successful. A non-zero error code otherwise.
 *
 * @remark			The data size needs to fit into the 256-byte are of the given page.
 *
 * @sa NUR_SUCCESS, NUR_ERROR_INVALID_PARAMETER, MIN_SCRATCHPAGE, MAX_SCRATCHPAGE, MIN_SCRATCHBYTES, MAX_SCRATCHBYTES
*/
NUR_API
int NURAPICONV NurApiWriteScratchArea(HANDLE hApi, int page, int offset, BYTE *scratchData, int szScratchData);

/** @fn int NurApiReadScratchArea(HANDLE hApi, int page, int offset, BYTE *scratchData, int szScratchData)
 *
 * Read scratch area data from the module. In the module's non-volatile memory there is 2 x 256 bytes reserved for the user defined data.
 * The user scratch data are behaves like EEPROM i.e. it can be written and read per byte basis.
 * 
 * @param hApi		Handle to valid NurApi object instance
 * @param page		The page to read from.
 * @param offset	The data offset within the page, byte alignment.
 * @param scratchData	Pointer to where the read data is stored into.
 * @param szScratchData	Number of bytes to read.
 *
 * @return			Zero when successful. A non-zero error code otherwise.
 *
 * @remark			The data size needs to fit into the 256-byte are of the given page.
 *
 * @sa NUR_SUCCESS, NUR_ERROR_INVALID_PARAMETER, MIN_SCRATCHPAGE, MAX_SCRATCHPAGE, MIN_SCRATCHBYTES, MAX_SCRATCHBYTES
*/
NUR_API
int NURAPICONV NurApiReadScratchArea(HANDLE hApi, int page, int offset, BYTE *scratchData, int szScratchData);

/** @fn int NURAPICONV NurApiWLanEnable(HANDLE hApi, BOOL enable)
 *
 * Enable/Disable WLan module.
 * 
 * @param hApi		Valid NUR API handle.
 * @param enable	TRUE = WLan module up running, FALSE = Wlan module not in use. 
 *
 * @return			Zero when successful. A non-zero error code otherwise.
 *
 * @remark			When WLan disabled, module is in hibernate state.
 *
*/
NUR_API
int NURAPICONV NurApiWLanEnable(HANDLE hApi, BOOL enable);

/** @fn int NURAPICONV NurApiWLanScanEnable(HANDLE hApi, BOOL enable)
 *
 * Enable/Disable Network scan functionality. When enabled, network scan will be performed between 30 sec.
 * Use NurApiWLanGetNetworkList for getting list of network nodes found. (SSID, Security type, RSSI)
 * 
 * @param hApi		Valid NUR API handle.
 * @param enable	TRUE = enable network scan, FALSE to disable.
 *
 * @return			Zero when successful. A non-zero error code otherwise.
 *
 * @remark			By default, scan is disabled.
 * 
*/
NUR_API
int NURAPICONV NurApiWLanScanEnable(HANDLE hApi, BOOL enable);

/** @fn int NurApiWLanGetStatus(HANDLE hApi, struct NUR_WLANSTATUS *status, DWORD statusSize)
 *
 * Get current status from WLAN module.
 *
 * @sa NUR_WLANSTATUS
 * 	   
 * @param	hApi		Handle to valid NurApi object instance. 
 * @param	status		Pointer to a buffer that receives WLan module current status.
 * @param	statusSize	Size of the <i>status</i> structure in bytes. Usually: sizeof(struct NUR_WLANSTATUS).
 * 
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiWLanGetStatus(HANDLE hApi, struct NUR_WLANSTATUS *status, DWORD statusSize);

/** @fn int NURAPICONV NurApiWLanGetProfile(HANDLE hApi, struct NUR_WLANPROFILE *profile, BYTE profileIndex)
 *
 * Get connection profile from WLAN module.
 * If profile not found based on profileIndex, function return NUR_WLANPROFILE struct where index member = 0.
 *
 * @sa NUR_WLANPROFILE,NurApiWLanDelProfile, NurApiWLanAddProfile
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	profile			Pointer to a buffer that receives WLan module current status.
 * @param	profileIndex	Index of profile to get (1-7). If profile not found, NUR_WLANPROFILE-->index = 0.
 * 
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiWLanGetProfile(HANDLE hApi, struct NUR_WLANPROFILE *profile, BYTE profileIndex);

/** @fn int NURAPICONV NurApiWLanAddEnterpriseProfile(HANDLE hApi, TCHAR *ssid,TCHAR *seckey, TCHAR *user, TCHAR *anonuser, BYTE eapmethod, BYTE priority)
 *
 * Add enterprise connection profile to WLAN module.
 * the device connects to a station from the profiles table. Only one enterprise profile is supported.
 *
 * @sa NurApiWLanGetProfile, NurApiWLanDelProfile
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	ssid			Pointer to a string buffer containing SSID name.
 * @param	seckey			Pointer buffer containing security key string. (hexadecimal digits 0-9, a-f and A-F)
 * @param	user			The enterprise user name
 * @param	anonuser		The anonymous user name (optional) for two phase enterprise connections.
 * @param	eapmethod		EAP Method
 * @param	priority		0-7 (0=lowest) If several profiles configured the device chose the highest priority profile, within each priority group, device will chose profile based on security policy, signal strength, etc parameters
 * 
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiWLanAddEnterpriseProfile(HANDLE hApi, TCHAR *ssid,TCHAR *seckey, TCHAR *user, TCHAR *anonuser, BYTE eapmethod, BYTE priority);

/** @fn int NURAPICONV NurApiWLanAddProfile(HANDLE hApi, TCHAR *ssid, BYTE secType, TCHAR *seckey, BYTE priority)
 *
 * Add connection profile to WLAN module.
 * the device connects to a station from the profiles table. Up to 7 profiles are supported.
 *
 * @sa NurApiWLanGetProfile, NurApiWLanDelProfile
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	ssid			Pointer to a string buffer containing SSID name.
 * @param	secType			OPEN=0, WEP=1, WPA=2, WPA2=3, WPA_ENT=6
 * @param	seckey			Pointer buffer containing security key string. (hexadecimal digits 0-9, a-f and A-F)
 * @param	priority		0-7 (0=lowest) If several profiles configured the device chose the highest priority profile, within each priority group, device will chose profile based on security policy, signal strength, etc parameters
 * 
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiWLanAddProfile(HANDLE hApi, TCHAR *ssid, BYTE secType, TCHAR *seckey, BYTE priority);

/** @fn int NURAPICONV NurApiWLanDelProfile(HANDLE hApi, BYTE index)
 *
 * Deleting a certain stored profile, or for deleting all profiles at once. 
 *
 * @sa NurApiWLanGetProfile, NurApiWLanAddProfile.
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	index			Index of profile to delete (1-7) 0 = Delete all profiles.  
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiWLanDelProfile(HANDLE hApi, BYTE index);

/** @fn int NURAPICONV NurApiWLanFileWrite(HANDLE hApi, TCHAR *name, DWORD length, BYTE *data)
 *
 * Write file to file system of WLAN module.
 *
 * @sa NurApiWLanFileDelete.
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	name			Name/path of file where to store in the module.
 * @param	length			Length of file in bytes.
 * @param	data			Pointer to file data buffer.
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiWLanFileWrite(HANDLE hApi, TCHAR *name, DWORD length, BYTE *data);

/** @fn int NURAPICONV NurApiWLanFileDelete(HANDLE hApi, TCHAR *name)
 *
 * Delete file from file system of WLAN module
 *
 * @sa NurApiWLanFileWrite.
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	name			name/path of file to delete. 
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiWLanFileDelete(HANDLE hApi, TCHAR *name);


/** @fn int NURAPICONV NurApiGen2v2Untraceable(HANDLE hApi, DWORD passwd, struct NUR_UNTRACEABLE_PARAM *pUtrace, DWORD szParam)
 *
 * Performs the Gen2 version 2 "Untraceable" command without explicitly selecting a tag.
 *
 * @sa NurApiGen2v2UntraceableByEPC, NurApiGen2v2Untraceable32, NUR_UNTRACEABLE_PARAM
 * @sa NUR_UTRACE_RANGE_NORMAL, NUR_UTRACE_RANGE_REDUCE
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	passwd			Untraceable always executes in secured state; must be valid.
 * @param	pUtrace			Pointer to a valid untraceable command's parameter structure.
 * @param	szParam			Size of the parameter structure.
 * 
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGen2v2Untraceable(HANDLE hApi, DWORD passwd, struct NUR_UNTRACEABLE_PARAM *pUtrace, DWORD szParam);

/** @fn int NURAPICONV NurApiGen2v2UntraceableByEPC(HANDLE hApi, DWORD passwd, BYTE *epcBuffer, DWORD epcBufferLen, struct NUR_UNTRACEABLE_PARAM *pUtrace, DWORD szParam)
 *
 * Performs the Gen2 version 2 "Untraceable" command using the tag's EPC as selection parameter.
 *
 * @sa NurApiGen2v2Untraceable, NurApiGen2v2Untraceable32, NUR_UNTRACEABLE_PARAM
 * @sa NUR_UTRACE_RANGE_NORMAL, NUR_UTRACE_RANGE_REDUCE
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	passwd			Untraceable always executes in secured state; must be valid.
 * @param	epcBuffer		Pointer to the tag's expected EPC constents starting from bit addresss 32 (word addres 2) in the EPC bank.
 * @param	epcBufferLen	Length, in bytes, of the EPC buffer.
 * @param	pUtrace			Pointer to a valid untraceable command's parameter structure.
 * @param	szParam			Size of the parameter structure.
 * 
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGen2v2UntraceableByEPC(HANDLE hApi, DWORD passwd, BYTE *epcBuffer, DWORD epcBufferLen, struct NUR_UNTRACEABLE_PARAM *pUtrace, DWORD szParam);

/** @fn int NURAPICONV NurApiGen2v2Untraceable32(HANDLE hApi, DWORD passwd, BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask, struct NUR_UNTRACEABLE_PARAM *pUtrace, DWORD szParam)
 *
 * Performs the Gen2 version 2 "Untraceable" command with explicit selection criteria.
 *
 * @sa NurApiGen2v2Untraceable, NurApiGen2v2UntraceableByEPC, NUR_UNTRACEABLE_PARAM
 * @sa NUR_UTRACE_RANGE_NORMAL, NUR_UTRACE_RANGE_REDUCE
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	passwd			Untraceable always executes in secured state; must be valid.
 * @param	sBank			Bank where the selection is applied to (EPC, TID or user memory).
 * @param	sAddress		Selection mask's bit address.
 * @param	sMaskBitLength	Selection mask's length in bits.
 * @param	sMask			Pointer to the selection mask data.
 * @param	pUtrace			Pointer to a valid untraceable command's parameter structure.
 * @param	szParam			Size of the parameter structure.
 * 
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiGen2v2Untraceable32(HANDLE hApi, DWORD passwd, BYTE sBank, DWORD sAddress, 
								   int sMaskBitLength, BYTE *sMask, struct NUR_UNTRACEABLE_PARAM *pUtrace, DWORD szParam);

/** @fn int NURAPICONV NurApiGen2v2Authenticate32(HANDLE hApi, BOOL secured, DWORD passwd, BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask, struct NUR_AUTHENTICATE_PARAM *pAuth, DWORD szParam, struct NUR_AUTHENTICATE_RESP *pResp)
 *
 * Performs the Gen2 version 2 "Authenticate" command with specific selection parameters.
 *
 * @sa NurApiGen2v2AuthenticateByEPC, NurApiGen2v2Authenticate
 * @sa NUR_AUTHENTICATE_PARAM, NUR_AUTHENTICATE_RESP
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	secured			If TRUE then the operation is done in secured state (NOT RECOMMENDED).
 * @param	passwd			Password to use in the secured state.
 * @param	sBank			Bank where the selection is applied to (EPC, TID or user memory).
 * @param	sAddress		Selection mask's bit address.
 * @param	sMaskBitLength	Selection mask's length in bits.
 * @param	sMask			Pointer to the selection mask data.
 * @param	pAuth			Authentication parameters. 
 * @param	szParam			Size of the parameter structure.
 * @param	pResp			Response to the authentication (tag's reply if present).
 * 
 * @return	Zero when succeeded, on error non-zero error code is returned if the error is due to invalid parameters or communication failure.
 * @remarks The function may return zero also when the authentication itself failed; the there is additional information in the response structure.
 */
NUR_API
int NURAPICONV NurApiGen2v2Authenticate32(HANDLE hApi, BOOL secured, DWORD passwd, BYTE sBank, DWORD sAddress, 
										  int sMaskBitLength, BYTE *sMask, struct NUR_AUTHENTICATE_PARAM *pAuth, DWORD szParam, struct NUR_AUTHENTICATE_RESP *pResp);

/** @fn int NURAPICONV NurApiGen2v2AuthenticateByEPC(HANDLE hApi, BOOL secured, DWORD passwd, BYTE *epcBuffer, DWORD epcBufferLen, struct NUR_AUTHENTICATE_PARAM *pAuth, DWORD szParam, struct NUR_AUTHENTICATE_RESP *pResp)
 *
 * Performs the Gen2 version 2 "Authenticate" command using the tag's EPC as selection parameter.
 *
 * @sa NurApiGen2v2Authenticate32, NurApiGen2v2Authenticate
 * @sa NUR_AUTHENTICATE_PARAM, NUR_AUTHENTICATE_RESP
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	secured			If TRUE then the operation is done in secured state (NOT RECOMMENDED).
 * @param	passwd			Password to use in the secured state.
 * @param	epcBuffer		Pointer to the tag's expected EPC constents starting from bit addresss 32 (word addres 2) in the EPC bank.
 * @param	epcBufferLen	Length, in bytes, of the EPC buffer.
 * @param	pAuth			Authentication parameters. 
 * @param	szParam			Size of the parameter structure.
 * @param	pResp			Response to the authentication (tag's reply if present).
 * 
 * @return	Zero when succeeded, on error non-zero error code is returned if the error is due to invalid parameters or communication failure.
 * @remarks The function may return zero also when the authentication itself failed; the there is additional information in the response structure.
 */
NUR_API
int NURAPICONV NURAPICONV NurApiGen2v2AuthenticateByEPC(HANDLE hApi, BOOL secured, DWORD passwd, BYTE *epcBuffer, DWORD epcBufferLen, 
												  struct NUR_AUTHENTICATE_PARAM *pAuth, DWORD szParam, struct NUR_AUTHENTICATE_RESP *pResp);

/** @fn int NURAPICONV NurApiGen2v2Authenticate(HANDLE hApi, BOOL secured, DWORD passwd, struct NUR_AUTHENTICATE_PARAM *pAuth, DWORD szParam, struct NUR_AUTHENTICATE_RESP *pResp)
 *
 * Performs the Gen2 version 2 "Authenticate" command without explicitly selecting a tag.
 *
 * @sa NurApiGen2v2AuthenticateByEPC, NurApiGen2v2Authenticate32
 * @sa NUR_AUTHENTICATE_PARAM, NUR_AUTHENTICATE_RESP
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	secured			If TRUE then the operation is done in secured state (NOT RECOMMENDED).
 * @param	passwd			Password to use in the secured state.
 * @param	pAuth			Authentication parameters. 
 * @param	szParam			Size of the parameter structure.
 * @param	pResp			Response to the authentication (tag's reply if present).
 * 
 * @return	Zero when succeeded, on error non-zero error code is returned if the error is due to invalid parameters or communication failure.
 * @remarks The function may return zero also when the authentication itself failed; the there is additional information in the response structure.
 */
NUR_API
int NURAPICONV NurApiGen2v2Authenticate(HANDLE hApi, BOOL secured, DWORD passwd, struct NUR_AUTHENTICATE_PARAM *pAuth, DWORD szParam, struct NUR_AUTHENTICATE_RESP *pResp);

/** @fn int NURAPICONV NurApiGen2v2ReadBuffer(HANDLE hApi, BOOL secured, DWORD passwd, WORD bitAddress, WORD bitCount, BYTE *buffer, int *actualBits)
 *
 * Performs the Gen2 version 2 "ReadBuffer" command without tag singulation.
 *
 * @sa NurApiGen2v2ReadBuffer32, NurApiGen2v2ReadBufferByEPC, MAX_V2_RDBUFBITS, VER2_RDBUF_MAXADDR
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	secured			If TRUE then the operation is done in secured state.
 * @param	passwd			Password to use in the secured state.
 * @param	bitAddress		Bit address to start the read from. Range is 0...0xFFF.
 * @param	bitCount		Number of bits to read from the tag's internal buffer. Range is 1...1000.
 * @param	buffer			The tag's reply is stored here.
 * @param	actualBits		Pointer to WORD variable where the number of bits actually received from the tag is stored into. 
 * 
 * @return	Zero when succeeded, on error non-zero error code is returned if the error is due to invalid parameters or communication failure.
 */
NUR_API
int NURAPICONV NurApiGen2v2ReadBuffer(HANDLE hApi, BOOL secured, DWORD passwd, WORD bitAddress, WORD bitCount, BYTE *buffer, int *actualBits);

/** @fn int NURAPICONV NurApiGen2v2ReadBuffer32(HANDLE hApi, BOOL secured, DWORD passwd, BYTE sBank, DWORD sAddress, DWORD sMaskBitLength, BYTE *sMask, WORD bitAddress, WORD bitCount, BYTE *buffer, int *actualBits)
 *
 * Performs the Gen2 version 2 "ReadBuffer" command  with specific selection parameters.
 *
 * @sa NurApiGen2v2ReadBuffer, NurApiGen2v2ReadBufferByEPC, MAX_V2_RDBUFBITS, VER2_RDBUF_MAXADDR
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	secured			If TRUE then the operation is done in secured state.
 * @param	passwd			Password to use in the secured state.
 * @param	sBank			Bank where the selection is applied to (EPC, TID or user memory).
 * @param	sAddress		Selection mask's bit address.
 * @param	sMaskBitLength	Selection mask's length in bits.
 * @param	sMask			Pointer to the selection mask data.
 * @param	bitAddress		Bit address to start the read from. Range is 0...0xFFF.
 * @param	bitCount		Number of bits to read from the tag's internal buffer. Range is 1...1000.
 * @param	buffer			The tag's reply is stored here.
 * @param	actualBits		Pointer to WORD variable where the number of bits actually received from the tag is stored into. 
 * 
 * @return	Zero when succeeded, on error non-zero error code is returned if the error is due to invalid parameters or communication failure.
 */
NUR_API
int NURAPICONV NurApiGen2v2ReadBuffer32(HANDLE hApi, BOOL secured, DWORD passwd, BYTE sBank, DWORD sAddress, DWORD sMaskBitLength, BYTE *sMask, WORD bitAddress, WORD bitCount, BYTE *buffer, int *actualBits);

/** @fn int NURAPICONV NurApiGen2v2ReadBufferByEPC(HANDLE hApi, BOOL secured, DWORD passwd, BYTE *epcBuffer, DWORD epcBufferLen, WORD bitAddress, WORD bitCount, BYTE *buffer, int *actualBits)
 *
 * Performs the Gen2 version 2 "ReadBuffer" command by singulating the tag with its EPC contents.
 *
 * @sa NurApiGen2v2ReadBuffer, NurApiGen2v2ReadBuffer32, MAX_V2_RDBUFBITS, VER2_RDBUF_MAXADDR
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	secured			If TRUE then the operation is done in secured state.
 * @param	passwd			Password to use in the secured state.
 * @param	epcBuffer		Pointer to the tag's expected EPC constents starting from bit addresss 32 (word addres 2) in the EPC bank.
 * @param	epcBufferLen	Length, in bytes, of the EPC buffer.
 * @param	bitAddress		Bit address to start the read from. Range is 0...0xFFF.
 * @param	bitCount		Number of bits to read from the tag's internal buffer. Range is 1...1000.
 * @param	buffer			The tag's reply is stored here.
 * @param	actualBits		Pointer to WORD variable where the number of bits actually received from the tag is stored into. 
 * 
 * @return	Zero when succeeded, on error non-zero error code is returned if the error is due to invalid parameters or communication failure.
 */
NUR_API
int NURAPICONV NurApiGen2v2ReadBufferByEPC(HANDLE hApi, BOOL secured, DWORD passwd, BYTE *epcBuffer, DWORD epcBufferLen, WORD bitAddress, WORD bitCount, BYTE *buffer, int *actualBits);


/** @fn int NURAPICONV NurApiAES128_ECBEncrypt(BYTE* input, const BYTE* key, BYTE *output)
 *
 * Performs one 16-byte (128-bit) block AES ECB encryption.
 *
 * @sa NurApiAES128_ECBDecrypt, NurApiAES128_CBCEncrypt, NurApiAES128_CBCDecrypt
 * 	   
 * @param	input		Input data. Must not be NULL and length is 16 bytes (128 bits).
 * @param	output		Output is stored here. Must not be NULL and minimum required allocation is 16 bytes (128 bits).
 * @param	key			Key to use. Must be 16 bytes (128 bits) long.
 * 
 * @return	Zero when succeeded. Error can only be parameter error.
 */
NUR_API
int NURAPICONV NurApiAES128_ECBEncrypt(BYTE *input, const BYTE *key, BYTE *output);

/** @fn int NURAPICONV NurApiAES128_ECBDecrypt(BYTE *input, const BYTE *key, BYTE *output)
 *
 * Performs one 16-byte (128-bit) block AES ECB decryption.
 *
 * @sa NurApiAES128_ECBEncrypt, NurApiAES128_CBCEncrypt, NurApiAES128_CBCDecrypt
 * 	   
 * @param	input		Input data. Must not be NULL and length is 16 bytes (128 bits).
 * @param	output		Output is stored here. Must not be NULL and minimum required allocation is 16 bytes (128 bits).
 * @param	key			Key to use. Must not be NULL and length is 16 bytes (128 bits).
 * 
 * @return	Zero when succeeded. Error can only be parameter error.
 */
NUR_API
int NURAPICONV NurApiAES128_ECBDecrypt(BYTE *input, const BYTE *key, BYTE *output);

/** @fn int NURAPICONV NurApiAES128_CBCEncrypt(BYTE *input, DWORD length, const BYTE *key, const BYTE *iv, BYTE *output)
 *
 * Performs AES CBC encryption. There may be several 16-byte (128-bit) blocks as an input.
 *
 * @sa NurApiAES128_ECBEncrypt, NurApiAES128_ECBDecrypt, NurApiAES128_CBCDecrypt
 * 	   
 * @param	input	Input data. Must not be NULL, minimum length is 16 bytes (128 bits) and total length is aligned by 16.
 * @param	length	Minimum value is 16 and with larger values the alignment must be 16.
 * @param	key		Key to use. Must not be NULL and length is 16 bytes (128 bits).
 * @param	iv		The Initialization Vector (IV) to use. Must not be NULL and length is 16 bytes (128 bits).
 * @param	output	Output is stored here. Must not be NULL and minimum required allocation is 16 bytes (128 bits).
 * 
 * @return	Zero when succeeded. Error can only be parameter error.
 */
NUR_API
int NURAPICONV NurApiAES128_CBCEncrypt(BYTE *input, DWORD length, const BYTE *key, const BYTE *iv, BYTE *output);

/** @fn int NURAPICONV NurApiAES128_CBCDecrypt(BYTE *input, DWORD length, const BYTE *key, const BYTE *iv, BYTE *output)
 *
 * Performs AES CBC decryption. There may be several 16-byte (128-bit) blocks as an input.
 *
 * @sa NurApiAES128_ECBEncrypt, NurApiAES128_ECBDecrypt, NurApiAES128_CBCEncrypt
 * 	   
 * @param	input	Input data. Must not be NULL, minimum length is 16 bytes (128 bits) and total length is aligned by 16.
 * @param	length	Minimum value is 16 and with larger values the alignment must be 16.
 * @param	key		Key to use. Must not be NULL and length is 16 bytes (128 bits).
 * @param	iv		The Initialization Vector (IV) to use. Must not be NULL and length is 16 bytes (128 bits).
 * @param	output	Output is stored here. Must not be NULL and minimum required allocation is 16 bytes (128 bits).
 * 
 * @return	Zero when succeeded. Error can only be parameter error.
 */
NUR_API
int NURAPICONV NurApiAES128_CBCDecrypt(BYTE *input, DWORD length, const BYTE *key, const BYTE *iv, BYTE *output);

/** @fn int NURAPICONV NurApiISO29167_10_TAM32(HANDLE hApi, BYTE sBank, DWORD sAddress, int sMaskBitLength, BYTE *sMask, struct NUR_TAM_PARAM *pTAM, struct NUR_TAM_RESP *pTAMResp)
 *
 * Performs the ISO29167-10 Tag Authentication Method (TAM) 1 or 2 specified by the parameters using specific tag selection parameters.
 *
 * @sa NurApiISO29167_10_TAM32, NurApiISO29167_10_TAMByEPC, NurApiISO29167_10_TAM, NUR_TAM_PARAM, NUR_TAM_RESP
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	sBank			Bank where the selection is applied to (EPC, TID or user memory).
 * @param	sAddress		Selection mask's bit address.
 * @param	sMaskBitLength	Selection mask's length in bits.
 * @param	sMask			Pointer to the selection mask data.
 * @param	pTAM			Pointer to the TAM parameter structure.
 * @param	pTAMResp		Pointer to the TAM response structure.
 * 
 * @return	Zero when succeeded. If an error occurs then an error code is returned and the response may contain additional information about the error.
 */
NUR_API
int NURAPICONV NurApiISO29167_10_TAM32(HANDLE hApi, BYTE sBank, DWORD sAddress, 
									  int sMaskBitLength, BYTE *sMask, struct NUR_TAM_PARAM *pTAM, struct NUR_TAM_RESP *pTAMResp);

/** @fn int NURAPICONV NurApiISO29167_10_TAMByEPC(HANDLE hApi, BYTE *epcBuffer, DWORD epcBufferLen, struct NUR_TAM_PARAM *pTAM, struct NUR_TAM_RESP *pTAMResp)
 *
 * Performs the ISO29167-10 Tag Authentication Method (TAM) 1 or 2 specified by the parameters using EPC singulation.
 *
 * @sa NurApiISO29167_10_TAM32, NurApiISO29167_10_TAM, NUR_TAM_PARAM, NUR_TAM_RESP
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	epcBuffer		Pointer to the tag's expected EPC constents starting from bit addresss 32 (word addres 2) in the EPC bank.
 * @param	epcBufferLen	Length, in bytes, of the EPC buffer.
 * @param	pTAM			Pointer to the TAM parameter structure.
 * @param	pTAMResp		Pointer to the TAM response structure.
 * 
 * @return	Zero when succeeded. If an error occurs then an error code is returned and the response may contain additional information about the error.
 */
NUR_API
int NURAPICONV NurApiISO29167_10_TAMByEPC(HANDLE hApi, BYTE *epcBuffer, DWORD epcBufferLen, struct NUR_TAM_PARAM *pTAM, struct NUR_TAM_RESP *pTAMResp);

/** @fn int NURAPICONV NurApiISO29167_10_TAM(HANDLE hApi, struct NUR_TAM_PARAM *pTAM, struct NUR_TAM_RESP *pTAMResp)
 *
 * Performs the ISO29167-10 Tag Authentication Method (TAM) 1 or 2 specified by the parameters using specific tag selection parameters.
 *
 * @sa NurApiISO29167_10_TAM32, NurApiISO29167_10_TAMByEPC, NUR_TAM_PARAM, NUR_TAM_RESP
 * 	   
 * @param	hApi			Handle to valid NurApi object instance. 
 * @param	pTAM			Pointer to the TAM parameter structure.
 * @param	pTAMResp		Pointer to the TAM response structure.
 * 
 * @return	Zero when succeeded. If an error occurs then an error code is returned and the response may contain additional information about the error.
 */
NUR_API
int NURAPICONV NurApiISO29167_10_TAM(HANDLE hApi, struct NUR_TAM_PARAM *pTAM, struct NUR_TAM_RESP *pTAMResp);

/** @fn int NurApiStartTagTracking(HANDLE hApi, struct NUR_TAGTRACKING_CONFIG *cfg, DWORD cfgSize)
 *
 * Start tag tracking streaming on module. 
 * Module will start tag tracking on the module and send tag tracking events to host as notification.
 * The notifications are configured in the parameter NUR_TAGTRACKING_CONFIG flags. 
 *
 * @sa NurApiStopTagTracking(), NUR_TAGTRACKING_CONFIG, NUR_TAGTRACKING_FLAGS
 *
 * @param	hApi			Handle to valid NurApi object instance.
 * @param	cfg				Pointer to NUR_TAGTRACKING_CONFIG parameters.
 * @param	cfgSize			Size of <i>cfg</i> structure in bytes..
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStartTagTracking(HANDLE hApi, struct NUR_TAGTRACKING_CONFIG *cfg, DWORD cfgSize);

/** @fn int NurApiStopTagTracking(HANDLE hApi)
 *
 * Stop tag tracking streaming on module. 
 *
 * @param	hApi	Handle to valid NurApi object instance
 *
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiStopTagTracking(HANDLE hApi);

/** @fn BOOL NurApiIsTagTrackingRunning(HANDLE hApi)
 *
 * Return TRUE if tag tracking stream routine is running on module.
 *
 * @param	hApi	Handle to valid NurApi object instance
 *
 * @return	TRUE if tag tracking stream routine is running. FALSE otherwise.
 */
NUR_API
BOOL NURAPICONV NurApiIsTagTrackingRunning(HANDLE hApi);

/** @fn int NurApiTagTrackingGetTags(HANDLE hApi, DWORD events, struct NUR_TT_TAG *tagDataBuffer, int *tagDataCount, DWORD szSingleEntry)
 *
 * Get tag tracking tags.
 * 
 * @sa NurApiGetTagCount(), NurApiClearTags(), NUR_TT_TAG
 * 
 * @param	hApi				Handle to valid NurApi object instance.
 * @param	events				Which tags to include to the return value. For values to input @sa NUR_TAGTRACKING_EVENTS
 * @param   tagDataBuffer		Pointer to a NUR_TT_TAG structures. Must contain at least <i>tagDataCount</i> entries of NUR_TT_TAG.
 * @param	tagDataCount		Number of entries in <i>tagDataBuffer</i>. On return number of valid entries is received in this pointer.
 * @param	szSingleEntry		Size of one NUR_TT_TAG entry.
 * 					
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiTagTrackingGetTags(HANDLE hApi, DWORD events, struct NUR_TT_TAG *tagDataBuffer, int *tagDataCount, DWORD szSingleEntry);

/** @fn int NurApiSetHostFlags(HANDLE hApi, TCHAR* resp, DWORD hostFlags)
 * Set module host flags. Uses NurApiPing to send the selected flags to the device. 
 * @param	hApi	Handle to valid NurApi object instance.
 * @param	resp	Pointer to a buffer that receives ping response. This parameter can be NULL. <b>NOTE: If not NULL, buffer must be atleast 16 TCHARs long</b>.
 * @param	hostFlags Host configuration flags. See NUR_HOSTFLAGS
 * @return	Zero when succeeded, On error non-zero error code is returned.
 */
NUR_API
int NURAPICONV NurApiSetHostFlags(HANDLE hApi, TCHAR* resp, DWORD hostFlags);

/** @example NurApiExample.cpp
 * Simple console application project.
 */
/** @example ReadWriteExample.cpp
 * Example for how to use read & write functions
 */
/** @example SetupExample.cpp
 * Example of Setup functions.
 */
/** @example GpioExample.cpp
 * Example of GPIO functions.
 */

/** @} */ // end of API

/*! \mainpage Nur API Index
 *
 * NurApi C/C++ documentation for programming Nordic ID UHF RFID module
 * 
 * \section intro_sec Introduction
 *
 * Nur API allows Windows developers to create applications for Nordic ID NUR products with C/C++ programming languages.
 * 
 * API Includes:
 * @li This documentation
 * @li Needed prebuilt libraries and headers
 * @li Sample program with Microsoft visual studio 2005 project files
 *
 * \section devtools_sec Development tools
 *
 * @li O/S: Microsoft Windows XP (SP3 or later), Vista or Windows 7
 * @li Languages: C/C++, however any language that can load dll can be used.
 * @li Development environment: Microsoft visual studio 2005 / 2008 / 2010
 * 
 * \section build_sec Build settings
 * Nur API dll is built with following settings:
 * @li Runtime Library: Multi-threaded (/MT)
 * @li Character Set: Use Unicode Character Set
 * @li All functions calling convention is <b>__stdcall</b>
 *  
 * \section devtools_sec2 Linux support
 *
 * @li API is available for x86, x64 and various ARM architectures (Raspberry Pi, BeagleBone, Linaro, ...)
 * @li Languages: C/C++
 * @li Development environment: any cross-compilation environment that can produce the code for the used platform
 * @li Requires additional linking of math (-lm) and thread (-lpthread) libraries
 * @li May need -fsigned-char command line option when compiled if not default
 *  
 * \section doc_change Changes in this document
 * This is not a preliminary documentation. However, the API will develop further over the time.
 */

#ifdef __cplusplus 
}
#endif

#endif
