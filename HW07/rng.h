#ifndef RNG_H
#define RNG_H

#include <stdint.h>

typedef struct {
    uint64_t state;
} RNG;

void rng_seed(RNG *rng, uint64_t seed);
uint64_t rng_next_u64(RNG *rng);
double rng_uniform(RNG *rng);

#endif
