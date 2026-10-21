/*
 * A288156  Two even followed by three odd integers: the pattern is (0+2k,
 *          0+2k, 1+2k, 1+2k, 1+2k) for k >= 0.
 * https://oeis.org/A288156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n+1)/5)
 * Range:     u8 n <= 639, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (2*n+1)/5 using idx_t arithmetic to avoid overflow
       of seq_t for large n; the result fits into seq_t for all n
       that the build expects to test. */
    idx_t val = (2 * n + 1) / 5;
    return (seq_t)val;
}
