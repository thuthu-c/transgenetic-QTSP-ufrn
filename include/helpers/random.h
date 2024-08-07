#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <random>

extern std::random_device rd;
extern std::mt19937 engine;

void initializeRandomEngine();

#endif
