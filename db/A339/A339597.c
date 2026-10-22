/*
 * A339597  When 2*n+1 first appears in A086799.
 * https://oeis.org/A339597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A339597:  a(n) = 2*(n+1) - (n+1 & -(n+1))
   (that is, n+1 with a 0 bit inserted just above its least‑significant 1‑bit) */

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n + (seq_t)1;               /* n+1 as a seq_t value   */
    seq_t low = x & (seq_t)(-x);                  /* lowest set bit of x     */
    return (x << 1) - low;                        /* 2*x - lowbit            */
}
