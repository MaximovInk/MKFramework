#pragma once

#define MK_ENGINE_EXPORTS

#ifdef MK_ENGINE_EXPORTS
#define MK_ENGINE_API __declspec(dllexport)
#else
#define MK_ENGINE_API __declspec(dllimport)
#endif

#include "spdlog/spdlog.h"

#define LOG spdlog

namespace MKEngine {
	MK_ENGINE_API void utilsExample1();
}