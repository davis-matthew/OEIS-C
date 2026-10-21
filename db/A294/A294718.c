/*
 * A294718  Number of permutations of [n] avoiding {4231, 2134, 1243}.
 * https://oeis.org/A294718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/360)*(16560 - 13902*n + 4501*n^2 - 825*n^3 + 175*n^4 -
 *            33*n^5 + 4*n^6) for n>2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 33, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special cases up to n=2 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t N = (seq_t)n;
    /* Horner's rule for 4*n^6 - 33*n^5 + 175*n^4 - 825*n^3 + 4501*n^2 - 13902*n + 16560 */
    seq_t t = (seq_t)4;
    t = t * N - (seq_t)33;
    t = t * N + (seq_t)175;
    t = t * N - (seq_t)825;
    t = t * N + (seq_t)4501;
    t = t * N - (seq_t)13902;
    t = t * N + (seq_t)16560;

    return t / (seq_t)360;
}
