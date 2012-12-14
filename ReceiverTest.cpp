#include "ReceiverTest.h"

int main(int argc, char** argv)
{
	antenna::BaseStation testReceiver;
	testReceiver.start(7681);

	//CvCapture* mov = cvCaptureFromAVI( "D:/Data/Jane.avi" );

	//int p[] = {CV_IMWRITE_JPEG_QUALITY, 100, 0};

	int frameNumber = 0;

	while(testReceiver.yieldTime() >= 0)
	{
		/*
		IplImage* img = cvQueryFrame(mov);
		frameNumber++;
		if(!img || frameNumber > 600)
		{
			frameNumber = 0;
			cvReleaseCapture(&mov);
			mov = cvCaptureFromAVI( "D:/Data/Jane.avi" );
			continue;
		}

		CvMat* buf = cvEncodeImage(".jpg", img, p);
		testReceiver.broadcastData(buf->data.ptr, buf->width);
		cvReleaseMat(&buf);
		*/


		usleep(1000/30);
	}

    return 0;
}
