/*
 * A363529  E.g.f. satisfies A(x) = exp(x * (1 + x * A(x)^4)).
 * https://oeis.org/A363529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{j=0..⌊n/2⌋} (4*j+1)^(n-j-1) * binomial(n, j) *
 *            (n-j)! / (n-2j)! (equivalent to OEIS A363529)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Fast power: base^exp */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* Compute n!  (nf = factorial of n) */
    seq_t nf = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) nf *= (seq_t)i;

    seq_t sum = (seq_t)0;
    idx_t max_j = n >> 1;                 /* floor(n/2) */

    /* fact_m holds (n-2j)!  – starts with n!  */
    seq_t fact_m = nf;
    idx_t m = n;

    /* fact_j holds j!  – starts with 0! = 1   */
    seq_t fact_j = (seq_t)1;

    for (idx_t j = 0; j <= max_j; ++j) {
        seq_t base = (seq_t)(4 * j + 1);
        idx_t exp = n - j - 1;            /* exponent for this term */
        seq_t powv = pow_seq(base, exp);

        /* Denominator = j! * (n-2j)!  */
        seq_t denom = fact_j * fact_m;

        /* Comb = n! / (j!*(n-2j)!)  */
        seq_t comb = nf / denom;

        /* Term contribution */
        seq_t term = powv * comb;
        sum += term;

        /* Prepare for next j */
        if (j < max_j) {
            /* Update j!   */
            fact_j *= (seq_t)(j + 1);

            /* Update (n-2j)!   */
            if (m > 1) {
                seq_t denomm = (seq_t)m * (seq_t)(m - 1);
                fact_m /= denomm;
            }
            m -= 2;
        }
    }
    return sum;
}
