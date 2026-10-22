/*
 * A387225  Values of v in the quartets (2, u, v, w) of type 3; i.e., values
 *          of v for solutions to 2(2 - u) = v(v - w), in distinct positive
 *          integers, with v > 1, sorted by nondecreasing values of u; see
 *          A386631.
 * https://oeis.org/A387225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the v‑value in every integer solution (2,u,v,w) of *
 *            2(2–u) = v(v–w) with 2, u, v, w positive, distinct, v>1, *
 *            ordered first by nondecreasing u and, for a fixed u, by
 *            ascending v
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"

/* Global state for memoisation */
static idx_t _next_u = 1;      /* next u to process when extending the cache */
static seq_t * _cache = NULL;  /* array of found v-values */
static size_t _cache_len = 0;  /* number of values stored */
static size_t _cache_cap = 0;  /* allocated capacity of _cache */

/* Append a value to the global cache, growing the array as needed */
static inline void append_to_cache(seq_t val)
{
    if (_cache_len == _cache_cap)
        ensure_capacity(_cache_len + 1, &_cache_cap, (void **)&_cache, sizeof(seq_t));
    _cache[_cache_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0 */

    /* Grow the cache until we have at least n values */
    while (_cache_len < (size_t)n) {
        idx_t u = _next_u++;
        if (u == 2) continue;          /* u=2 gives R=0 which cannot yield distinct values */

        __int128 R = (__int128)4 - (__int128)2 * (__int128)u;
        if (R == 0) continue;          /* safety, should never happen after u==2 */

        /* collect all valid v for this u */
        seq_t local[4096];
        size_t localcnt = 0;

        /* iterate over positive divisors of |R| */
        idx_t absR = (idx_t)((R >= 0) ? R : -R);
        for (idx_t d = 1; d <= absR / d; ++d) {
            if (absR % d != 0) continue;
            idx_t d1 = d;
            idx_t d2 = absR / d;

            /* try divisor d1 as a candidate v */
            if (d1 > 1 && d1 != 2 && d1 != u) {
                __int128 div = R / (__int128)d1;
                __int128 w128 = (__int128)d1 - div;
                if (w128 > 0) {
                    seq_t w = (seq_t)w128;
                    if (w != 2 && w != (seq_t)u)
                        local[localcnt++] = (seq_t)d1;
                }
            }

            /* try divisor d2 if distinct from d1 */
            if (d2 != d1 && d2 > 1 && d2 != 2 && d2 != u) {
                __int128 div = R / (__int128)d2;
                __int128 w128 = (__int128)d2 - div;
                if (w128 > 0) {
                    seq_t w = (seq_t)w128;
                    if (w != 2 && w != (seq_t)u)
                        local[localcnt++] = (seq_t)d2;
                }
            }
        }

        /* sort the local list and remove duplicates */
        for (size_t i = 1; i < localcnt; ++i) {
            seq_t key = local[i];
            size_t j = i;
            while (j > 0 && local[j - 1] > key) {
                local[j] = local[j - 1];
                --j;
            }
            local[j] = key;
        }
        size_t uniq = 0;
        for (size_t i = 0; i < localcnt; ++i) {
            if (i == 0 || local[i] != local[i - 1])
                local[uniq++] = local[i];
        }

        /* append the unique sorted values to the global cache */
        for (size_t i = 0; i < uniq; ++i)
            append_to_cache(local[i]);
    }

    return _cache[(size_t)n - 1];
}
