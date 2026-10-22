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
 * Range:     i8 n <= 20, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   _riordan_row, catalan, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isqrt.h"     // provides idx_t _floor_isqrt_u64(idx_t)
#include "../utils/catalan.h"   // provides seq_t catalan(idx_t)
#include "../utils/_riordan_row.h"

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
