#include "BaseStation.h"

static struct libwebsocket_protocols antennaBaseStationProtocols[] = {
	/* first protocol must always be HTTP handler */

	{
		"http-only",		/* name */
		antenna::BaseStation::_httpCallback,		/* callback */
		0			/* per_session_data_size */
	},
	{
		"Antenna-BaseStation",
		antenna::BaseStation::_receiverCallback,
		//4,
		512 * 512 * 3 * sizeof(unsigned char),
	},
	{
		NULL, NULL, 0		/* End of list */
	}
};

antenna::BaseStation::BaseStation(void)
{
}

void antenna::BaseStation::start(int port, bool useSSL)
{
	m_port = port;

	const char *sslCertificatePath = NULL;
	const char *sslKeyPath = NULL;

	if(useSSL)
	{
		sslCertificatePath = _getSSLCertificatePath().c_str();
		sslKeyPath = _getSSLKeyPath().c_str();
	}

	unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 1024 + LWS_SEND_BUFFER_POST_PADDING];

	m_context = shared_ptr<libwebsocket_context>(libwebsocket_create_context(
												m_port, NULL, antennaBaseStationProtocols,
												NULL,
												sslCertificatePath, sslKeyPath, -1, -1, 0),
												[] (libwebsocket_context* ptr) {libwebsocket_context_destroy(ptr);});

	if (nullptr == m_context) 
	{
		fprintf(stderr, "libwebsocket init failed\n");
		return;
	}

	buf[LWS_SEND_BUFFER_PRE_PADDING] = 'x';
	
	return;
}

void antenna::BaseStation::stop(void)
{
	//	Null out the context. Shared_ptr will close the context
	m_context.reset();
}

int antenna::BaseStation::yieldTime(void)
{
  return libwebsocket_service(m_context.get(), 1000);
}

void antenna::BaseStation::broadcastData(unsigned char* data, unsigned int length)
{
  libwebsockets_broadcast(&antennaBaseStationProtocols[1], data, length);
}

int antenna::BaseStation::_httpCallback(struct libwebsocket_context *context,
		struct libwebsocket *wsi,
		enum libwebsocket_callback_reasons reason, void *user,
							   void *in, size_t len)
{
	char client_name[128];
	char client_ip[128];

	switch (reason) 
	{
		//	Notifies us when a new client connects.
		case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:

			libwebsockets_get_peer_addresses((int)(long)user, client_name, sizeof(client_name), client_ip, sizeof(client_ip));

			clog << "Received network connect from " << client_name << " (" << client_ip << ")\n";
			// if we returned non-zero from here, we kill the connection 
			break;

		default:
			break;
	}

	return 0;
}

int antenna::BaseStation::_receiverCallback(struct libwebsocket_context *context, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len)
{
    int n;
    unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 512 * 512 * 3 * sizeof(unsigned char) + LWS_SEND_BUFFER_POST_PADDING];
    unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];
    struct per_session_data_receiver *pss = static_cast<per_session_data_receiver*>(user);

	static int close_testing;


    switch (reason) 
    {
        case LWS_CALLBACK_ESTABLISHED:
			cout << "Antenna::Receiver::_receiverCallback - Connection established" << endl;
			pss->number = 0;
            break;

            /*
             * in this protocol, we just use the broadcast action as the chance to
             * send our own connection-specific data and ignore the broadcast info
             * that is available in the 'in' parameter
             */

        case LWS_CALLBACK_BROADCAST:
			memcpy(p, in, len);

            //n = libwebsocket_write(wsi, p, n, LWS_WRITE_TEXT);
			n = libwebsocket_write(wsi, p, len, LWS_WRITE_BINARY);

            if (n < 0) {
                fprintf(stderr, "ERROR writing to socket");
                return 1;
            }
            
            break;

		// Received data. Let any observers know that we have new info from a client
        case LWS_CALLBACK_RECEIVE:
			fprintf(stderr, "rx %d\n", (int)len);
            break;
        
        default:
            break;
    }

    return 0;
}

string antenna::BaseStation::_getSSLCertificatePath(void)
{
	return "C:/UnknownRightNow.cert";
}

string antenna::BaseStation::_getSSLKeyPath(void)
{
	return "C:/UnknownRightNow.key";
}