/*
 * A390904  a(n) is the number of 3 X 3 matrices with elements 1..n where at
 *          least one row is strictly increasing.
 * https://oeis.org/A390904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9 - (n^3 - binomial(n,3))^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;               /* n as seq_t */
    seq_t n3  = n_t * n_t * n_t;        /* n^3 */
    seq_t bin3= binomial(n, 3);         /* C(n,3) */
    seq_t diff= n3 - bin3;              /* n^3 - binomial(n,3) */
    seq_t diff2= diff * diff;           /* (diff)^2 */
    seq_t diff3= diff2 * diff;          /* (diff)^3 */
    seq_t n6  = n3 * n3;                /* n^6 */
    seq_t n9  = n3 * n6;                /* n^9 */
    return n9 - diff3;
}
