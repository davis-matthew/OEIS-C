/*
 * A367254  a(n) = binomial(2*n - 1, n - 1)*(2*n)^n.
 * https://oeis.org/A367254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n - 1, n - 1) * (2*n)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;           /* a(0) = 1 */
    }

    /* Compute binomial(2*n - 1, n - 1) multiplicatively */
    seq_t bin = (seq_t)1;
    idx_t k = n - 1;
    idx_t N = 2 * n - 1;
    for (idx_t i = 1; i <= k; ++i) {
        bin = bin * (seq_t)(N - k + i);   /* multiply by numerator */
        bin = bin / (seq_t)i;             /* exact integer division by denominator */
    }

    /* Compute (2*n)^n */
    seq_t base = (seq_t)(2 * n);
    seq_t pow  = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow = pow * base;
    }

    /* Result */
    return bin * pow;
}
