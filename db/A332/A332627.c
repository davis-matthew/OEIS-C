/*
 * A332627  a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) * k! * k^n.
 * https://oeis.org/A332627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) * k! * k^n (OEIS
 *            sequence A332627)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 229 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
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
    /* Special case n = 0: term is defined as 1. */
    if (n == 0) {
        return 1;
    }

    seq_t pos = 0;   /* sum of terms with positive sign */
    seq_t neg = 0;   /* sum of terms with negative sign */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin   = binomial(n, k);          /* C(n,k) */
        seq_t factk = factorial(k);            /* k!   */
        seq_t powk  = ipow_idx((seq_t)k, n);    /* k^n  */

        seq_t term = bin * factk * powk;        /* binom * k! * k^n */

        /* Sign: (-1)^(n-k) */
        if (((n - k) & 1) == 0) {
            pos += term;   /* positive */
        } else {
            neg += term;   /* negative */
        }
    }

    /* Final result: positive sum minus negative sum. */
    return pos - neg;
}
