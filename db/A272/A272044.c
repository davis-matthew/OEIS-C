/*
 * A272044  Numbers n such that n and n^2+1 have the same number of prime
 *          factors (including multiplicities).
 * https://oeis.org/A272044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A272044: numbers n such that bigomega(n)=bigomega(n^2+1) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t sz  = 0;
    static idx_t checked_upto = 1;   /* last value examined */

    while (sz < n) {
        idx_t m = checked_upto + 1;
        checked_upto = m;

        seq_t omega1 = bigomega(m);
        idx_t sq = (idx_t)m * m + 1;
        seq_t omega2 = bigomega(sq);

        if (omega1 == omega2) {
            if (sz + 1 > cap)
                ensure_capacity(sz + 1, &cap, (void **)&cache, sizeof(seq_t));
            cache[sz++] = (seq_t)m;
        }
    }
    return cache[n-1];
}
