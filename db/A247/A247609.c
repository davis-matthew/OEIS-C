/*
 * A247609  a(n) = Sum_{k=0..4} binomial(8,k)*binomial(n,k).
 * https://oeis.org/A247609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..4} binomial(8,k) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 196, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t coeffs[5] = {1, 8, 28, 56, 70};
    seq_t result = 0;
    seq_t bin = 1;           /* binomial(n,0) */
    result += coeffs[0] * bin;               /* k = 0 */

    for (idx_t k = 1; k <= 4; ++k) {
        if (n < k) break;                     /* binomial(n,k) = 0 for k > n */

        /* Compute binomial(n,k) from binomial(n,k-1) */
        bin = bin * (seq_t)(n - k + 1) / (seq_t)k;
        result += coeffs[k] * bin;
    }

    return result;
}
