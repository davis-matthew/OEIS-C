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
 * Validated: standard, pass, 70 terms through n = 8935 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375397: the nth number having at least two distinct prime factors
 * and possessing a prime factor other than the smallest whose exponent is ≥ 2. */
/*  The first term is a(1) = 18. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
