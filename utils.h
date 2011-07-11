#pragma once

#include <opencv/cv.h>
#include "types.h"

namespace module
{
namespace utils
{

class ConvertUtils
{
public:
	inline static IplImage* Convert(const MetaCameraBW_t& data);
	inline static IplImage* Convert(const MetaCameraC_t& data);
	inline static IplImage* Convert(const MarkerTaillightImage_t& data);
};
}
}
