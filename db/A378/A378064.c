/*
 * A378064  a(n) = 5*n^4 - 6*n^2 + 1. Column 4 of A378066.
 * https://oeis.org/A378064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^4 - 6*n^2 + 1
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 171, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n;
    seq_t n2  = x * x;
    seq_t n4  = n2 * n2;
    seq_t term1 = (seq_t)5 * n4;
    seq_t term2 = (seq_t)6 * n2;
    seq_t res   = term1 + (seq_t)1;
    res -= term2;
    return res;
}
