/*
 * A268445  Integers n such that A005897(n) is the sum of two nonzero squares.
 * https://oeis.org/A268445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer k such that 6*k^2 + 2 is the sum of two
 *            nonzero squares
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function lazily builds the sequence.  Each call examines
 *  successive integers k, tests the condition 6*k^2+2 = a^2 + b^2 (a,b > 0),
 *  and stores the qualifying k.  The list is cached so repeated calls with
 *  arbitrary indices are fast.  The implementation uses only integer
 *  arithmetic and the provided floor_sqrt utility. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Return true iff m can be expressed as a^2 + b^2 with a,b > 0. */
static bool two_nonzero_squares(seq_t m)
{
    seq_t lim = floor_sqrt(m);
    for (seq_t a = (seq_t)1; a <= lim; ++a) {
        seq_t a2 = a * a;
        seq_t b2 = m - a2;
        if (b2 == (seq_t)0)        /* b must be > 0 */
            continue;
        seq_t b = floor_sqrt(b2);
        if (b * b == b2)
            return true;           /* found representation */
    }
    return false;
}

/* Cached sequence values */
static seq_t *cache = NULL;          /* array of a(k) values */
static idx_t  cached_size = 0;       /* number of terms stored */
static idx_t  cached_cap  = 0;       /* array capacity */
static idx_t  next_candidate = 1;    /* next integer k to test */

/* Grow the cache when needed. */
static void grow_cache(void)
{
    if (cached_size >= cached_cap) {
        cached_cap = (cached_cap == 0 ? 8 : cached_cap << 1);
        cache = (seq_t *)realloc(cache, cached_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    /* The sequence starts at offset 1: a(1) = 1. */
    while (cached_size < n) {
        seq_t cur_val = (seq_t)next_candidate;      /* candidate k */
        seq_t m = (seq_t)6 * cur_val * cur_val + (seq_t)2;  /* 6*k^2+2 */

        if (two_nonzero_squares(m)) {
            grow_cache();
            cache[cached_size++] = cur_val;
            next_candidate = next_candidate + (idx_t)1;
        } else {
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
