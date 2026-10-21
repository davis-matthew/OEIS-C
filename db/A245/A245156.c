/*
 * A245156  G.f.: Sum_{n>=0} x^n/((1+x)^(2*n)*(1 - (2*n)*x)).
 * https://oeis.org/A245156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Sum_{j=0..n-k} (2k)^{n-k-j} * (-1)^j *
 *            binom(2k+j-1, j)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute the OEIS sequence A245156 */
seq_t a(idx_t n)
{
    /* special case: k = 0 contributes only when n == 0 */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t pos_sum = 0;   /* sum of terms with positive sign */
    seq_t neg_sum = 0;   /* sum of terms with negative sign */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = n - k;   /* maximum j for this k */

        /* two_k = 2*k */
        seq_t two_k = (seq_t)k + (seq_t)k;

        /* compute initial power pow = (2k)^m */
        seq_t pow = 1;
        for (idx_t t = 0; t < m; ++t) {
            pow *= two_k;
        }

        /* B_0 = 1 for binomial part */
        seq_t B = 1;

        for (idx_t r = 0; r <= m; ++r) {
            seq_t prod = pow * B;          /* (2k)^{m-r} * binom(2k+r-1, r) */

            if ((r & 1) == 0) {
                pos_sum += prod;          /* even r -> + */
            } else {
                neg_sum += prod;          /* odd r -> - */
            }

            if (r < m) {
                /* prepare for next r: divide pow by (2k) */
                pow /= two_k;

                /* update B for next r: B_{r+1} = B_r * (2k + r) / (r+1) */
                seq_t numerator = B * ((seq_t)(2 * k + r));
                B = numerator / ((seq_t)(r + 1));
            }
        }
    }

    /* a(0) contributed by k = 0 is 1, already handled.
       The overall result is pos_sum - neg_sum which is non‑negative. */
    return pos_sum - neg_sum;
}
