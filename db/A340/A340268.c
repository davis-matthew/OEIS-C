/*
 * A340268  Composite numbers k>1 such that (s-1) | (d-1) for each d | k,
 *          where s = lpf(k) = A020639(k).
 * https://oeis.org/A340268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   _ok_composite
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:
   Return the nth composite integer k>1 such that
   for every prime factor p of k,
     (p-1) is a multiple of (s-1),
   where s=lpf(k) is the smallest prime factor of k.
   Even composites (s=2) and numbers whose smallest prime is 3
   automatically satisfy the property, so they are accepted without
   further testing.  all other composites are checked by trial
   division.  The function stores results in a static cache so it
   works for arbitrary access order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_ok_composite.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;
    static uint64_t next_k = 4; /* smallest composite */

    while (sz < n) {
        if (_ok_composite(next_k)) {
            /* Grow cache if necessary */
            if (sz == cap) {
                size_t newcap = cap == 0 ? 16 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure */
                cache = tmp;
                cap   = newcap;
            }
            cache[sz++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
