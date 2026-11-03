/*
 * A343698  a(n) is the number of preference profiles in the stable marriage
 *          problem with n men and n women such that there are n pairs of
 *          soulmates (people who rank each other first).
 * https://oeis.org/A343698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)!^(2n) * n!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    if (n == 0) return (seq_t)0;   /* only n >= 1 is defined */
    seq_t fact_n   = factorial(n);    /* n!          */
    seq_t fact_nm1 = factorial(n-1);  /* (n-1)!     */
    seq_t exp      = (seq_t)(2*n);    /* 2n          */
    seq_t pow_term = ipow(fact_nm1, exp); /* (n-1)!^(2n) */
    return fact_n * pow_term;         /* final product */
}
