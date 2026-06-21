#include "rng.h"

void rng_seed(RNG *rng, uint64_t seed)
{
    if (seed == 0) seed = 88172645463393265ull;
    rng->state = seed;
}

uint64_t rng_next_u64(RNG *rng)
{
    uint64_t x = rng->state;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    rng->state = x;
    return x * 2685821657736338717ull;
}

double rng_uniform(RNG *rng)
{
    return (rng_next_u64(rng) >> 11) * (1.0 / 9007199254740992.0);
}
