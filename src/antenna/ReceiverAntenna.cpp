#include "ReceiverAntenna.h"

static struct libwebsocket_protocols antennaReceiverAntennaProtocols[] = {
	{
		"Antenna",
		antenna::ReceiverAntenna::_receiverCallback,
		0,
	},
	{
		NULL, NULL, 0		/* End of list */
	}
};

void antenna::ReceiverAntenna::start(const char* address, int port, bool useSSL)
{
	//	This context is needed by libwebsockets to service the socket connection.
	m_serviceContext = shared_ptr<libwebsocket_context>(libwebsocket_create_context(CONTEXT_PORT_NO_LISTEN,
		NULL, antennaReceiverAntennaProtocols, libwebsocket_internal_extensions, NULL, NULL, -1, -1, 0, NULL),
		[] (libwebsocket_context* ptr) {libwebsocket_context_destroy(ptr);});

	if (nullptr == m_serviceContext) 
	{
		fprintf(stderr, "libwebsocket init failed\n");
		return;
	}

	//	Now create a client side Antenna websocket context
	m_antennaSocket = shared_ptr<struct libwebsocket>(
		libwebsocket_client_connect(m_serviceContext.get(), address, 
		port, useSSL, "/", "host", "origin", antennaReceiverAntennaProtocols[0].name, -1));
}

void antenna::ReceiverAntenna::stop(void)
{
	//	Null out the context. Shared_ptr will close the context
	m_serviceContext.reset();
}

int antenna::ReceiverAntenna::yieldTime(void)
{
	if(nullptr != m_serviceContext)
		return libwebsocket_service(m_serviceContext.get(), 1000);
	else
		return -1;
}

int antenna::ReceiverAntenna::_receiverCallback(struct libwebsocket_context *context, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len)
{
	switch(reason)
	{
	case LWS_CALLBACK_CLOSED:
		//TODO: Need to close the connection
		break;
	case LWS_CALLBACK_CLIENT_RECEIVE:
		//TODO: Receive the data
		break;
	default:
		break;
	}

	return 0;
}

string antenna::ReceiverAntenna::_getSSLCertificatePath(void)
{
	return "C:/UnknownRightNow.cert";
}

string antenna::ReceiverAntenna::_getSSLKeyPath(void)
{
	return "C:/UnknownRightNow.key";
}