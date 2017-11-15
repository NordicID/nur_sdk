#ifndef _NURAPI_ERRORS_H_
#define _NURAPI_ERRORS_H_ 1

#include "NurAPIExport.h"

#ifdef __cplusplus 
extern "C" { 
#endif

/** @addtogroup API
 *  @{
 */

/**
 * NurApi error codes
 */
enum NUR_ERRORCODES
{
	/** Call succeeded; Same as NUR_NO_ERROR  */
	NUR_SUCCESS = 0,
	/** Call succeeded; Same as NUR_SUCCESS */
	NUR_NO_ERROR = NUR_SUCCESS,
	/** Invalid command sent to module */
	NUR_ERROR_INVALID_COMMAND,
	/** Invalid packet length sent to module */
	NUR_ERROR_INVALID_LENGTH,
	/** Command parametr(s) out of range */
	NUR_ERROR_PARAMETER_OUT_OF_RANGE,
	/** Data receive timeout */
	NUR_ERROR_RECEIVE_TIMEOUT,
	/** Invalid command parameter(s); Invalid function parameter(s) */
	NUR_ERROR_INVALID_PARAMETER,
	/** Programming failure */
	NUR_ERROR_PROGRAM_FAILED,
	/** Parameter mismatch */
	NUR_ERROR_PARAMETER_MISMATCH,
	/** HW mismatch */
	NUR_ERROR_HW_MISMATCH,
	NUR_ERROR_RESERVED1,
	/** Page programming failure */
	NUR_ERROR_PAGE_PROGRAM,
	/** Memory check failed */
	NUR_ERROR_CRC_CHECK,
	/** CRC mismatch in parameter */
	NUR_ERROR_CRC_MISMATCH,
	/** Device not ready or region that is being programmed is not unlocked */
	NUR_ERROR_NOT_READY,
	/** Module application not present */
	NUR_ERROR_APP_NOT_PRESENT,

	/** Generic, non-interpreted / unexpected error */
	NUR_ERROR_GENERAL = 0x10,

	/** No tag(s) found */
	NUR_ERROR_NO_TAG = 0x20,
	/** Air error */
	NUR_ERROR_RESP_AIR,
	/** G2 select error */
	NUR_ERROR_G2_SELECT,
	/** G2 select data missing */
	NUR_ERROR_MISSING_SELDATA,
	/** G2 access error */
	NUR_ERROR_G2_ACCESS,

	/** G2 Read error, unspecified */
	NUR_ERROR_G2_READ = 0x30,
	/** G2 Partially successful read */
	NUR_ERROR_G2_RD_PART,
	/** G2 Write error, unspecified */
	NUR_ERROR_G2_WRITE = 0x40,
	/** G2 Partially successful write */
	NUR_ERROR_G2_WR_PART,
	/** G2 Tag read responded w/ error */
	NUR_ERROR_G2_TAG_RESP,

	/** Special error; Some additional debug data is returned with this error */
	NUR_ERROR_G2_SPECIAL = 0x50,

	/** HW error */
	NUR_ERROR_READER_HW = 0x60,
	/** Antenna too bad */
	NUR_ERROR_BAD_ANTENNA,
	/** Low voltage */
	NUR_ERROR_LOW_VOLTAGE,
	/** Over temperature */
	NUR_ERROR_OVER_TEMP,

	/** Invalid handle passed to function */
	NUR_ERROR_INVALID_HANDLE = 0x1000,
	/** Transport error */
	NUR_ERROR_TRANSPORT,
	/** Transport not connected */
	NUR_ERROR_TR_NOT_CONNECTED,
	/** Transport timeout */
	NUR_ERROR_TR_TIMEOUT,
	/** Buffer too small */
	NUR_ERROR_BUFFER_TOO_SMALL,
	/** Functionality not supported */
	NUR_ERROR_NOT_SUPPORTED,
	/** Packet contains no payload */
	NUR_ERROR_NO_PAYLOAD,
	/** Packet is invalid */
	NUR_ERROR_INVALID_PACKET,
	/** Packet too long */
	NUR_ERROR_PACKET_TOO_LONG,
	/** Packet Checksum failure */
	NUR_ERROR_PACKET_CS_ERROR,
	/** Data not in WORD boundary */
	NUR_ERROR_NOT_WORD_BOUNDARY,
	/** File not found */
	NUR_ERROR_FILE_NOT_FOUND,
	/** File error; not in NUR format */
	NUR_ERROR_FILE_INVALID,	
	/** NUR file and module's MCU architecture mismatch  */
	NUR_ERROR_MCU_ARCH,		

	/** The specified memory location does not exists or the EPC length field is not supported by the tag */
	NUR_ERROR_G2_TAG_MEM_OVERRUN,
	/** The specified memory location is locked and/or permalocked and is either not writeable or not readable */
	NUR_ERROR_G2_TAG_MEM_LOCKED,
	/** The tag has insufficient power to perform the memory-write operation */
	NUR_ERROR_G2_TAG_INSUF_POWER,
	/** The tag does not support error-specific codes */
	NUR_ERROR_G2_TAG_NON_SPECIFIC,
	/** Transport suspended error */
	NUR_ERROR_TR_SUSPENDED,
	/** TCP/IP Server error */
	NUR_ERROR_SERVER,
	/** Device query is busy. */
	NUR_ERROR_QUERY_BUSY,
	/** Tag backscattered error code 0x00: "catch all" error. */
	NUR_ERROR_G2_TAG_OTHER_ERROR,		
	/** Tag backscattered error code 0x01: not supported parameters or feature. */
	NUR_ERROR_G2_TAG_NOT_SUPPORTED,
	/** Tag backscattered error code 0x04: insufficient priviledge. */
	NUR_ERROR_G2_TAG_INSUF_PRIVILEDGE,
	/** Tag backscattered error code 0x05: cryptographic suite error. */
	NUR_ERROR_G2_TAG_CRYPTO_SUITE,
	/** Tag backscattered error code 0x06: command was not encapsulated in AuthComm or SecureComm. */
	NUR_ERROR_G2_TAG_NOT_ENCAPSULATED,
	/** Tag backscattered error code 0x07: ResponseBuffer overflowed. */
	NUR_ERROR_G2_TAG_RESPBUFFER_OVF,
	/** Tag backscattered error code 0x10: failure because of security timeout. */
	NUR_ERROR_G2_TAG_SEC_TIMEOUT
};

/** Get human readable error message for error code
 * @param error		Error code
 * @param buffer	Buffer to store message
 * @param bufferLen	Length of buffer in characters. This should be atleast 128 characters length.
 * @return Number of characters stored in buffer.
 */
NUR_API
int NURAPICONV NurApiGetErrorMessage(int error, TCHAR *buffer, int bufferLen);

/** @} */ // end of API

#ifdef __cplusplus 
}
#endif

#endif
