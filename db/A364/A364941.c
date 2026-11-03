/*
 * A364941  E.g.f. satisfies A(x) = exp( x*A(x)^2 / (1 - x*A(x))^2 ).
 * https://oeis.org/A364941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n + k + 1)^{k-1} * binomial(n + k - 1,
 *            n - k) / k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t fact_n = factorial(n);          /* n! */
    seq_t r = fact_n;                    /* r holds n! / (k-1)! during the loop */
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t r_div_k = r / (seq_t)k;              /* n! / k!  (exact division) */
        seq_t base  = (seq_t)(n + k + 1);          /* base = n + k + 1 */
        seq_t exp   = (seq_t)(k - 1);             /* exponent = k-1 */
        seq_t power = ipow(base, exp);          /* (n + k + 1)^{k-1} */
        seq_t bin   = binomial((idx_t)(n + k - 1), (idx_t)(n - k));
        seq_t term  = r_div_k * power * bin;
        sum += term;
        r = r_div_k;                               /* prepare r for next iteration */
    }

    return sum;
}
