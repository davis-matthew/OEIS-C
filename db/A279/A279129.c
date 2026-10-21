/*
 * A279129  Number of nX3 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and vertical neighbors, with the
 *          exception of exactly two elements, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 47, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279129
   Formula (empirical recurrence, valid for n > 17):
   a(n) = 6*a(n-1) -11*a(n-2) +5*a(n-3) +a(n-5) +9*a(n-6)
          -6*a(n-7) +3*a(n-8) -5*a(n-9) -a(n-11)
   Initial values:
   a(1)=0, a(2)=9, a(3)=66, a(4)=256, a(5)=820,
   a(6)=2551, a(7)=7491, a(8)=21131, a(9)=57971,
   a(10)=155551, a(11)=409876, a(12)=1064002,
   a(13)=2727560, a(14)=6917391, a(15)=17381168,
   a(16)=43320628, a(17)=107203164 */
#include <stddef.h>
#include <stdbool.h>

#define MAX_CACHE 10001  /* sufficient for n up to about 10000 */

/* constants for the first 17 terms (index 0 unused) */
static const seq_t init[18] = {
    0,            /* dummy for index 0 */
    0,            /* a(1) */
    9,            /* a(2) */
    66,           /* a(3) */
    256,          /* a(4) */
    820,          /* a(5) */
    2551,         /* a(6) */
    7491,         /* a(7) */
    21131,        /* a(8) */
    57971,        /* a(9) */
    155551,       /* a(10) */
    409876,       /* a(11) */
    1064002,      /* a(12) */
    2727560,      /* a(13) */
    6917391,      /* a(14) */
    17381168,     /* a(15) */
    43320628,     /* a(16) */
    107203164     /* a(17) */
};

/* cache for computed terms */
static seq_t cache[MAX_CACHE + 1];
static bool cache_initialised = false;
static size_t cache_highest = 17;

/* compute a(n) for any n >= 1 */
seq_t a(idx_t n)
{
    /* initialise cache the first time */
    if (!cache_initialised) {
        for (size_t i = 0; i <= 17; ++i)
            cache[i] = init[i];
        cache_initialised = true;
    }

    /* return one of the initial values directly */
    if (n <= 17)
        return init[(size_t)n];

    /* if the requested index is beyond the static cache size,
       compute the term on the fly using a circular buffer of length 12
       without allocating a large array (safe for arbitrary-precision types). */
    if (n > MAX_CACHE) {
        seq_t ring[12];
        /* initialise ring with the first 17 terms */
        for (size_t i = 0; i < 12; ++i)
            ring[i] = 0;
        for (size_t i = 1; i <= 17; ++i)
            ring[i % 12] = init[i];

        for (size_t i = 18; i <= (size_t)n; ++i) {
            seq_t next =
                6 * ring[(i-1) % 12]
              - 11 * ring[(i-2) % 12]
              + 5  * ring[(i-3) % 12]
              + ring[(i-5) % 12]
              + 9  * ring[(i-6) % 12]
              - 6  * ring[(i-7) % 12]
              + 3  * ring[(i-8) % 12]
              - 5  * ring[(i-9) % 12]
              - ring[(i-11) % 12];
            ring[i % 12] = next;
        }
        return ring[n % 12];
    }

    /* compute and extend the static cache up to n if needed */
    if (cache_highest < (size_t)n) {
        for (size_t i = cache_highest + 1; i <= (size_t)n; ++i) {
            cache[i] =
                6 * cache[i-1]
              - 11 * cache[i-2]
              + 5  * cache[i-3]
              + cache[i-5]
              + 9  * cache[i-6]
              - 6  * cache[i-7]
              + 3  * cache[i-8]
              - 5  * cache[i-9]
              - cache[i-11];
        }
        cache_highest = (size_t)n;
    }

    return cache[(size_t)n];
}
