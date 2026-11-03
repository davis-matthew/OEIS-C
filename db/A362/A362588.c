/*
 * A362588  Triangle read by rows, T(n, k) = RisingFactorial(n - k, k) *
 *          FallingFactorial(n, k).
 * https://oeis.org/A362588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = binomial(n,k) * binomial(n-1,k) * (k!)^2 a(n) is the
 *            flattening of this triangle by rows (offset 0)
 * Range:     u8 n <= 17, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-01)
 * Helpers:   binomial, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/isqrt.h"

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
    /* Determine row r (0‑based) and column k such that
       n = r(r+1)/2 + k,  0 ≤ k ≤ r. */
    if (n == 0)
        return (seq_t)1;   /* row 0, col 0 */

    /* 8*n+1 fits in idx_t for all n up to 2^63/8. */
    idx_t sq = isqrt(8*n + 1);
    idx_t r = (sq - 1) / 2;          /* floor((-1+sqrt(8n+1))/2) */

    /* Adjust for any rounding errors (safe for all sizes). */
    while ((r + 1)*(r + 2)/2 <= n) r++;
    while (r*(r + 1)/2 > n) r--;

    idx_t k = n - r*(r + 1)/2;        /* column index */

    if (r == 0)                      /* row 0 handled above, but keep guard */
        return (seq_t)1;

    /* Compute T(r,k). */
    seq_t val = binomial(r, k);
    val *= binomial(r - 1, k);
    seq_t fk = factorial(k);
    val *= fk;
    val *= fk;

    return val;
}
