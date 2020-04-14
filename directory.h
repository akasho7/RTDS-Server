#pragma once
#include <map>
#include "sp_entry.h"

class Directory
{
	static std::map<sourcePairV4, EntryV4*> V4EntryMap;		//!< STL map mapping V4 SourcePair address to the EntryV4 pointer
	static std::map<sourcePairV6, EntryV6*> V6EntryMap;		//!< STL map mapping V6 SourcePair address to the EntryV6 pointer

	static std::mutex V4insertionLock;						//!< Lock this mutex before searching and insertion into map
	static std::mutex V6insertionLock;						//!< Lock this mutex before searching and insertion into map
public:
/*******************************************************************************************
* @brief Return a pointer to V4 Entry for the given IP4 address and port number
*
* @param[in] ipAddr				The IPv4 address
* @param[in] portNum			The port number
* @return						The pointer to the V4Entry
*
* @details
* Make a sourcePairV4 with the provided address and port number.
* Do insertion lock and check if an entry exists, yes then return it's pointer.
* No entry exists then generate EntryV4 dynamically, insert it to map and return it's pointer.
* Values of portNumber, UID, sourcePortV4 address and IP4 address will be initialized.
********************************************************************************************/
	static EntryV4* makeEntry(asio::ip::address_v4, unsigned short);
/*******************************************************************************************
* @brief Return a pointer to V6 Entry for the given IP6 address and port number
*
* @param[in] ipAddr				The IPv6 address
* @param[in] portNum			The port number
* @return						The pointer to the V6Entry
*
* @details
* Make a sourcePairV6 with the provided address and port number.
* Do insertion lock and check if an entry exists, yes then return it's pointer.
* No entry exists then generate EntryV6 dynamically, insert it to map and return it's pointer.
* Values of portNumber, UID, sourcePortV6 address and IP6 address will be initialized.
********************************************************************************************/
	static EntryV6* makeEntry(asio::ip::address_v6, unsigned short);
/*******************************************************************************************
* @brief Return a pointer to V4 Entry for the given IP4 address and port number
*
* @param[in] ipAddr				The IPv4 address
* @param[in] portNum			The port number
* @return						The pointer to the V4Entry or nullptr
*
* @details
* Make a sourcePairV4 with the provided address and port number.
* Search the sourcePortV4 address in the map and check if the entry have expired.
* If entry exists and is not expired then return the EntryV4* else return nullptr.
********************************************************************************************/
	static EntryV4* findEntry(asio::ip::address_v4&, unsigned short);
/*******************************************************************************************
* @brief Return a pointer to V6 Entry for the given IP6 address and port number
*
* @param[in] ipAddr				The IP64 address
* @param[in] portNum			The port number
* @return						The pointer to the V6Entry or nullptr
*
* @details
* Make a sourcePairV6 with the provided address and port number.
* Search the sourcePortV6 address in the map and check if the entry have expired.
* If entry exists and is not expired then return the EntryV6* else return nullptr.
********************************************************************************************/
	static EntryV6* findEntry(asio::ip::address_v6&, unsigned short);
/*******************************************************************************************
* @brief Return the maximum privilege the command issuing entry have
*
* @param[in] entry				The Entry in which the change is taking place.
* @param[in] cmdEntry			The entry associated with the command issuing peer.
* @return						The maximum privilage the comEntry have on entry.
*
* @details
* If both IP address are not the same then - return Privilege::LIBERAL_ENTRY;
* Both have the same IPv4 address only - return Privilege::PROTECTED_ENTRY;
* Both have the same IPv4 adddress and port number - return Privilege::RESTRICTED_ENTRY;
********************************************************************************************/
	static Privilege maxPrivilege(EntryV4*, EntryV4*);
/*******************************************************************************************
* @brief Return the maximum privilege the command issuing entry have
*
* @param[in] entry				The Entry in which the change is taking place.
* @param[in] cmdEntry			The entry associated with the command issuing peer.
* @return						The maximum privilage the comEntry have on entry.
*
* @details
* If both IP address are not the same then - return Privilege::LIBERAL_ENTRY;
* Both have the same IPv6 address only - return Privilege::PROTECTED_ENTRY;
* Both have the same IPv6 adddress and port number - return Privilege::RESTRICTED_ENTRY;
********************************************************************************************/
	static Privilege maxPrivilege(EntryV6*, EntryV6*);
};