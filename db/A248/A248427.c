/*
 * A248427  Circumference of the (n,n)-knight graph.
 * https://oeis.org/A248427
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1 + (-1)^n + 2*n^2)/2 for n>4; a(3) = 8, a(4) = 14
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3u) return (seq_t)8;
    if (n == 4u) return (seq_t)14;

    /* For n >= 5, the formula simplifies to:
       • a(n) = n^2          if n is even
       • a(n) = n^2 – 1      if n is odd    */
    seq_t nseq = (seq_t)n;
    seq_t sq    = nseq * nseq;           /* n^2 */
    return (n & 1u) ? (sq - (seq_t)1) : sq;
}
