/*
 * A358732  Number of labeled trees covering 2n nodes, half of which are
 *          leaves.
 * https://oeis.org/A358732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)!/n! * S(2n-2,n) = (2n)! * sum_{i=0}^n (-1)^i
 *            binom(n,i)*(n-i)^{2n-2} / (n!)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where S(k,m) is Stirling number of the second kind.  */

static seq_t pow_seq_small(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp)
    {
        if (exp & 1) result *= b;
        exp >>= 1;
        if (exp) b *= b;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1, n>=1 in practice */
    /* factorial of n */
    seq_t fact_n = 1;
    for (idx_t k = 2; k <= n; ++k) fact_n *= k;

    /* sum_{i=0}^n (-1)^i * C(n,i) * (n-i)^{2n-2} */
    seq_t sum = 0;
    seq_t binom = 1;                /* C(n,0) */
    idx_t exp = (idx_t)(2 * n - 2);
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t base = (seq_t)(n - i);
        seq_t power = pow_seq_small(base, exp);
        seq_t term = binom * power;
        sum += (i & 1) ? -term : term;

        if (i < n) /* update to C(n,i+1) */
        {
            /* binom_{i+1} = binom_i * (n-i) / (i+1)  */
            seq_t num = binom * (seq_t)(n - i);
            seq_t den = (seq_t)(i + 1);
            binom = num / den;      /* exact division */
        }
    }

    /* Stirling number: S(2n-2,n) = sum / n! */
    seq_t stirling = sum / fact_n;

    /* product (n+1)*(n+2)*...*(2n) */
    seq_t prod = 1;
    for (idx_t k = n + 1; k <= 2 * n; ++k) prod *= k;

    return prod * stirling;
}
