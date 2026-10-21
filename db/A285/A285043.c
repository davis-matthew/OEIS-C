/*
 * A285043  Expansion of cosh(3*arctanh(2*sqrt(x))).
 * https://oeis.org/A285043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 1)*C(2*n, n) (OEIS A285043)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the central binomial coefficient C(2n, n) multiplicatively.
       This avoids large intermediate factorials and works for arbitrary
       precision seq_t types. */
    seq_t binom = 1;
    seq_t n_seq = (seq_t)n;
    for (idx_t i = 1; i <= n; ++i) {
        /* binom = binom * (n + i) / i */
        binom = binom * (n_seq + (seq_t)i) / (seq_t)i;
    }

    /* Multiply by (8*n + 1) */
    seq_t factor = (seq_t)8 * n_seq + (seq_t)1;
    return binom * factor;
}
