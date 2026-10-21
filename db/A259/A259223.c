/*
 * A259223  E.g.f.: Sum_{n>=1} x^(n^2) * exp(2*x^n) / n!.
 * https://oeis.org/A259223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(d-n/d) * binomial(d, n/d) * n!/d! for n>=1
 *            (see OEIS A259223). The implementation uses only integer
 *            arithmetic and the helper utilities for power and binomial
 *            coefficients
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 23, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/binomial.h"

/* Compute the summand for a particular divisor d of n.  If d < n/d the
   binomial coefficient vanishes, so the function returns 0 in that case. */
static inline seq_t term_d(idx_t n, idx_t d)
{
    idx_t nd = n / d;
    if (d < nd) return 0;            /* binomial(d, nd) == 0 */

    /* 2^(d - nd) */
    seq_t term = pow_uint(2, d - nd);

    /* binomial(d, nd) */
    term *= binomial(d, nd);

    /* product from (d+1) to n equals n! / d!.  Only multiplication is
       needed, no division is required. */
    seq_t prod = 1;
    for (idx_t k = d + 1; k <= n; ++k)
        prod *= (seq_t)k;

    term *= prod;
    return term;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined for n = 0, return 0 for safety */

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i) continue;

        /* divisor i */
        sum += term_d(n, i);

        /* complementary divisor n/i */
        idx_t j = n / i;
        if (j != i)
            sum += term_d(n, j);
    }
    return sum;
}
