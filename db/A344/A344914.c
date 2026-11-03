/*
 * A344914  T(n, k) = 2^(3*k)*(n - 3*k)!, for n >= 0 and 0 <= k <= floor(n/3).
 *          Triangle read by rows.
 * https://oeis.org/A344914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Sum_{k=0}^{⌊row/3⌋} 2^(3*k) * (row-3*k)! read by rows
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

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

/* total number of terms in all rows from row 0 up to and including row r */
static inline idx_t terms_upto(idx_t r)
{
    idx_t q   = r / 3;
    idx_t rem = r % 3;
    idx_t sum = (3 * q * (q - 1)) / 2 + (rem + 1) * q;
    return sum + r + 1;
}

seq_t a(idx_t n)
{
    /* find smallest r such that terms_upto(r) > n */
    idx_t hi = 0;
    while (terms_upto(hi) <= n)
        hi = (hi == 0 ? 1 : hi << 1);

    idx_t lo = hi >> 1;

    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (terms_upto(mid) > n)
            hi = mid;
        else
            lo = mid;
    }

    idx_t r      = hi;                       /* row containing the term */
    idx_t offset = (r == 0 ? 0 : terms_upto(r - 1));
    idx_t k      = n - offset;                /* column within the row   */
    idx_t m      = r - 3 * k;                 /* remaining factorial part */

    seq_t term = pow2(3 * k) * factorial(m);
    return term;
}
