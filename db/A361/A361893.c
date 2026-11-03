/*
 * A361893  Triangle read by rows. T(n, k) = n! * binomial(n - 1, k - 1) / (n
 *          - k)!.
 * https://oeis.org/A361893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if n=0; * otherwise let r = floor((sqrt(8*n+1)-1)/2), *
 *            k = n - r*(r+1)/2. * If k == 0 -> a(n) = 0 (for r>0), * else
 *            a(n) = r! * C(r-1,k-1) / (r-k)!
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   binomial, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
    /* Handle first term */
    if (n == 0)
        return 1;

    /* Determine the row r and column k */
    idx_t t   = 8 * n + 1;                     /* 8*n+1 */
    idx_t s   = isqrt(t);            /* floor(sqrt(t)) */
    idx_t r   = (s - 1) / 2;                   /* r = floor((sqrt(8*n+1)-1)/2) */
    idx_t first = r * (r + 1) / 2;             /* index of first term in row r */
    idx_t k   = n - first;

    /* Edge cases */
    if (k == 0)   /* r>0, k==0 */
        return 0;

    /* Compute T(r,k) = r! * C(r-1,k-1) / (r-k)! */
    seq_t fact_r   = factorial(r);
    seq_t bin      = binomial(r - 1, k - 1);
    seq_t fact_den = factorial(r - k);
    seq_t res = fact_r;
    res *= bin;
    res /= fact_den;
    return res;
}
