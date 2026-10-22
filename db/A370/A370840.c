/*
 * A370840  Expansion of (1/x) * Series_Reversion( x * (1/(1-x^3) - x) ).
 * https://oeis.org/A370840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/3⌋} (-1)^k * binomial(2*n-3*k+1,
 *            k) * binomial(2*n-3*k, n-3*k)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 14, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation avoids external dependencies by computing binomials
   multiplicatively using the provided seq_t type.  It works for arbitrary
   precision seq_t as well as the standard signed integer sizes. */

#include <stdint.h>

/* Helper: multiplicative binomial coefficient. */
static seq_t binomial_multiplicative(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;        /* use symmetry */
    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        /* result = result * (n - k + i) / i; integer division is exact */
        result = result * (seq_t)(n - k + i) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    idx_t maxk = n / (idx_t)3;    /* floor(n/3) */
    for (idx_t k = (idx_t)0; k <= maxk; ++k) {
        idx_t n1 = (idx_t)(2 * n - 3 * k + 1);
        idx_t n2 = (idx_t)(2 * n - 3 * k);
        idx_t k2 = (idx_t)(n - 3 * k);

        seq_t b1 = binomial_multiplicative(n1, k);
        seq_t b2 = binomial_multiplicative(n2, k2);

        seq_t prod = b1 * b2;
        if (k & (idx_t)1) {
            prod = -prod;
        }
        sum += prod;
    }
    /* divide by n+1; exact division guaranteed by the formula */
    sum = sum / (seq_t)(n + (idx_t)1);
    return sum;
}
