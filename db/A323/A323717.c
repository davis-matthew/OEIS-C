/*
 * A323717  a(n) = Product_{k=0..n} (n! + k!).
 * https://oeis.org/A323717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} (n! + k!) (OEIS A323717)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 29 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

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
    /* Compute n! once. */
    seq_t fact_n = factorial(n);

    seq_t product = 1;
    seq_t fact_k  = 1;          /* k! for current k */

    for (idx_t k = 0; k <= n; ++k) {
        product *= (fact_n + fact_k);
        if (k < n)
            fact_k *= (k + 1); /* update (k+1)! from k! */
    }
    return product;
}
