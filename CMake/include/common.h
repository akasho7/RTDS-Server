﻿#ifndef COMMON_H
#define COMMON_H

#define RTDS_DUAL_STACK					// Enable IPV6 support
#define RTDS_PORT 389					// Default RTDS port number

#define PRINT_DEBUG_LOG					// Print debug logs
#define PRINT_LOG						// Print critical console logs

#define STR_V4 "v4"						// Version V4 in string
#define STR_V6 "v6"						// Version V6 in string

#define RTDS_BUFF_SIZE 330				// Maximum size of the readBuffer
#define MAX_BGID_SIZE 128				// Maximum size of BGID
#define MAX_BROADCAST_SIZE 256			// Maximum size of B data
#define MAX_TAG_SIZE 32					// Maximum size of Tag
#define MAX_MSG_CACHE_SIZE 128			// Maximum number of messages to be cached
#define MIN_MSG_KEEP_TIME 1				// Minimum number of minutes to keep the message

#include <array>
typedef std::array<char, RTDS_BUFF_SIZE + 1> ReceiveBuffer;
typedef std::string BGID;
typedef std::string BGT;

/*******************************************************************************************
* @brief Enum class for Response
*
* @details
* ok_success		Command is successful.
* bad_command		Bad command.
* bad_param			Bad parameter provided with the command.
* wait_retry		Wait and try again after sometime.
* is_listening		Peer is already listening to a BG.
* not_listening		Peer is not listening to any BG.
********************************************************************************************/
enum class Response
{
	SUCCESS = 0,
	BAD_COMMAND = 1,
	BAD_PARAM = 2,
	WAIT_RETRY = 3,
	IS_IN_BG = 4,
	NOT_IN_BG = 5
};

/*******************************************************************************************
* @brief Enum class for Command
*
* @details
* broadcast			Broadcast a message to the BG.
* ping				Ping with the RTDS server.
* listen			Listen to the activities in a Broadcast Group.
* change			Change the Broadcast Group.
* leave				Stop listening the Broadcast group.
* exit				Terminate the connection.
********************************************************************************************/
enum class Command
{
	BROADCAST = 0,
	PING = 1,
	LISTEN = 2,
	LEAVE = 3,
	EXIT = 4
};

#endif