/*
 * A395985  Smallest base such that n is a repunit in that base.
 * https://oeis.org/A395985
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compare_repunit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest base b>1 such that n is a repunit in base b.
   Computes a(n) for n >= 3.  The algorithm tests all possible
   lengths L (number of 1‑digits) from 64 down to 3 and performs a
   binary search for the base.  For L = 2 the solution is always
   b = n‑1.  The function is O(log n * log n) per call. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compare_repunit_sum.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;               /* offset not used */

    seq_t target     = (seq_t)n;
    seq_t best_base  = target - 1;      /* base n-1 works for L = 2 */

    /* Search for shorter lengths (larger bases) first:
       L decreases from 64 to 3.  If a smaller base is found, it
       must be the answer, so we can break early. */
    for (idx_t L = 64; L >= 3; --L) {
        seq_t low  = (seq_t)2;
        seq_t high = target - 1;

        while (low <= high) {
            seq_t mid = (low + high) / 2;
            int cmp = compare_repunit_sum(mid, L, target);

            if (cmp == 0) {
                best_base = mid;          /* found a repunit */
                break;
            } else if (cmp < 0) {
                low = mid + 1;             /* sum too small */
            } else {
                high = mid - 1;            /* sum too large */
            }
        }
        if (best_base == (seq_t)2)          /* minimal possible base */
            break;
    }

    return best_base;
}
