/*
 * A304375  a(n) = 27*n^2/2 + 45*n/2 - 12 (n>=1).
 * https://oeis.org/A304375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*n^2/2 + 45*n/2 - 12 (offset 1)
 * Range:     u8 n <= 2, u16 n <= 48, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to maintain precision */
    seq_t N = (seq_t)n;
    /* Compute n*(27*n+45) first; this is always even, so division by 2 is exact */
    seq_t val = N * ((seq_t)27 * N + (seq_t)45);
    val /= (seq_t)2;
    val -= (seq_t)12;
    return val;
}
