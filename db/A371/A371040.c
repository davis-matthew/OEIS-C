/*
 * A371040  E.g.f. satisfies A(x) = exp(x^3*A(x)^2) / (1-x).
 * https://oeis.org/A371040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A371040
   Formula: a(n) = Σ_{k=0..⌊n/3⌋} (n! / k!) * (2k+1)^(k-1) * binomial(n-k, n-3k)
   (a(0)=1) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

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
    seq_t fact_n = factorial(n);          /* n!  (exact integer) */
    seq_t sum    = 0;                     /* running total */
    seq_t fact_k = 1;                     /* k!  initially 0! = 1 */

    idx_t max_k = n / 3;                  /* floor(n/3) */

    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t bin = binomial(n - k, n - 3 * k);

        seq_t pow_val;
        if (k == 0) {
            pow_val = 1;                  /* (2*0+1)^{-1} = 1 */
        } else {
            pow_val = ipow_idx((seq_t)(2 * k + 1), k - 1);
        }

        seq_t term = fact_n / fact_k;     /* n! / k!  (integer) */
        term *= pow_val;                 /* * (2k+1)^(k-1) */
        term *= bin;                     /* * binomial(...) */

        sum += term;                     /* accumulate integer sum */

        /* prepare k! for next iteration */
        if (k < max_k) {
            fact_k *= (k + 1);
        }
    }

    return sum;                          /* final sequence value */
}
