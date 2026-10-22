/*
 * A367962  Triangle read by rows. T(n, k) = Sum_{j=0..k} (n!/j!).
 * https://oeis.org/A367962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle read by rows where T(n,k) = Sum_{j=0..k}
 *            (n! / j!)
 * Range:     u8 n <= 16, u16 n <= 36, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n ≥ 0. Uses integer arithmetic only. */

#include <stddef.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that the entry lies in row r.
       row r has r+1 entries and the cumulative entries up to
       row r-1 are r*(r+1)/2. */
    idx_t r = 0;
    idx_t cumulative = 0;
    idx_t row_start = 0;        /* cumulative count before the current row */

    for (r = 0; ; ++r)
    {
        idx_t row_len = r + 1;              /* number of entries in this row */
        if (cumulative + row_len > n)        /* n lies in this row */
        {
            row_start = cumulative;
            break;
        }
        cumulative += row_len;
    }

    /* column index within the row */
    idx_t col = n - row_start;              /* 0 <= col <= r */

    /* Compute T(r, col) = sum_{j=0..col} r! / j! */
    seq_t fac  = factorial(r);              /* r! */
    seq_t term = fac;                       /* current term r! / j! */
    seq_t sum  = fac;                       /* accumulate sum */

    for (idx_t j = 1; j <= col; ++j)
    {
        term = term / (seq_t)j;             /* exact division */
        sum += term;
    }

    return sum;
}
