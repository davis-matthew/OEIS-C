/*
 * A308874  Composite numbers that are neither squares nor oblongs.
 * https://oeis.org/A308874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 2934, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A308874: composite numbers that are neither squares nor oblongs.
   The code performs a simple incremental search: it tests successive
   integers, skips primes, squares and oblongs, and caches the qualifying
   values so that any call to a(n) can be answered in O(n) time. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */
#include "../utils/is_prime.h"     /* bool is_prime(idx_t) */

/* Test whether x is a perfect square. */
static bool is_square(seq_t x)
{
    seq_t s = floor_sqrt(x);
    return (s * s) == x;
}

/* Test whether x is an oblong (pronic) number, i.e. x = k*(k+1). */
static bool is_oblong(seq_t x)
{
    /* t = 1 + 4*x */
    seq_t t = (seq_t)1 + (seq_t)4 * x;
    seq_t s = floor_sqrt(t);

    if ((s * s) != t)          /* 1+4*x must be a perfect square */
        return false;

    /* sqrt(1+4x) must be odd for k to be integer */
    if (((uint64_t)s & 1ULL) == 0)
        return false;

    seq_t k = (s - (seq_t)1) / (seq_t)2;
    return (k * (k + (seq_t)1)) == x;
}

/* --------------------------------------------------------------------- */
/* Cache for precomputed terms.  The array size is sufficient for n up
   to about 10 000 (see comments).  It is statically allocated so that
   the code is valid C11 as well as C++17. */

static seq_t cache[40000];      /* enough for n ≤ 10 000 */
static size_t cache_size = 0;   /* number of cached terms */
static idx_t next_candidate = 2;  /* next integer to test for inclusion */

/* --------------------------------------------------------------------- */
/* Main sequence function ------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; return a dummy for 0 */
        return 0;

    /* Generate terms until the cache contains at least n elements */
    while (cache_size < (size_t)n) {
        /* Test whether next_candidate qualifies */
        if (!is_prime(next_candidate) &&          /* composite */
            !is_square((seq_t)next_candidate) &&  /* not a square */
            !is_oblong((seq_t)next_candidate))    /* not oblong */
        {
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    /* Return the n-th term (1‑based index). */
    return cache[cache_size - 1];
}
