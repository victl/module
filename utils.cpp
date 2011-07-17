#include "utils.h"

namespace module
{
namespace utils
{
IplImage* ConvertUtils::Convert(const MetaCameraBW_t& data)
{
	IplImage* img = cvCreateImage(cvSize(BWCAM_IMAGE_WIDTH, BWCAM_IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
	memcpy(img->imageData, data.mdata, img->imageSize);
	return img;
}

IplImage* ConvertUtils::Convert(const MetaCameraC_t& data)
{
	IplImage* img = cvCreateImage(cvSize(CCAM_IMAGE_WIDTH, CCAM_IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
	memcpy(img->imageData, data.mdata, img->imageSize);
	return img;
}

//IplImage* ConvertUtils::Convert(const MarkerTaillightImage_t& data)
//{
//	IplImage* img = cvCreateImage(cvSize(data.width, data.height), IPL_DEPTH_8U, 3);
//	memcpy(img->imageData, data.imgdata, img->imageSize);
//	return img;
//}
}
}
