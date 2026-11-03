/*
 * A395930  Triangle read by rows: T(n, k) = 4^n * [x^k] hypergeom([-n, -n,
 *          1/2], [1], x).
 * https://oeis.org/A395930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) with r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2; T(r,k) = 4^(r-k) * C(r,k)^2 * C(2*k,k) * k!
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

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

static seq_t compute_T(idx_t n, idx_t k)
{
    /* 4^(n-k) */
    seq_t res = ipow_u64_idx(4ULL, n - k);
    /* square binomial(n,k) */
    seq_t bin = binomial(n, k);
    res *= bin;
    res *= bin;
    /* multiply by binomial(2k,k) */
    res *= binomial(2 * k, k);
    /* multiply by k! */
    res *= factorial(k);
    return res;
}

seq_t a(idx_t n)
{
    /* determine row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrt((double)(8 * n + 1)) - 1.0) / 2.0);
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;
    return compute_T(r, k);
}
