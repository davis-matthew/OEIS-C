/*
 * A375397  Numbers divisible by the square of some prime factor other than
 *          the least. Non-hooklike numbers.
 * https://oeis.org/A375397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 7535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375397: the nth number having at least two distinct prime factors
 * and possessing a prime factor other than the smallest whose exponent is ≥ 2. */
/*  The first term is a(1) = 18. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Check whether n satisfies the definition. */
static bool good(uint64_t n)
{
    if (n < 18)                 /* numbers below 18 cannot work */
        return false;

    uint64_t smallest = 0;          /* first prime factor found */
    int distinct = 0;               /* number of distinct primes   */
    bool has_square_of_non_least = false;

    uint64_t x = n;
    for (uint64_t p = 2; p <= x / p; ++p) {
        if (x % p == 0) {
            if (smallest == 0)
                smallest = p;
            ++distinct;
            int exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            if (p != smallest && exp >= 2)
                has_square_of_non_least = true;
        }
    }
    if (x > 1) {   /* remaining prime factor */
        if (smallest == 0)
            smallest = x;
        else
            ++distinct;
        /* exponent of this remaining prime is 1, so it cannot contribute a square */
    }

    return distinct >= 2 && has_square_of_non_least;
}

/* The main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached sequence values   */
    static size_t cache_len = 0;     /* number of cached terms   */
    static size_t cache_cap = 0;     /* allocated capacity       */
    static uint64_t next_to_check = 2;   /* next integer to test    */

    if (n == 0)
        return (seq_t)0;             /* undefined for n=0       */

    /* Ensure we have at least n terms cached. */
    while (cache_len < n) {
        if (good(next_to_check)) {
            /* Grow the cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_to_check;
        }
        ++next_to_check;
    }

    return cache[n - 1];
}
