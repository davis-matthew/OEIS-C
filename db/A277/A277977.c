/*
 * A277977  a(n) = n*(1-3n+2*n^2+2*n^3)/2.
 * https://oeis.org/A277977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(1-3*n+2*n*n+2*n*n*n)/2
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 254, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t and compute powers */
    seq_t sq = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t cu = sq * (seq_t)n;                /* n^3 */
    seq_t qu = cu * (seq_t)n;                /* n^4 */

    /* Build the polynomial: 2*n^4 + 2*n^3 + n - 3*n^2 */
    seq_t term = (seq_t)2 * qu;              /* 2*n^4 */
    term += (seq_t)2 * cu;                  /* +2*n^3 */
    term += (seq_t)1 * (seq_t)n;            /* +n */
    term -= (seq_t)3 * sq;                  /* -3*n^2 */

    /* Divide by 2 (exact division). */
    return term / (seq_t)2;
}
