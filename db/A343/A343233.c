/*
 * A343233  Triangle read by rows: Riordan triangle T = (1 - x*c(x), x), with
 *          the generating function c of A000108 (Catalan).
 * https://oeis.org/A343233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = value of the Riordan triangle T = (1 - x*c(x), x) read by
 *            rows. For n >= 0 define r = row containing position n, where r
 *            satisfies r(r+1)/2 <= n < (r+1)(r+2)/2. Let c = n - r(r+1)/2 be
 *            the column in that row. Then a(n) = 1 if c == r, a(n) =
 *            -Catalan(r-1-c) otherwise
 * Range:     i8 n <= 27, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   catalan, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/catalan.h"
#include "../utils/isqrt_u64.h"

static idx_t _riordan_row(idx_t n)
{
    /* Find r: largest r with r(r+1)/2 <= n.  Binary search using 128-bit
       arithmetic to avoid overflow. */
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* r */
}

seq_t a(idx_t n)
{
    idx_t r = _riordan_row(n);
    /* start index of row r */
    idx_t start = (idx_t)(((__uint128_t)r * (r + 1)) / 2);
    idx_t c = n - start;            /* 0 <= c <= r */

    if (c == r)
        return (seq_t)1;

    /* For c < r: a(n) = -Catalan(r - 1 - c) */
    idx_t cat_index = (r - 1) - c;
    seq_t cat = catalan(cat_index);
    return -cat;
}
