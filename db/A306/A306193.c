/*
 * A306193  a(n) = Product_{k=0..n} (1 + n!/k!).
 * https://oeis.org/A306193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \prod_{k=0..n} (1 + n! / k!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A306193 */
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
    /* n! */
    seq_t fact_n = factorial(n);

    seq_t prod = 1;          /* running product */
    seq_t fac_k = 1;         /* k! */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = (fact_n / fac_k) + 1;  /* 1 + n!/k! */
        prod *= term;                       /* multiply into product */

        /* Prepare factorial for next k */
        if (k < n)
            fac_k *= (k + 1);
    }

    return prod;
}
