/*
 * A243155  Larger of two consecutive primes whose positive difference is a
 *          cube.
 * https://oeis.org/A243155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cube_root_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for successive primes, records the larger of two consecutive
   primes whose difference is a perfect cube.  The function returns the
   nth such prime (a(1)=3). */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */
#include "../utils/floor_cube_root_u64.h"

static uint64_t next_prime_u64(uint64_t start)
{
    for (uint64_t cand = start; ; ++cand) {
        if (is_prime((idx_t)cand))
            return cand;
    }
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        abort();                /* sequence defined for n >= 1 */

    static uint64_t prev_prime = 2;   /* last prime processed */

    while (cache_len < (size_t)n) {
        uint64_t p = next_prime_u64(prev_prime + 1);
        uint64_t gap = p - prev_prime;
        uint64_t r = floor_cube_root_u64(gap);
        if ((__int128)r * r * r == gap) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }
        prev_prime = p;
    }
    return cache[n - 1];
}
