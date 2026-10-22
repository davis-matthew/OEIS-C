/*
 * A340863  a(n) = n!*LaguerreL(n, -n^2).
 * https://oeis.org/A340863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n^(2*k) * (n-k)! * binomial(n,k)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the formula gives 1 */
    if (n == 0)
        return (seq_t)1;

    /* curFact = n!  ...  fact factor, starts as n! */
    seq_t curFact = 1;
    for (idx_t i = 1; i <= n; ++i)
        curFact = curFact * (seq_t)i;

    /* bin = C(n,0) = 1; pow_val = n^(0) = 1 */
    seq_t bin = 1;
    seq_t pow_val = 1;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* term = n^(2k) * (n-k)! * C(n,k)^2  */
        seq_t term = pow_val;
        term = term * curFact;
        term = term * bin;
        term = term * bin;

        sum = sum + term;

        if (k == n)   /* last iteration */
            break;

        /* update n^(2k) -> n^(2(k+1)) */
        pow_val = pow_val * (seq_t)n;
        pow_val = pow_val * (seq_t)n;

        /* update binomial coefficient C(n,k+1) = C(n,k) * (n-k)/(k+1) */
        bin = bin * (seq_t)(n - k);
        bin = bin / (seq_t)(k + 1);

        /* update factorial factor (n-k)! -> (n-k-1)! */
        curFact = curFact / (seq_t)(n - k);
    }

    return sum;
}
