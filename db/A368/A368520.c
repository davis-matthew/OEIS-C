/*
 * A368520  Irregular triangular array T, read by rows: T(n,k) = number of
 *          sums |x-y|+|y-z| = k, where x,y,z are in {1,2,...,n} and x <= z.
 * https://oeis.org/A368520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = concatenation of rows of the irregular triangular array T
 *            where T(n,k) = |{(x,y,z)∈{1..n}³ : x ≤ z and |x−y|+|y−z| = k }|.
 *            The sequence is read row‑wise: 1, 2 2 2, 3 4 7 2 2, …
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* helper – integer square root (floor) for unsigned 64‑bit values      */
/* --------------------------------------------------------------------- */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    if (x < 2) return x;
    idx_t lo = 1, hi = x;
    while (lo <= hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid <= x / mid)        /* avoid overflow of mid*mid   */
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* ceil(sqrt(x)) – uses floor_sqrt_u64, safe for x≥1              */
static inline idx_t ceil_sqrt_u64(idx_t x)
{
    if (x == 0) return 0;
    idx_t s = floor_sqrt_u64(x - 1);
    return s + 1;
}

/* --------------------------------------------------------------------- */
/* storage for precomputed rows                                        */
/* --------------------------------------------------------------------- */
static seq_t **rows = NULL;     /* rows[0] unused, rows[1]..rows[rows_cap] filled     */
static size_t rows_cap = 0;      /* highest row index that has been computed          */

/* --------------------------------------------------------------------- */
/* build a single row of the triangle                                   */
/* --------------------------------------------------------------------- */
static void build_row(idx_t r)
{
    size_t len = 2 * r - 1;
    seq_t *row = (seq_t *)calloc(len, sizeof(seq_t));
    rows[r] = row;

    for (idx_t x = 1; x <= r; ++x)
    {
        for (idx_t z = x; z <= r; ++z)
        {
            idx_t k0 = z - x;                /* base k value (z−x)          */
            row[k0] += (seq_t)(k0 + 1);     /* y in [x, z] contribute      */

            /* y > z  (right side) */
            for (idx_t d = 1; d <= r - z; ++d)
                row[k0 + 2 * d] += 1;

            /* y < x  (left side) */
            for (idx_t d = 1; d <= x - 1; ++d)
                row[k0 + 2 * d] += 1;
        }
    }
}

/* --------------------------------------------------------------------- */
/* public sequence function                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset, return 0 */

    /* Determine which row r contains the nth term                  */
    idx_t r = ceil_sqrt_u64(n);           /* smallest r with r² ≥ n       */
    idx_t prev_sq = (r - 1) * (r - 1);     /* total terms before row r     */
    idx_t k = n - prev_sq - 1;             /* 0‑based column in row r      */

    /* Lazily compute missing rows up to r                            */
    if ((size_t)r > rows_cap)
    {
        size_t new_cap = r;
        seq_t **new_rows = (seq_t **)realloc(rows, sizeof(seq_t *) * (new_cap + 1));
        rows = new_rows;
        for (size_t i = rows_cap + 1; i <= new_cap; ++i) rows[i] = NULL;
        for (size_t i = rows_cap + 1; i <= new_cap; ++i) build_row((idx_t)i);
        rows_cap = new_cap;
    }

    return rows[r][(size_t)k];
}
