#pragma once
#include <iostream>

#ifndef ENABLE_DEBUG_LOGS
#define ENABLE_DEBUG_LOGS 0 // by default, debug logs are disabled
#endif

#if ENABLE_DEBUG_LOGS       // enable debug logs if defined
#define DEBUG_LOG(message) do { std::cout << message << std::endl; } while (0) // debug log macro
#else
#define DEBUG_LOG(message) do {} while (0)  // if debug logs are disabled do nothing
#endif