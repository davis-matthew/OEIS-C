/*
 * A392419  Number of mutual-visibility sets in the n X 3 grid graph.
 * https://oeis.org/A392419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n^6 + 33*n^5 + 50*n^4 + 225*n^3 + 1379*n^2 - 1698*n +
 *            720)/180 for n >= 3; a(1) = 7, a(2) = 41
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 26, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return 7;
    if (n == 2) return 41;

    /* use Horner's rule to avoid very large intermediate values */
    seq_t t = 11;                          /* 11 */
    t = t * (seq_t)n + 33;                 /* 11*n + 33 */
    t = t * (seq_t)n + 50;                 /* (11*n + 33)*n + 50 */
    t = t * (seq_t)n + 225;                /* ... */
    t = t * (seq_t)n + 1379;
    t = t * (seq_t)n - 1698;
    t = t * (seq_t)n + 720;                /* final polynomial value */

    return t / 180;
}
