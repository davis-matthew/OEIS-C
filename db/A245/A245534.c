/*
 * A245534  a(n) = n^2 + floor(n/2)*(-1)^n.
 * https://oeis.org/A245534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + floor(n/2)*(-1)^n
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic. */
    seq_t nn = (seq_t)n;
    seq_t half = (seq_t)(n / 2);          /* floor(n/2) */
    seq_t sq = nn * nn;                   /* n^2 */

    /* If n is odd subtract half, otherwise add it. */
    return (n & 1U) ? (sq - half) : (sq + half);
}
