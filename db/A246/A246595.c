/*
 * A246595  Run Length Transform of squares.
 * https://oeis.org/A246595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (product of lengths of runs of 1's in binary expansion of
 *            n)^2
 * Range:     u8 n <= 494, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t i = n;
    idx_t run = 0;

    while (i > 0) {
        if (i & 1U) {
            run++;
        } else {
            if (run != 0) {
                prod *= (seq_t)run;
                run = 0;
            }
        }
        i >>= 1;
    }
    if (run != 0) {
        prod *= (seq_t)run;
    }

    return prod * prod;
}
