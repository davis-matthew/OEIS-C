/*
 * A347210  Expansion of the e.g.f. (1 - 2*x - 2*log(1 - x) - exp(2*x)*(1 -
 *          x)^2) / 4 - 1.
 * https://oeis.org/A347210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-1)!/2 - (n-1)*(n-4)*2^(n-4) (for n ≥ 4)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = -1, a(1) = 0, a(2) = 1, a(3) = 2  */

#include <stdint.h>
#include <stdbool.h>
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
    /* handle the first few values explicitly */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* n ≥ 4 */
    seq_t fact = factorial(n - 1);              /* (n‑1)!   */
    seq_t half_fact = fact / 2;                 /* (n‑1)!/2 */
    seq_t pow2 = ipow((seq_t)2, n - 4);      /* 2^(n‑4)   */
    seq_t term2 = pow2 * ((seq_t)(n - 1) * (seq_t)(n - 4)); /* (n-1)*(n-4)*2^(n-4) */
    return half_fact - term2;                   /* a(n)     */
}
