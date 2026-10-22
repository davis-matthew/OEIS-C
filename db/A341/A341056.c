/*
 * A341056  a(n) = n! * [x^n] exp(x/(1 - n*x)) / (1 - x).
 * https://oeis.org/A341056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] exp(x/(1 - n*x)) / (1 - x)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute n! */
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact_n *= (seq_t)i;
    }

    seq_t sum = 0;           /* accumulator for the double sum */
    seq_t factor = fact_n;   /* initially fact_n / fact[0] */

    for (idx_t m = 0; m <= n; ++m) {
        if (m == 0) {
            /* only the term with p = 0 contributes (binom = 1) */
            sum += factor;
        } else {
            seq_t binom = 1;          /* binom(m + 0 - 1, 0) */
            seq_t n_pow = 1;          /* n^0 */

            /* inner sum over p = 0 .. n-m */
            for (idx_t p = 0; p <= n - m; ++p) {
                seq_t term = binom * n_pow * factor;
                sum += term;

                if (p == n - m) break;

                /* update binom for next p:
                   binom(p+1) = binom(p) * (m+p) / (p+1)    */
                binom = binom * (seq_t)(m + p) / (seq_t)(p + 1);
                n_pow *= (seq_t)n;   /* next power of n */
            }
        }

        if (m < n) {
            /* prepare factor for m+1: divide by (m+1) */
            factor /= (seq_t)(m + 1);
        }
    }

    return sum;
}
