#include "cmd_interpreter.h"
#include "directory.h"
#include "log.h"

const std::string CmdInterpreter::RESP[] = 
{
	"redudant_data",
	"ok_success",
	"no_privilege",
	"bad_command",
	"bad_param",
	"no_exist",
	"wait_retry",
};

const std::string CmdInterpreter::COMM[] =
{
	"ping",
	"add",
	"search",
	"ttl",
	"charge",
	"update",
	"remove",
	"count",
	"mirror",
	"leave",
	"exit"
};

void CmdInterpreter::processCommand(Peer& peer)
{
	auto commandString = std::string_view{ peer.dataBuffer };
	auto commEnd = commandString.find(' ');
	if (commEnd == std::string::npos)										/* Command is a single word command */	
	{													
		if (commandString == COMM[(short)Command::PING])
			_ping(peer);
		else if (commandString == COMM[(short)Command::EXIT])
			_exit(peer);
		else if (commandString == COMM[(short)Command::ADD])
			_add(peer);
		else if (commandString == COMM[(short)Command::MIRROR])
		{
		}
		else if (commandString == COMM[(short)Command::COUNT])
			_count(peer);
		else if (commandString == COMM[(short)Command::LEAVE])
		{
		}
		else if (commandString == COMM[(short)Command::TTL])
			_ttl(peer);
		else if (commandString == COMM[(short)Command::CHARGE])
			_charge(peer);
		else if (commandString == COMM[(short)Command::SEARCH])
			_search(peer);
	}



	else
	{	
		/* Command is a multi word command */
		peer.writeBuffer = RESP[(short)Response::BAD_COMMAND];
	}
	peer._sendPeerData();
}

inline void CmdInterpreter::_ping(Peer& peer)
{
	if (peer.remoteEp.address().is_v4())
		__ping(peer, peer.peerEntry.Ev4);
	else
		__ping(peer, peer.peerEntry.Ev6);
}

void CmdInterpreter::_add(Peer& peer)
{
	auto response = Directory::addEntry(peer.peerEntry.Ev);
	std::lock_guard<std::mutex> lock(peer.peerEntry.Ev->accessLock);
	peer.writeBuffer += RESP[(short)response] + " ";
	peer.writeBuffer += peer.peerEntry.Ev->UID;
}

void CmdInterpreter::_search(Peer& peer)
{
	if(!peer.peerEntry.Ev->inDirectory())
		peer.writeBuffer += RESP[(short)Response::NO_EXIST] + " ";
}

void CmdInterpreter::_charge(Peer& peer)
{
	peer.writeBuffer += RESP[(short)Response::SUCCESS] + " ";
	peer.writeBuffer += std::to_string((short)TTL::RESTRICTED_TTL);
}

void CmdInterpreter::_ttl(Peer& peer)
{
	peer.writeBuffer += RESP[(short)Response::SUCCESS] + " ";
	peer.writeBuffer += std::to_string((short)TTL::RESTRICTED_TTL);
}

void CmdInterpreter::_count(Peer& peer)
{
	peer.writeBuffer += RESP[(short)Response::SUCCESS] + " ";
	peer.writeBuffer += std::to_string(Directory::getEntryCount());
}

void CmdInterpreter::_exit(Peer& peer)
{
	peer.peerSocket->shutdown(asio::ip::tcp::socket::shutdown_both);
}
