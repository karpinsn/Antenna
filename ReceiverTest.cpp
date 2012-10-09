#include "ReceiverTest.h"

int main(int argc, char** argv)
{
	antenna::BaseStation testReceiver;
	testReceiver.start(7681);

	IplImage* img = cvLoadImage( "N:/Data/0000.png" );

	int p[] = {CV_IMWRITE_JPEG_QUALITY, 100, 0};
	CvMat* buf = cvEncodeImage(".jpg", img, p);
	//CvMat* buf = cvEncodeImage(".png", img, 0);

	while(testReceiver.yieldTime() >= 0)
	{
	  testReceiver.broadcastData(buf->data.ptr, buf->width);
	  usleep(1000/30);
	}

    return 0;
}
