#ifndef LOG_H_
#define LOG_H_

#ifndef _INC_STDIO
#include <stdio.h>
#endif

#define LOG_INFO(...)                                                          \
	printf("[INFO(%s:%d)] ", __FILE__, __LINE__), printf(__VA_ARGS__),         \
		printf("\n")
#define LOG_WARN(...)                                                          \
	printf("[WARN(%s:%d)] ", __FILE__, __LINE__), printf(__VA_ARGS__),         \
		printf("\n")
#define LOG_ERROR(...)                                                         \
	printf("[ERROR(%s:%d)] ", __FILE__, __LINE__), printf(__VA_ARGS__),        \
		printf("\n")

#endif // LOG_H_
