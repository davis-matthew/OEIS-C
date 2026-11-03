/*
 * A349728  Triangle read by rows, T(n, k) = RisingFactorial(k, n) /
 *          FallingFactorial(n, k).
 * https://oeis.org/A349728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row index with * cumulative count >
 *            n, k = n - r(r+1)/2. * T(r,k) = rising(k,r)/falling(r,k) =
 *            binomial(r + k - 1, k - 1) * factorial(r - k)
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

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
    idx_t r = 0;
    /* Find r such that (r+1)(r+2)/2 > n */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;
    if (r == 0)            /* only T(0,0)=1 */
        return (seq_t)1;
    if (k == 0)            /* T(r,0)=0 for r>0 */
        return (seq_t)0;
    /* Compute T(r,k) using the identity */
    seq_t val = binomial(r + k - 1, k - 1);
    val = val * factorial(r - k);
    return val;
}
