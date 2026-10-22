/*
 * A326714  a(n) = binomial(n,2) + (2-adic valuation of n).
 * https://oeis.org/A326714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,2) + v2(n)
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where binomial(n,2) = n*(n-1)/2 and v2(n) is the 2-adic valuation of n */

seq_t a(idx_t n)
{
    /* binomial(n,2) */
    seq_t binom = (seq_t)n * ((seq_t)n - 1) / 2;

    /* compute 2-adic valuation of n */
    idx_t m = n;
    seq_t v2 = 0;
    while ((m & 1) == 0) {
        v2++;
        m >>= 1;
    }

    return binom + v2;
}
