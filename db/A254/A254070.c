/*
 * A254070  a(n) = -1 + (3/2)^(-1 + v(1 + F(4*n - 3)))*(1 + F(4*n - 3)), where
 *          v(y) is the 2-adic valuation of y, F(x) = (3*x + 1)/2^v(3*x + 1),
 *          and x == 1 (mod 2).
 * https://oeis.org/A254070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first odd number in the 3x+1 trajectory of 4*n-3 that is
 *            ≡1 (mod 4)
 * Range:     u8 n <= 10, u16 n <= 682, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   collatz_successor_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/collatz_successor_odd.h"

seq_t a(idx_t n)
{
    /* 4*n-3 is odd for all n >= 1 */
    seq_t start = (seq_t)4 * (seq_t)n - 3;
    seq_t cur   = collatz_successor_odd(start);
    while ((cur & 3) != 1)
        cur = collatz_successor_odd(cur);
    return cur;
}
