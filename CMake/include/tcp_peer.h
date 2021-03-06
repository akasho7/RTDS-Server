#ifndef TCP_PEER_H
#define TCP_PEER_H

#include <asio/ip/tcp.hpp>
#include <atomic>
#include "message.h"
#include "sapair.h"
#include "common.h"
#include "advanced_buffer.h"

class BGroup;
class TCPpeer
{		
	static std::atomic_int mPeerCount;		// Keep the total count of peers
	
	AdancedBuffer mDataBuffer;				// Buffer to which the commands are received
	asio::ip::tcp::socket* mPeerSocket;		// Socket handling the data from peer system
	const SApair mSApair;					// Source address pair of this peer

	BGroup* mBgPtr;							// Pointer to broadcast group
	BGID mBgID;								// Broadcast group ID
	BGT mBgTag;								// Broadcast group Tag

	bool mPeerIsActive;						// True if the peer socket is operational
	bool mIsInBG;							// True if this peer is in Broadcast Group

/*******************************************************************************************
* @brief Shedule a send for writeBuffer contents to the peer system
*
* @details
* The callback function _sendFeedback() will be invoked after the data is send.
* The callback function will be called even if thier is a error in tcp connection.
********************************************************************************************/
	void mSendPeerBufferData();
/*******************************************************************************************
 * @brief Shedule handler funtion for peerSocket to receive the data in dataBuffer[]
 *
 * @details
 * The callback function _processData() will be invoked when their is new data in buffer.
 * The callback function will be called even if thier is a error in tcp connection.
 ********************************************************************************************/
	void mPeerReceiveData();
/*******************************************************************************************
* @brief The callback function for getting (new data / socket error)
*
* @param[in] ec					Asio error code
* @param[in] size				Number of bytes received
*
* @details
* Append receved data with '\0' to make it string
* Pass the command to the command interpreter to process the command.
* Send back Response for the received command.
* If ec state a error in connection, this peer object will be deleted.
********************************************************************************************/
	void mProcessData(const asio::error_code&, std::size_t);
/*******************************************************************************************
* @brief This callback function will be called after sending data in the write buffer
*
* @param[in] ec					Asio error code
*
* @details
* If ec state a error in connection, this object will be deleted.
* If the connection is ok then, clear the write buffer and register for next receive.
********************************************************************************************/
	void mSendFuncFeedbk(const asio::error_code&);
/*******************************************************************************************
* @brief This callback function will be called after sending message
*
* @param[in] ec					Asio error code
*
* @details
* If ec state a error in connection, signal peer object to be deleted.
********************************************************************************************/
	void mSendMssgFuncFeedbk(const asio::error_code&);
/*******************************************************************************************
* @brief Close and delete peerSocket
*
* @details
* Leave the BG if in listening mode.
* Decrement peer count. Shutdown and delete peer socket.
********************************************************************************************/
	~TCPpeer();

public:
/*******************************************************************************************
* @brief Create a Peer object with an accepted socketPtr*
*
* @param[in]			Pointer to the newly accepted socket
*
* @details
* Reserve buffer size and get the peer endpoint.
* Create a SourceAddressPair with the pointer to the socket. 
********************************************************************************************/
	TCPpeer(asio::ip::tcp::socket*);
/*******************************************************************************************
* @brief Return the number of peers
*
* @return				Return peer count
********************************************************************************************/
	static int peerCount();
/*******************************************************************************************
* @brief Return the string view of the received command
*
* @return				Return peer count
********************************************************************************************/
	std::string_view getCommandString();
/*******************************************************************************************
* @brief Shedule a send for message to the peer system
*
* @param[in]			Message to be send
* @param[in]			Message Tag
*
* @details
* Only send the message if the tags are compatible.
* The callback function _sendMFeedback() will be invoked after the data is send.
* The callback function will be called even if thier is a error in tcp connection.
********************************************************************************************/
	void sendMessage(const Message*, const std::string_view&);
	void sendMessage(const Message*);

/*******************************************************************************************
* @brief Disconnect the peer and delete the object
********************************************************************************************/
	void disconnect();
/*******************************************************************************************
* @brief Start listening to a brodcast group
*
* @param[in]			Broadcast Group ID
* @param[in]			Broadcast Group Tag
*
* @details
* Send WAIT_RETRY if peer failed to join the broadcast group
* Send SUCCESS if the joining was success 
********************************************************************************************/
	void listenTo(const std::string_view&, const std::string_view&);
/*******************************************************************************************
* @brief Leave the brodcast group
********************************************************************************************/
	void leaveBG();
/*******************************************************************************************
* @brief Print the source address pair info to the write buffer
*
* @details
* Print Version, IP address and port number
********************************************************************************************/
	void printPingInfo();
/*******************************************************************************************
* @brief Send response to the peer
*
* @param[in]			Response
********************************************************************************************/
	void respondWith(const Response);
/*******************************************************************************************
* @brief Broadcast a message to the group
*
* @param[in]			Message
* @param[in]			Broadcast Group Tag
********************************************************************************************/
	void broadcast(const std::string_view&);
	void broadcast(const std::string_view&, const std::string_view&);
};

#endif