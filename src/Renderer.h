#pragma once

#include <glad/glad.h>
#include "Log.h"

#ifdef EON_DEBUG
	#define ASSERT(x, ...) { if(!(x)) { EON_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else // EON_DEBUG
	#define ASSERT(x, ...)
#endif