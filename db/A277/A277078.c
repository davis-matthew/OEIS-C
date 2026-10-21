/*
 * A277078  Triangular array similar to A255935 but with 0's and 2's swapped
 *          in the trailing diagonal. The columns alternate in signs.
 * https://oeis.org/A277078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = A007318(n) - A197870(n+1)
 * Range:     i8 n <= 58, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    long double d   = sqrtl((long double)8 * (long double)n + 1);
    idx_t      r    = (idx_t)((d - 1) / 2);

    /* Correct possible rounding errors. */
    while (r * (r + 1) / 2 > n)          --r;
    while ((r + 1) * (r + 2) / 2 <= n)   ++r;

    idx_t tri = r * (r + 1) / 2;   /* first index of row r */
    idx_t c   = n - tri;           /* column within the row */

    /* Trailing diagonal override: 2 if r even, 0 if r odd. */
    if (c == r) {
        return (r & 1) ? (seq_t)0 : (seq_t)2;
    }

    /* Off‑diagonal entries: (-1)^c * binom(r, c). */
    seq_t val = binom(r, c);
    if (c & 1) val = -val;
    return val;
}
