/**
  * @file
  * @author Nikolaus Karpinsky
  * @date   07/08/2011
  *
  * Class used to receive TCP packets from a network. Allows a program to
  * register an observer which will then be called with complete data packets
  */

#ifndef _ANTENNA_RECEIVER_H_
#define _ANTENNA_RECEIVER_H_

#include <libwebsockets.h>

#include <iostream>
#include <string>
#include <memory>

using namespace std;

namespace antenna
{
class Receiver
{
private:
	int m_port;
	
	struct libwebsocket_protocols m_protocols[3];
	struct per_session_data_receiver { int number; };
	shared_ptr<libwebsocket_context> m_context;


public:
	Receiver(void);
	void start(int port, bool useSSL = false);
	void stop(void);

private:
	static int _httpCallback(struct libwebsocket_context *context,
	struct libwebsocket *wsi,
	enum libwebsocket_callback_reasons reason, 
	void *user, void *in, size_t len);

	static int _receiverCallback(struct libwebsocket_context *context, 
	struct libwebsocket *wsi, 
	enum libwebsocket_callback_reasons reason, 
	void *user, void *in, size_t len);

	string _getSSLCertificatePath(void);
	string _getSSLKeyPath(void);
};
}

#endif //_ANTENNA_RECEIVER_H_
