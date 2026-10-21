/*
 * A241580  Triangle read by rows: T(n,k) (1 <= k <= n) defined by T(n,n) =
 *          (n-1)^(n-1), T(n,k) = T(n,k+1) - (n-1)*T(n-1,k) for k = n-1 .. 1.
 * https://oeis.org/A241580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle T(n,k) defined by
 *            T(n,n) = (n-1)^(n-1), T(n,k) = T(n,k+1) – (n-1)*T(n-1,k) for 1 ≤
 *            k ≤ n-1. The sequence a(n) is the flattened triangle read by
 *            rows
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

/* cache of computed rows; _rows[0] is unused */
static seq_t **_rows = NULL;
static idx_t _rows_cap = 0;

/* compute rows up to ordinal n (inclusive) */
static void _ensure_rows(idx_t n)
{
    if (_rows_cap >= n) return;

    /* resize pointer array to hold rows 1..n */
    seq_t **tmp = (seq_t **)realloc(_rows, (n + 1) * sizeof(seq_t *));
    if (!tmp) return;                 /* silently fail on OOM */
    _rows = tmp;
    for (idx_t i = _rows_cap + 1; i <= n; ++i) _rows[i] = NULL;

    /* compute each new row sequentially */
    for (idx_t i = _rows_cap + 1; i <= n; ++i)
    {
        seq_t *row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (!row) continue;           /* skip if allocation fails */
        _rows[i] = row;

        if (i == 1)
        {
            /* definition: 0^0 = 1 for the first row */
            row[1] = (seq_t)1;
            continue;
        }

        seq_t prev = (seq_t)(i - 1);  /* (n-1) as seq_t */
        /* diagonal entry */
        row[i] = big_pow(prev, prev);

        /* other entries using recurrence */
        seq_t *prev_row = _rows[i - 1];
        for (idx_t k = i - 1; k >= 1; --k)
        {
            row[k] = row[k + 1] - prev * prev_row[k];
        }
    }
    _rows_cap = n;
}

/* public interface: returns the nth term of the triangle flattened by rows */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* determine the row r that contains position n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* ensure all rows up to r are computed */
    _ensure_rows(r);

    /* column within the row */
    idx_t before = (r - 1) * r / 2;
    idx_t k = n - before;            /* 1‑based column */

    return _rows[r][k];
}
