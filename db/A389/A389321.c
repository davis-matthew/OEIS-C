/*
 * A389321  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+2*k-1,n-k).
 * https://oeis.org/A389321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+2*k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 gives the first term */
    if (n == 0)
        return 1;

    seq_t sum = 0;
    seq_t binom_nk = 1;        /* binom(n,0) */
    seq_t binom_upper_prev = 0;/* B_{k-1} in the recurrence */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t binom_upper = 0;

        if (k == 0) {
            /* binom(n-1,n) = 0 for n>0, term contributes nothing */
            binom_upper = 0;
        } else if (k == 1) {
            /* compute B_1 = binom(n+1, n-1) directly */
            idx_t upper = n + 1;
            idx_t lower = n - 1;
            if (lower <= upper) {
                seq_t val = 1;
                for (idx_t i = 1; i <= lower; ++i) {
                    val = val * (upper - lower + i);
                    val = val / i;
                }
                binom_upper = val;
            }
        } else {
            /* recurrence for k ≥ 2:
               B_k = B_{k-1} * (n+2k‑1)(n+2k‑2)(n‑k+1) /
                               ((3k‑1)(3k‑2)(3k‑3)) */
            seq_t num = 1;
            num *= (seq_t)(n + 2*k - 1);
            num *= (seq_t)(n + 2*k - 2);
            num *= (seq_t)(n - k + 1);

            seq_t den = 1;
            den *= (seq_t)(3*k - 1);
            den *= (seq_t)(3*k - 2);
            den *= (seq_t)(3*k - 3);

            binom_upper = binom_upper_prev * num / den;
        }

        sum += binom_nk * binom_upper;

        if (k < n) {
            /* update binom(n,k+1) from binom(n,k) */
            binom_nk = binom_nk * (seq_t)(n - k) / (seq_t)(k + 1);
        }

        binom_upper_prev = binom_upper;
    }

    return sum;
}
