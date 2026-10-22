/*
 * A343528  Number of ways to write n as x^4 + T(y)^2 + T(z) + 2^w, where
 *          x,y,z are nonnegative integers, w is a positive integer, and T(m)
 *          denotes the triangular number m*(m+1)/2.
 * https://oeis.org/A343528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts representations of n as x^4 + T(y)^2 + T(z) + 2^w
 *            where x,y,z are non‑negative integers, w>0, and T(m) = m*(m+1)/2
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm builds a table of counts up to the largest n that has
   been requested so far.  Each new call recomputes the table if the
   requested n exceeds the current maximum.  The table is indexed from
   0 to max_n, but a(0) is unused (offset is 1). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* table of counts */
static idx_t  cached_max = 0; /* largest index that has been computed */

/* Allocate a new cache of size (max_n+1) */
static void recompute_counts(idx_t max_n)
{
    /* malloc returns void*; casting satisfies both C and C++ */
    seq_t *new_cache = (seq_t *)malloc((size_t)(max_n + 1) * sizeof(seq_t));
    if (!new_cache) return; /* silently give up on allocation failure */

    memset(new_cache, 0, (size_t)(max_n + 1) * sizeof(seq_t));

    seq_t max_n_t = (seq_t)max_n;
    idx_t x = 0;

    for (;; ++x) {
        seq_t x4 = (seq_t)x * x * x * x;
        if (x4 > max_n_t) break;

        idx_t y = 0;
        for (;; ++y) {
            seq_t ty = (seq_t)y * (y + 1) / 2; /* T(y) */
            seq_t ty2 = ty * ty;              /* T(y)^2 */
            seq_t xy = x4 + ty2;
            if (xy > max_n_t) break;

            idx_t z = 0;
            for (;; ++z) {
                seq_t tz = (seq_t)z * (z + 1) / 2; /* T(z) */
                seq_t sum = xy + tz;
                if (sum > max_n_t) break;

                /* w >= 1 → 2^w starts at 2 */
                for (seq_t pw = 2; sum + pw <= max_n_t; pw <<= 1) {
                    idx_t idx = (idx_t)(sum + pw);
                    new_cache[idx] += (seq_t)1;
                }
            }
        }
    }

    /* Replace old cache */
    if (cache) free(cache);
    cache = new_cache;
    cached_max = max_n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset 1, so a(1) is first term */

    if (n > cached_max) recompute_counts(n);

    /* cached_max >= n, so the index exists */
    return cache[(size_t)n];
}
