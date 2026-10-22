/*
 * A357257  Number of n-node tournaments that have exactly three circular
 *          triads.
 * https://oeis.org/A357257
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*(2*(n-4) + (1/3)*(n-5)*(n-6) +
 *            (1/162)*(n-6)*(n-7)*(n-8)*[n>5])
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* term 1: 2*(n-4) */
    seq_t term1 = fact * ((seq_t)2 * ((seq_t)n - 4));

    /* term 2: (n-5)*(n-6)/3 , only for n ≥ 6 */
    seq_t term2 = 0;
    if (n >= 6) {
        seq_t fact_div3 = fact / 3;
        term2 = fact_div3 * ((seq_t)n - 5) * ((seq_t)n - 6);
    }

    /* term 3: (n-6)*(n-7)*(n-8)/162 , only for n ≥ 9 */
    seq_t term3 = 0;
    if (n >= 9) {
        seq_t fact_div162 = fact / 162;
        term3 = fact_div162 * ((seq_t)n - 6) * ((seq_t)n - 7) * ((seq_t)n - 8);
    }

    return term1 + term2 + term3;
}
