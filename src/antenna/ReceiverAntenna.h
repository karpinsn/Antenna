/**
  * @file
  * @author Nikolaus Karpinsky
  * @date   07/08/2011
  *
  * Class used to receive Websocket packets across a network.
  */

#ifndef _ANTENNA_RECEIVER_ANTENNA_H_
#define _ANTENNA_RECEIVER_ANTENNA_H_

#include <libwebsockets.h>

#include <string>
#include <memory>

using namespace std;

namespace antenna
{
class ReceiverAntenna
{
private:
	int m_port;
	shared_ptr<libwebsocket_context> m_serviceContext;
	shared_ptr<struct libwebsocket> m_antennaSocket;

public:
  ReceiverAntenna(void) { };
  void start(const char* address, int port, bool useSSL = false);
  void stop(void);
  int yieldTime(void);
  
  static int _receiverCallback(struct libwebsocket_context *context, 
	struct libwebsocket *wsi, 
	enum libwebsocket_callback_reasons reason, 
	void *user, void *in, size_t len);

private:
	string _getSSLCertificatePath(void);
	string _getSSLKeyPath(void);
};
}

#endif //_ANTENNA_RECEIVER_ANTENNA_H_
