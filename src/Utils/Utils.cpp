#include "Utils.hpp"
#include <cmath>
#include <algorithm>
#include <cctype>

bool IsEqual(float x, float y)
{
    return fabsf(x - y) < EPSILON;    
}
bool IsGreaterThanOrEqual(float x, float y) 
{
    return x > y || IsEqual(x, y);
}
bool IsLessThanOrEqual(float x, float y) 
{
    return x < y || IsEqual(x, y);
}
float MillisecondsToSeconds(unsigned int milliseconds)
{
	return static_cast<float>(milliseconds) / 1000.0f;
}
