#include "ReceiverTest.h"

int main(int argc, char** argv)
{
	antenna::Receiver testReceiver;
	testReceiver.start(7681);

    return 0;
}
