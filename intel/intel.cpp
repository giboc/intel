#include "opencv\cv.h"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include "util_render.h"
#include "util_pipeline.h"

int main()
{
	UtilPipeline pipeline;
	pipeline.EnableImage(PXCImage::COLOR_FORMAT_RGB24,640,480);
	
	pipeline.Init();
	UtilRender color_render(L"Color Stream");
	

	///////////// OPENCV
	IplImage *image = 0;
	CvSize gab_size;
	gab_size.height = 480;
	gab_size.width = 640;
	image = cvCreateImage(gab_size,8,3);

	PXCImage::ImageData data;

	unsigned char *rgb_data;//=new unsigned char[];
	//rgb_data=(unsigned char*)image->imageData;
	PXCImage::ImageInfo rgb_info;

	for (;;) {
		if (!pipeline.AcquireFrame(true)) break;
		PXCImage *color_image=pipeline.QueryImage(PXCImage::IMAGE_TYPE_COLOR);

		color_image->AcquireAccess(PXCImage::ACCESS_READ_WRITE,PXCImage::COLOR_FORMAT_RGB24,&data); 
		rgb_data = data.planes[0];
		memcpy(image->imageData, rgb_data, 640*480*3);
		color_image->ReleaseAccess(&data);
		cv::Mat hsv_img(image);
		cv::cvtColor(hsv_img,hsv_img,CV_BGR2HSV);
		cv::inRange(hsv_img,cv::Scalar(0,0,0),cv::Scalar(20,255,255),hsv_img);
		cv::imshow("HSV",hsv_img);
		


		if( cvWaitKey(10) >= 0 )
			break;
		pipeline.ReleaseFrame();
	}
	cvReleaseImage(&image);
	pipeline.Close();
	return 0;
}