#include "../../include/helpers/random.h"

std::random_device rd;
std::mt19937 engine(rd());

void initializeRandomEngine() {
    engine.seed(rd());
}
