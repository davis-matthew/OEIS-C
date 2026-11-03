/*
 * A350149  Triangle read by rows: T(n, k) = n^(n-k)*k!.
 * https://oeis.org/A350149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = r^(r-k) * k! where r = floor((sqrt(8*n+1)-1)/2) (row
 *            index), k = n - r*(r+1)/2 (column index within that row)
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Validated: standard, pass, 61 terms through n = 1120 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t tri = r * (r + 1) / 2;
    idx_t k   = n - tri;            /* 0 <= k <= r */

    seq_t pow_val = ipow((seq_t)r, (seq_t)(r - k));
    seq_t fact_val = factorial(k);
    return pow_val * fact_val;
}
