/*
 * A377831  Expansion of e.g.f. (1/x) * Series_Reversion( x * (1 - x) *
 *          exp(-x) ).
 * https://oeis.org/A377831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n+1)^{k-1} * binomial(2*n-k, n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* B = binomial(2n, n)
       F = n!
       Compute term0 = B * F / (n+1) without loss of precision */
    seq_t B = binomial((idx_t)(2 * n), (idx_t)n);
    seq_t F = factorial(n);
    seq_t B0 = B;
    seq_t F0 = F;
    seq_t temp = (seq_t)(n + 1);

    seq_t g = gcd(B0, temp);
    B0 /= g;
    temp /= g;
    g = gcd(F0, temp);
    F0 /= g;
    temp /= g;   /* temp should now be 1 */

    seq_t sum = B0 * F0;  /* term0 */
    seq_t pow = 1;        /* (n+1)^{k-1} for k = 0 above, so 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update B to binomial(2n-k, n-k)
           Update F to n! / k!   */
        B *= (seq_t)(n - k + 1);
        B /= (seq_t)(2 * n - k + 1);
        F /= k;

        /* term_k = B * F * pow  (pow = (n+1)^{k-1}) */
        seq_t term = B * F;
        term *= pow;
        sum += term;

        /* Prepare pow for next iteration: (n+1)^{k} */
        pow *= (seq_t)(n + 1);
    }

    return sum;
}
