/*
 * A379990  Expansion of e.g.f. exp(-2*x)/(exp(-x) - x)^3.
 * https://oeis.org/A379990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1)^(n-k) * binomial(k+2,2)/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t prod_factor = 1;          /* n!/(n-k)! for current k */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t base_val = k + 1;
        idx_t exp_val  = n - k;

        /* (k+1)^(n-k) */
        seq_t power = pow_seq((seq_t)base_val, exp_val);

        /* binomial(k+2,2) = (k+2)*(k+1)/2 */
        seq_t bin_coeff = ((seq_t)(k + 2) * (seq_t)(k + 1)) / 2;

        /* term = prod_factor * power * bin_coeff */
        seq_t term = prod_factor;
        term *= power;
        term *= bin_coeff;

        sum += term;

        /* update prod_factor for next k (if any) */
        if (k < n)
            prod_factor *= (seq_t)(n - k);
    }

    return sum;
}
