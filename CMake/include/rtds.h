﻿#ifndef RTDS_H
#define RTDS_H

#include <asio.hpp>
#include "common.h"


class RTDS
{
	asio::io_context _ioContext;				// ioContext controls all the async functions related to ASIO
	asio::io_context::work _worker;				// Worker object to prevent ioContext.run() from exiting when without async jobs
	asio::ip::tcp::endpoint _tcpEp;				// TCP endpoint that describe the IPaddr ,Port and Protocol for the acceptor socket
	asio::ip::tcp::acceptor _tcpAcceptor;		// TCP acceptor socket that accept incoming tcp connections

	short _activeThreadCount = 0;				// Keep account of number of threads running ioContex.run()
	std::atomic<bool> _tcpServerRunning;		// Track if the TCP server is running
	std::atomic<bool> _keepAccepting;			// Only accept new connections if True

/*******************************************************************************************
 * @brief Contain activities to be done by a thread (async operation)
 *
 * @details
 * Run ioContext.run() function.
 * This function call will block all the threads untill the RTDS class object is distroyed.
 ********************************************************************************************/
	void _ioThreadJob();
/*******************************************************************************************
* @brief Function that defines how the new incoming TCP connections are to be accepted
*
* @details
* Make a socket X and try to accept the connection.
* If successfully connected, then make a dynamic Peer object using X(keepalive).
* If couldn't connect, then delete the socket X object and look for new connections.
********************************************************************************************/
	void _peerAcceptRoutine();
/*******************************************************************************************
* @brief Stop ioContext. All threads blocked by ioContext.run() will exit
*
* @details
* Block the caller till the ioContext is stopped and all threads have exited.
* 100 millisecond wait time and check again in loop
********************************************************************************************/
	void _stopIoContext();
/*******************************************************************************************
* @brief Cancell all async operations sheduled in acceptor socket and close it.
********************************************************************************************/
	void _stopTCPacceptor();

public:
/*******************************************************************************************
* @brief Create the server object with it's own ioContext and worker class object
*
* @param[in]		The default port is 389
*
* @details
* All the STL containers associated with this class are static in nature.
* Add #define RTDS_DUAL_STACK in RTDS.h to compile the RTDS in IPv6 dual stack mode.
* Start the logging system.
********************************************************************************************/
	RTDS(unsigned short = RTDS_PORT);
/*******************************************************************************************
* @brief Start the TCP server
*
* @param[in]		Number of threads with which RTDS starts.
* @return			True if the TCP server started sucessfully
*
* @details
* Open acceptor socket with the tcpEp.protocol(). Bind it to the tcp endpoint.
* Signal the OS to start listening for incomming connection requests. Start accepting.
********************************************************************************************/
	bool startTCPserver(const int threadCount = 1);
/*******************************************************************************************
* @brief Add X number of threads to run ioContex.run()
*
* @param[in]		Add X more threads to execute RTDS asyn tasks.
* @return			True if the threads are spwaned.
*
* @details
* Spawn child threads to run _ioThreadJob() and detach it from main thread.
********************************************************************************************/
	bool addThread(const int threadCount = 1);
/*******************************************************************************************
* @brief Add the calling thread to run ioContex.run()
*
* @details
* The calling thread will be blocked untill this RTDS object is destroyed
********************************************************************************************/
	void addThisThread();
/*******************************************************************************************
* @brief Start accepting new connections
********************************************************************************************/
	void startAccepting();
/*******************************************************************************************
* @brief Stop accepting new connections
********************************************************************************************/
	void stopAccepting();
/*******************************************************************************************
* @brief Stop the TCP server
*
* @details
* Stop TCP acceptor, reset the ioContext [ threads won't exit ][ all pending jobs discarded]
********************************************************************************************/
	void stopTCPserver();
/*******************************************************************************************
* @brief Stop the tcp server and ioContext
*
* @details
* Stop the TCP server, if it is running and stop the ioContext [ threads will exit ]
* Stop logging.
********************************************************************************************/
	~RTDS();
};

#endif