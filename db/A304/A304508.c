/*
 * A304508  a(n) = 5*(3*n+1)*(9*n+8)/2 (n>=0).
 * https://oeis.org/A304508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(3*n+1)*(9*n+8)/2
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t term1 = 3 * nn + 1;
    seq_t term2 = 9 * nn + 8;
    seq_t prod   = (term1 * term2) / 2;   /* product is even, safe to divide */
    return prod * 5;
}
