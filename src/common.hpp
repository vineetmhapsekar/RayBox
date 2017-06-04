// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#ifndef COMMON_HPP
#define COMMON_HPP

#include <memory>


#ifdef PERFORMANCE
	#include <chrono>
	#define TIMER_START(TAG)			std::chrono::high_resolution_clock::time_point time_start_##TAG = std::chrono::high_resolution_clock::now();
	#define TIMER_STOP(TAG)				std::chrono::high_resolution_clock::time_point time_stop_##TAG = std::chrono::high_resolution_clock::now(); \
										std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(time_stop_##TAG - time_start_##TAG); \
										std::cout << #TAG" Time Taken :" << std::chrono::duration_cast<std::chrono::microseconds>(time_span).count() << " microseconds." << std::endl;
#else
#define TIMER_START(TAG)
#define TIMER_STOP(TAG)
#endif

#endif //COMMON_HPP
// TODO: reference additional headers your program requires here
