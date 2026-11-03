/*
 * A363744  E.g.f. satisfies A(x) = exp(x * (1 + x * A(x))^2).
 * https://oeis.org/A363744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n-k+1)^(k-1) * binomial(2k,n-k) / k!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

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
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    seq_t sum = 0;
    seq_t fact_n = factorial(n);   /* n!  (exact for big‑int, up to 64‑bit for n<=20) */
    seq_t fact_k = 1;              /* k!  (start with 0! = 1) */

    for (idx_t k = 1; k <= n; ++k) {
        fact_k *= (seq_t)k;                          /* update to k! */

        seq_t prod = fact_n / fact_k;                /* n! / k!  = product_{i=k+1}^n i */

        seq_t binom = binomial((idx_t)(2 * k), (idx_t)(n - k));
        if (binom == (seq_t)0) {                     /* zero term for n>0 */
            continue;
        }

        /* (n-k+1)^(k-1)  (pow_uint handles nothing beyond 64‑bit safely) */
        seq_t pow_term = ipow_u64_idx((uint64_t)(n - k + 1), (idx_t)(k - 1));

        seq_t term = prod;
        term = term * binom;
        term = term * pow_term;

        sum += term;
    }

    return sum;
}
