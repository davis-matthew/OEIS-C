/*
 * A248186  Least k such that 1/18 - Sum_{h=1..k} 1/(h*(h+1)*(h+2)*(h+3)) <
 *          1/n^3.
 * https://oeis.org/A248186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimal k ≥ 1 such that 3·(k+1)(k+2)(k+3) > n^3 (i.e.
 *            1/18 - Σ_{h=1..k} 1/(h(h+1)(h+2)(h+3)) < 1/n^3)
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 1452, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   cond
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/cond.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1) */
    seq_t sn = (seq_t)n;
    seq_t rhs = sn * sn * sn;          /* n^3 */

    idx_t low  = 1;
    idx_t high = 1;

    /* find an upper bound */
    while (!cond(high, rhs))
        high <<= 1;                    /* double until condition holds */

    /* binary search on [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cond(mid, rhs))
            high = mid;
        else
            low = mid + 1;
    }

    return (seq_t)low;
}
