#include "stdafx.h"
#include "view/NumberFormat.h"

std::string formatNumber(double value)
{
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(0) << std::floor(value);
	return ss.str();
}
