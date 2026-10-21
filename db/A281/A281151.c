/*
 * A281151  a(n) = floor(4*n*(n+1)/5).
 * https://oeis.org/A281151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*floor(n/5)^2 + 4*(2*(n%5)+1)*floor(n/5) + (n%5)^2
 * Range:     u8 n <= 17, u16 n <= 285, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t k = n / 5;
    idx_t r = n % 5;
    seq_t kk = (seq_t)k;

    seq_t term1 = 20 * kk * kk;                 /* 20*k^2 */
    seq_t term2 = 4 * (seq_t)(2 * r + 1) * kk; /* 4*(2r+1)*k */
    seq_t term3 = (seq_t)(r * r);               /* r^2 */

    return term1 + term2 + term3;
}
