/*
 * A346949  Value of the permanent of the matrix [1-zeta^{j-k}]_{1<=j,k<=2n},
 *          where zeta is any primitive 2n-th root of unity.
 * https://oeis.org/A346949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(2*n)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-01)
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
    /* Compute m = 2*n.  Using m = n + n avoids overflow concerns
       for low‑level architectures. */
    idx_t m = n + n;
    seq_t f = factorial(m);
    return f * 2;
}
