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

namespace antenna
{
	class Receiver
	{
		public:
			Receiver(void);
			init(void);
		
	};
}

#endif //_ANTENNA_RECEIVER_H_
