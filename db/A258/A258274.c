/*
 * A258274  The smallest number of cents which cannot be made with fewer than
 *          n American coins.
 * https://oeis.org/A258274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 662, u32 n <= 9473, u64 n <= 9473,
 *            u128 n <= 9473, big any n
 * Helpers:   min_coins
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A258274 – Smallest amount that requires at least n coins.
 *  Formula (when n ≥ 8):  a(n) = 100*n – 706
 *  For 1 ≤ n < 8 the value is found by a short exhaustive search.
 */

#include <stdint.h>
#include "../utils/min_coins.h"

seq_t a(idx_t n)
{
    if (n >= 8)
        return 100 * n - 706;

    seq_t val = 0;
    while (1) {
        ++val;
        if (min_coins(val) >= n)
            return val;
    }
}
