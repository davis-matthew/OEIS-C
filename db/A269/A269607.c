/*
 * A269607  Number of length-4 0..n arrays with no repeated value differing
 *          from the previous repeated value by one or less.
 * https://oeis.org/A269607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 4*n^3 + 4*n^2 - n
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Indexed from 1: a(1) = 8, a(2) = 62, ... */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Ensure n is treated as seq_t before multiplying */
    seq_t nn = (seq_t)n;
    /* Compute the polynomial in a numerically stable order */
    seq_t term1 = nn * nn * nn * nn;          /* n^4 */
    seq_t term2 = 4 * nn * nn * nn;          /* 4*n^3 */
    seq_t term3 = 4 * nn * nn;               /* 4*n^2 */
    seq_t term4 = nn;                        /* n */
    return term1 + term2 + term3 - term4;
}
