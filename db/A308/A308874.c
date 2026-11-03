/*
 * A308874  Composite numbers that are neither squares nor oblongs.
 * https://oeis.org/A308874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   ipow_i32, is_prime
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

#include "../utils/ipow_i32.h"
#include "../utils/is_prime.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
