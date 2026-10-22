/*
 * A391978  a(n) = 2*n*binomial(n, 4).
 * https://oeis.org/A391978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*binomial(n,4) (and 0 for n < 4)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for the initial undefined values */
    if (n < 4)
        return 0;

    /* Compute binomial(n,4) multiplicatively to avoid large temporary values */
    seq_t bin = 1;
    for (idx_t i = 1; i <= 4; ++i) {
        bin = (bin * (seq_t)(n - 4 + i)) / (seq_t)i;
    }

    /* Final result: 2 * n * binomial(n,4) */
    return (seq_t)2 * (seq_t)n * bin;
}
