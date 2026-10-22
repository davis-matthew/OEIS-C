/*
 * A380799  Expansion of e.g.f. ( (1/x) * Series_Reversion( x * exp(-x * (1 +
 *          x)^2) / (1 + x)^3 ) )^2.
 * https://oeis.org/A380799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * n! * sum_{k=0..n} (n+2)^{k-1} * binomial(3*n+2*k+6,
 *            n-k)/k!
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, factorial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* n! */
    seq_t fact_n = factorial(n);

    /* term k = 0
     * (n+2)^(−1) * binom(3n+6, n) * n!   → integer part after division by (n+2) */
    seq_t C0 = binomial(3 * n + 6, n);
    seq_t sum = (C0 * fact_n) / (seq_t)(n + 2);

    /* term k = 1
     * binom(3n+8, n-1) * n!  (because (n+2)^{0}=1) */
    if (n >= 1) {
        seq_t C1 = binomial(3 * n + 8, n - 1);
        seq_t I1 = C1 * fact_n;          /* fact_n_over_1 = n! */
        sum += I1;

        /* Prepare for k >= 2 */
        seq_t I_prev = I1;          /* I_{k-1} for k=2 */

        for (idx_t k = 2; k <= n; ++k) {
            /* m and r for C_{k-1}   (k-1 = prev index)   */
            idx_t m = 3 * n + 2 * (k - 1) + 6;
            idx_t r = n - (k - 1);

            /* numerator and denominator of the binomial ratio C_k/C_{k-1} */
            unsigned long long numR = (unsigned long long)(m + 2) *
                                      (unsigned long long)(m + 1) *
                                      (unsigned long long)r;
            unsigned long long denomR = (unsigned long long)(m + 3 - r) *
                                        (unsigned long long)(m + 2 - r) *
                                        (unsigned long long)(m + 1 - r);

            /* I_prev / k  (k divides I_prev) */
            seq_t temp = I_prev / (seq_t)k;

            /* multiply by (n+2) */
            temp *= (seq_t)(n + 2);

            /* Reduce temp with denomR before multiplying by numR to keep values small */
            seq_t g1 = gcd(temp, (seq_t)denomR);
            temp /= g1;
            unsigned long long denomR_rem = denomR / (unsigned long long)g1;

            /* Reduce numR with the remaining denominator part */
            seq_t g2 = gcd((seq_t)numR, (seq_t)denomR_rem);
            unsigned long long numR_rem = numR / (unsigned long long)g2;
            denomR_rem /= (unsigned long long)g2;

            /* Compute the next term */
            seq_t prod = temp * (seq_t)numR_rem;
            seq_t I_curr = prod / (seq_t)denomR_rem;

            sum += I_curr;
            I_prev = I_curr;
        }
    }

    /* final a(n) = 2 * sum */
    return sum * (seq_t)2;
}
