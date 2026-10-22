/*
 * A318224  a(n) = n! * [x^n] exp(x/(1 + n*x)).
 * https://oeis.org/A318224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * [x^n] exp(x/(1 + n*x))
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 7, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the sum formula:
 *   a(n) = Σ_{k=0..n} (-n)^(n-k) * binomial(n-1,k-1) * n! / k!
 *   (term for k=0 vanishes).          */
#include <stdint.h>

/* Helper: power of a positive base using exponentiation by squaring. */
static seq_t pow_uint(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp) {
        if (exp & (idx_t)1)
            result *= b;
        b *= b;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* n! */
    seq_t fact_n = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i)
        fact_n *= (seq_t)i;

    /* Current power n^(n-k) starting with k=1 (exponent n-1). */
    seq_t pow_n = pow_uint((seq_t)n, n - (idx_t)1);

    seq_t sum = (seq_t)0;
    seq_t comb = (seq_t)1;         /* binomial(n-1,0) = 1 for k=1 */
    seq_t fact_k = (seq_t)1;       /* (k-1)! for k=1 */

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        /* Update binomial coefficient for current k */
        if (k > (idx_t)1) {
            comb *= (seq_t)(n - k + (idx_t)1);
            comb /= (seq_t)(k - (idx_t)1);
        }

        /* Update factorial of k */
        fact_k *= (seq_t)k;

        /* Term = comb * n! / k! */
        seq_t term = comb * fact_n;
        term /= fact_k;

        /* Multiply by (-n)^(n-k).  Use sign from parity of exponent */
        if ((n - k) & (idx_t)1)
            term = -term;
        term *= pow_n;

        sum += term;

        /* Prepare pow_n for next iteration (divide by n) */
        if (k < n)
            pow_n /= (seq_t)n;
    }

    return sum;
}
