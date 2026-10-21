/*
 * A243808  Position within the triangular array A226314(n)/A054531(n) of
 *          rationals x/y such that x < y, gcd(x,y)=1 and x+y odd.
 * https://oeis.org/A243808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate rational generator pairs (x,y) with
   x < y, gcd(x,y)=1 and x+y odd in the standard triangular order
   (index of pair (y,x) is (y-1)*y/2 + x).  Return the index of the
   nth such pair. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t y = 2;                     /* denominator (row) */
    idx_t x;                          /* numerator (column) */
    seq_t one = 1;                    /* constant for comparison */

    for (;; ++y) {
        for (x = 1; x < y; ++x) {
            if (((x + y) & 1U) && gcd((seq_t)x, (seq_t)y) == one) {
                ++found;
                if (found == n) {
                    /* index of (y,x) in the triangular array */
                    idx_t idx = (y - 1) * y / 2 + x;
                    return (seq_t)idx;
                }
            }
        }
    }
}
