#include "Receiver.h"

antenna::Receiver::Receiver(void)
{
	/* first protocol must always be HTTP handler */
	m_protocols[0].name = "http-only";
	m_protocols[0].callback = antenna::Receiver::_httpCallback;
	m_protocols[0].per_session_data_size = 0;

	size_t size = sizeof(struct per_session_data_receiver);

	m_protocols[1].name = "Antenna";
	m_protocols[1].callback = antenna::Receiver::_receiverCallback;
	m_protocols[1].per_session_data_size = size;

	m_protocols[2].name = NULL;
	m_protocols[2].callback = NULL;
	m_protocols[2].per_session_data_size = 0;
}

void antenna::Receiver::start(int port, bool useSSL)
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

	/* Cant link in libwebsocket_internal_extensions
	context = libwebsocket_create_context(m_port, interface, m_protocols,
				libwebsocket_internal_extensions,
				cert_path, key_path, -1, -1, opts);
				*/

	m_context = shared_ptr<libwebsocket_context>(libwebsocket_create_context(
												m_port, NULL, m_protocols,
												NULL,
												sslCertificatePath, sslKeyPath, -1, -1, 0),
												[] (libwebsocket_context* ptr) {libwebsocket_context_destroy(ptr);});

	if (nullptr == m_context) 
	{
		fprintf(stderr, "libwebsocket init failed\n");
		return;
	}

	buf[LWS_SEND_BUFFER_PRE_PADDING] = 'x';
	
	/*
		* This example server does not fork or create a thread for
		* websocket service, it all runs in this single loop.  So,
		* we have to give the websockets an opportunity to service
		* "manually".
		*
		* If no socket is needing service, the call below returns
		* immediately and quickly.  Negative return means we are
		* in process of closing
		*/
	while (libwebsocket_service(m_context.get(), 1000) >= 0) {
		
		/*
		 * This broadcasts to all dumb-increment-protocol connections
		 * at 20Hz.
		 *
		 * We're just sending a character 'x', in these examples the
		 * callbacks send their own per-connection content.
		 *
		 * You have to send something with nonzero length to get the
		 * callback actions delivered.
		 *
		 * We take care of pre-and-post padding allocation.
		 */
		/*
		if (((unsigned int)tv.tv_usec - oldus) > 50000) {
			libwebsockets_broadcast(
					&m_protocols[1],
					&buf[LWS_SEND_BUFFER_PRE_PADDING], 1);
			oldus = tv.tv_usec;
		}*/

		buf[LWS_SEND_BUFFER_PRE_PADDING] = 'a';
		int retVal = libwebsockets_broadcast(&m_protocols[1], &buf[LWS_SEND_BUFFER_PRE_PADDING], 1);
		usleep(500);
	}

	return;
}

void antenna::Receiver::stop(void)
{
	//	Null out the context. Shared_ptr will close the context
	m_context.reset();
}

int antenna::Receiver::_httpCallback(struct libwebsocket_context *context,
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

int antenna::Receiver::_receiverCallback(struct libwebsocket_context *context, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len)
{
    int n;
    unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 512 + LWS_SEND_BUFFER_POST_PADDING];
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
            n = sprintf((char *)p, "%d", pss->number++);
            n = libwebsocket_write(wsi, p, n, LWS_WRITE_TEXT);

            if (n < 0) {
                fprintf(stderr, "ERROR writing to socket");
                return 1;
            }
            
            break;

		// Received data. Let any observers know that we have new info from a client
        case LWS_CALLBACK_RECEIVE:
			//strcmp(reinterpret_cast<char*>(in), "reset\n"
            break;
        
        default:
            break;
    }

    return 0;
}

string antenna::Receiver::_getSSLCertificatePath(void)
{
	return "C:/UnknownRightNow.cert";
}

string antenna::Receiver::_getSSLKeyPath(void)
{
	return "C:/UnknownRightNow.key";
}