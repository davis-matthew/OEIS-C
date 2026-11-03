/*
 * A377552  E.g.f. satisfies A(x) = 1/(1 - x * exp(x*A(x)^2))^2.
 * https://oeis.org/A377552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * n! * Sum_{k=0..n} k^(n-k) * binomial(4*n-3*k+2,k) /
 *            ((4*n-3*k+2)*(n-k)!)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer arithmetic only. It follows the
   equivalent form:
   a(n) = Σ_{k=1..n} 2 * binomial(n,k) * (k-1)! * k^(n-k)
          * binomial(4*n-3*k+1, k-1)   (k=0 contributes 0 for n>0). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

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
    if (n == 0)
        return 1;   /* a(0) = 1 */

    seq_t sum = 0;
    idx_t k;

    for (k = 1; k <= n; ++k) {
        /* binomial(n,k) */
        seq_t binom_nk      = binomial(n, k);

        /* (k-1)!  */
        seq_t fact_km1      = factorial(k - 1);

        /* k^(n-k) */
        seq_t power         = ipow((seq_t)k, (seq_t)(n - k));

        /* binomial(4*n-3*k+1, k-1) */
        idx_t D = 4 * n - 3 * k + 1;
        seq_t binom_inner   = binomial(D, k - 1);

        /* term = 2 * binom_nk * fact_km1 * power * binom_inner */
        seq_t term = binom_nk;
        term *= fact_km1;
        term *= power;
        term *= binom_inner;
        term *= 2;

        sum += term;
    }

    return sum;
}
