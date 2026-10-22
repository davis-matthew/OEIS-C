/*
 * A303212  Number of minimum total dominating sets in the n X n rook
 *          complement graph.
 * https://oeis.org/A303212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(2-3*n+n^2)^2 / 6 (for n>2) Special cases: a(1)=0,
 *            a(2)=1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t if needed */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    seq_t t = (seq_t)n;             /* t = n */
    seq_t n2 = t * t;               /* n^2 */
    seq_t inner = n2 - (seq_t)3 * t + (seq_t)2; /* 2 - 3n + n^2 */
    seq_t inner2 = inner * inner;   /* (2 - 3n + n^2)^2 */
    seq_t numerator = n2 * inner2;  /* n^2 * (2 - 3n + n^2)^2 */
    return numerator / (seq_t)6;    /* divide by 6 */
}
