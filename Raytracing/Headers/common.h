#pragma once
#include <random>
#include <time.h>

float getRundomFloat();

float getRundomFloat()
{
	return (((double)rand() / (RAND_MAX)));
}