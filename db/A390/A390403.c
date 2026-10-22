/*
 * A390403  a(n) = (49*4^n + 6*n - 13)/9.
 * https://oeis.org/A390403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (49*4^n + 6*n - 13)/9
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 4^n with exponentiation by squaring. */
    seq_t pow4 = (seq_t)1;
    seq_t base = (seq_t)4;
    idx_t e = n;
    while (e) {
        if (e & (idx_t)1)
            pow4 *= base;
        base *= base;
        e >>= 1;
    }

    /* Compute the numerator: 49*4^n + 6*n - 13. */
    seq_t term1 = pow4 * (seq_t)49;
    seq_t term2 = (seq_t)6 * (seq_t)n;
    seq_t numerator = term1 + term2 - (seq_t)13;

    /* Divide by 9 to get the final result. */
    return numerator / (seq_t)9;
}
