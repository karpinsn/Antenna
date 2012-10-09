/**
  * @file
  * @author Nikolaus Karpinsky
  * @date   07/08/2011
  *
  * Class used to broacast information to many websocket connections.
  */

#ifndef _ANTENNA_BASE_STATION_H_
#define _ANTENNA_BASE_STATION_H_

#include <libwebsockets.h>

#include <iostream>
#include <string>
#include <memory>

using namespace std;

namespace antenna
{
class BaseStation
{
private:
	int m_port;
	
	struct per_session_data_receiver { int number; };
	shared_ptr<libwebsocket_context> m_context;


public:
	BaseStation(void);
	void start(int port, bool useSSL = false);
	void stop(void);
	void broadcastData(unsigned char* data, unsigned int length);
	int yieldTime(void);

	static int _httpCallback(struct libwebsocket_context *context,
	struct libwebsocket *wsi,
	enum libwebsocket_callback_reasons reason, 
	void *user, void *in, size_t len);

	static int _receiverCallback(struct libwebsocket_context *context, 
	struct libwebsocket *wsi, 
	enum libwebsocket_callback_reasons reason, 
	void *user, void *in, size_t len);

private:
	string _getSSLCertificatePath(void);
	string _getSSLKeyPath(void);
};
}

#endif //_ANTENNA_BASE_STATION_H_
