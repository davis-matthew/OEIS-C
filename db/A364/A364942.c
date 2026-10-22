/*
 * A364942  E.g.f. satisfies A(x) = exp( x*A(x)^2 / (1 - x*A(x))^3 ).
 * https://oeis.org/A364942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n+k+1)^(k-1) * binomial(n+2*k-1, n-k)
 *            / k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, factorial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    seq_t sum = 0;
    seq_t fact_n = factorial(n);          /* n! */
    seq_t factorial_k = 1;                /* k! */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t base = (seq_t)(n + k + 1);          /* n+k+1 */
        seq_t pow_val = pow_seq(base, (int)(k - 1)); /* (n+k+1)^(k-1) */
        factorial_k *= k;                         /* k! */
        seq_t fact_div = fact_n / factorial_k;     /* n! / k! */
        seq_t binom = binomial(n + 2 * k - 1, n - k); /* binomial(n+2k-1, n-k) */
        seq_t term = fact_div;
        term *= pow_val;
        term *= binom;
        sum += term;
    }
    return sum;
}
