/*
 * A361091  E.g.f. satisfies A(x) = exp( x/(1 - x/A(x)^2) ).
 * https://oeis.org/A361091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..n} (-2*n+2*k+1)^(k-1) * binomial(n-1,
 *            n-k)/k! (k=0 term is zero for n>0)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 10, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* local integer power using exponentiation by squaring */
static seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp > 0) {
        if (exp & 1) result *= cur;
        exp >>= 1;
        if (exp) cur *= cur;
    }
    return result;
}

/* sequence A361091 */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return 1;

    /* compute n! */
    seq_t n_fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        n_fact *= (seq_t)i;

    /* fact_nk = n! / k! (starts with k=1 → n!/1!) */
    seq_t fact_nk = n_fact;

    /* binomial coefficient C(n-1, k-1); starts with k=1 → C(n-1,0)=1 */
    seq_t binom = 1;

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* base = -2*n + 2*k + 1 */
        seq_t base =
            (seq_t)(-2LL * (long long)n + 2LL * (long long)k + 1);

        /* power = base^(k-1) */
        seq_t power = pow_seq_local(base, k - 1);

        /* term = fact_nk * binom * power */
        seq_t term = fact_nk * binom;
        term *= power;

        sum += term;

        /* update for next iteration (k+1) */
        if (k < n) {
            /* binom <- binom * (n - k) / k  */
            binom = binom * (seq_t)(n - k) / (seq_t)k;
            /* fact_nk <- fact_nk / (k + 1) */
            fact_nk = fact_nk / (seq_t)(k + 1);
        }
    }

    return sum;
}
