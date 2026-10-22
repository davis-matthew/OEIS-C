/*
 * A321220  a(n) = n+2 if n is even, otherwise a(n) = 2*n+1 if n is odd.
 * https://oeis.org/A321220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+2 if n is even, a(n) = 2*n+1 if n is odd
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t once to avoid repeated conversions */
    seq_t sn = (seq_t)n;
    return (n & 1) ? (sn * (seq_t)2 + (seq_t)1) : (sn + (seq_t)2);
}
