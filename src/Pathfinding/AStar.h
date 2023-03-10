#ifndef SCORPION_ASTAR_H
#define SCORPION_ASTAR_H

#include <stddef.h>
#include "Position.h"
#include <stdint.h>

Position** find_path(Position *start, Position *goal, WorldState *state, uint32_t *counter);

#endif //SCORPION_ASTAR_H