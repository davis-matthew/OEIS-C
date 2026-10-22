/*
 * A385207  G.f. A(x) satisfies A(x) = ( 1 + 49*x*A(x)^6 )^(1/7).
 * https://oeis.org/A385207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 49^n * binomial( (6*n+1)/7 , n ) / (6*n+1) Equivalent
 *            integer form: a(n) = 7^n * \prod_{i=0}^{n-1} (6*n+1-7*i) / (n! *
 *            (6*n+1)) This implementation computes the product and cancels
 *            the denominator progressively to avoid intermediate overflow
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 8, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   factorial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                          /* a(0) = 1 */

    seq_t denom = factorial(n);             /* n! */
    denom *= (seq_t)(6 * n + 1);            /* denom = n! * (6*n+1) */

    seq_t result = 1;
    seq_t base = (seq_t)(6 * n + 1);

    /* Multiply by the product terms and cancel common factors with denom */
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t term = base - (seq_t)(7 * i);
        result *= term;
        seq_t g = gcd(result, denom);
        if (g > 1) { result /= g; denom /= g; }
    }

    /* Multiply by 7^n, cancelling again */
    for (idx_t j = 0; j < n; ++j)
    {
        result *= 7;
        seq_t g = gcd(result, denom);
        if (g > 1) { result /= g; denom /= g; }
    }

    /* denom should now be 1; result is the desired term */
    return result;
}
