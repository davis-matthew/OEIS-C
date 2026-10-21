/*
 * A248167  Expansion of g.f.: 1 / AGM(1-33*x, sqrt((1-9*x)*(1-121*x))).
 * https://oeis.org/A248167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( Sum_{k=0..n} 3^(n-k) * 2^k * C(n,k) * C(2k,k) )^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;            /* a(0) = 1 */

    /* Compute term0 = 3^n, summing as we go. */
    seq_t term = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        term *= (seq_t)3;           /* term = 3^n */

    seq_t sum = term;

    /* Iterate k = 0 .. n-1 to generate subsequent terms and accumulate the sum. */
    for (idx_t k = 0; k < n; ++k) {
        idx_t kp1 = k + 1;

        /* numerator = 2 * (n-k) * (2*(k+1)) * (2*k+1) */
        unsigned long long num_ll = 2ULL * (n - k) *
                                   (2ULL * kp1) *
                                   (2ULL * k + 1ULL);
        seq_t num = (seq_t)num_ll;

        /* denominator = 3 * (k+1)^3 */
        unsigned long long denom_ll = 3ULL *
                                     ((unsigned long long)kp1 *
                                      kp1 *
                                      kp1);
        seq_t denom = (seq_t)denom_ll;

        term = (term * num) / denom; /* exact division */
        sum += term;
    }

    return sum * sum;                /* square the sum */
}
