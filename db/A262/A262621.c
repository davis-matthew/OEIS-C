/*
 * A262621  First differences of A262620.
 * https://oeis.org/A262621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 4 * ( 2^(⌊log2(n)⌋ + 2) - 1 - 2*n ), a(0) = 1
 * Range:     u8 n <= 63, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the first differences of A262620 via the closed form derived from A261693 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* a(0) = 1 */

    /* find floor(log2(n)) */
    idx_t k = 0;
    idx_t t = n;
    while (t >>= 1)          /* shift until zero; k counts the number of shifts */
        ++k;

    /* compute 2^(k+2) in 64-bit temporary */
    uint64_t pow = (uint64_t)1 << (k + 2);
    uint64_t val = pow - 1 - (uint64_t)2 * n;   /* intermediate value */
    uint64_t res = 4 * val;                     /* final a(n) */

    return (seq_t)res;   /* cast to seq_t (may truncate if seq_t is smaller) */
}
