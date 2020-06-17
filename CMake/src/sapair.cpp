#include "sapair.h"
#include "common.h"

SApair::SApair(asio::ip::tcp::socket* tcpSocket)
{
	auto remoteEp = tcpSocket->remote_endpoint();
	_ipAddr = remoteEp.address();
	_portNumber = remoteEp.port();

	if (_ipAddr.is_v4())
	{
		_saPairStr += STR_V4;
		auto _ipAddr4 = _ipAddr.to_v4();
		_saPairStr += " " + _ipAddr4.to_string();
	}
	else
	{
		_saPairStr += STR_V6;
		auto _ipAddr6 = _ipAddr.to_v6();
		if (_ipAddr6.is_v4_mapped())
			_saPairStr += " " + _ipAddr6.to_v4().to_string();
		else
			_saPairStr += " " + _ipAddr6.to_string();
	}
	_saPairStr += " " + std::to_string(_portNumber);
}

std::string SApair::toString() const
{
	return _saPairStr;
}

bool SApair::operator==(const SApair& saPair) const
{
	if (_ipAddr == saPair._ipAddr && _portNumber == saPair._portNumber)
		return true;
	else
		return false;
}
