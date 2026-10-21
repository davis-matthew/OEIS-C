/*
 * A278365  Number of n X 1 0..2 arrays with rows and columns in lexicographic
 *          nondecreasing order but with exactly three mistakes.
 * https://oeis.org/A278365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*binomial(n+6,11) + 5*binomial(n+5,11) -
 *            24*binomial(n+4,11) + 18*binomial(n+3,11) - 4*binomial(n+2,11)
 *            (n >= 5), a(n) = 0 for 1 <= n < 5. Derived from the generating
 *            function G(x) = x^5*(2-x)*(3-2x)*(1+2x-2x^2)/(1-x)^12
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 28, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 5u)
        return 0;

    /* Compute the binomial arguments (all are >= 11 for n >= 5) */
    idx_t k1 = n + 6;   /* n-5+11 */
    idx_t k2 = n + 5;   /* n-6+11 */
    idx_t k3 = n + 4;   /* n-7+11 */
    idx_t k4 = n + 3;   /* n-8+11 */
    idx_t k5 = n + 2;   /* n-9+11 */

    /* Separate positive and negative contributions to avoid
       unsigned underflow.  For n >= 5 we always have pos >= neg. */
    seq_t pos = 0;
    seq_t neg = 0;

    pos += (seq_t)6 * binomial(k1, 11);
    pos += (seq_t)5 * binomial(k2, 11);
    pos += (seq_t)18 * binomial(k4, 11);

    neg += (seq_t)24 * binomial(k3, 11);
    neg += (seq_t)4  * binomial(k5, 11);

    return pos - neg;
}
