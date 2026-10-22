/*
 * A365206  Centered octachoral numbers.
 * https://oeis.org/A365206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^4 - 16*n^3 + 16*n^2 - 8*n + 1
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 152, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t res = (seq_t)8;        /* 8 */
    res = res * x - (seq_t)16;   /* 8*n - 16 */
    res = res * x + (seq_t)16;   /* (8*n - 16)*n + 16 */
    res = res * x - (seq_t)8;    /* [(...)+16]*n - 8 */
    res = res * x + (seq_t)1;    /* final polynomial */
    return res;
}
