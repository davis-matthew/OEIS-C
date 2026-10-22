/*
 * A349812  Triangle read by rows: row 1 is [1]; for n >= 1, row n gives
 *          coefficients of expansion of (-1/x + x)*(1/x + 1 + x)^(n-1) in
 *          order of increasing powers of x.
 * https://oeis.org/A349812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 67, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle defined by:
   Row 0: [1]
   Row 1: [-1,0,1]
   For r ≥ 1, let row R be the coefficients of
   F_R(x) = (1 + x + x⁻¹) · F_{R-1}(x).
   Equivalently, each entry in a new row is the sum of up to
   three entries directly above it in the previous row.
   The flattened sequence a(n) is the triangle read by rows
   (row 0, then row 1, then row 2, …). */

#include <stdlib.h>

static seq_t **triangle = NULL;    /* array of rows */
static size_t triangle_used = 0;   /* number of rows stored */
static size_t triangle_cap  = 0;   /* allocated capacity */

/* helper: floor integer square root (no floating point) */
static idx_t isqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid <= n / mid)   /* mid*mid ≤ n without overflow */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ensure rows 0 … r are constructed */
static void ensure_rows(idx_t r)
{
    if ((size_t)r < triangle_used)
        return;

    /* grow storage of row pointers if needed */
    if ((size_t)r + 1 > triangle_cap) {
        size_t new_cap = triangle_cap ? triangle_cap * 2 : 4;
        while (new_cap <= (size_t)r + 1)
            new_cap *= 2;
        seq_t **tmp = (seq_t **)realloc(triangle, new_cap * sizeof(seq_t *));
        if (!tmp) abort();      /* allocation failed */
        triangle = tmp;
        triangle_cap = new_cap;
    }

    for (size_t i = triangle_used; i <= (size_t)r; ++i) {
        size_t len = 2 * i + 1;
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) abort();      /* allocation failed */
        for (size_t j = 0; j < len; ++j)
            row[j] = 0;         /* initialize to zero */

        if (i == 0) {
            row[0] = (seq_t)1;
        } else if (i == 1) {
            row[0] = (seq_t)-1;
            row[1] = (seq_t)0;
            row[2] = (seq_t)1;
        } else {
            seq_t *prev = triangle[i - 1];
            size_t prev_len = 2 * (i - 1) + 1;
            /* convolution with (1 + x + x⁻¹):
               new[j]   += prev[j]   (shift -1)
               new[j+1] += prev[j]   (shift  0)
               new[j+2] += prev[j]   (shift +1) */
            for (size_t j = 0; j < prev_len; ++j) {
                seq_t val = prev[j];
                row[j]     += val;
                row[j + 1] += val;
                row[j + 2] += val;
            }
        }

        triangle[i] = row;
    }
    triangle_used = r + 1;
}

seq_t a(idx_t n)
{
    /* Find the row that contains the nth term.  The
       cumulative number of terms up to row r-1 is r²,
       so r = floor(sqrt(n)). */
    idx_t r = isqrt(n);
    /* Position inside the row */
    idx_t p = n - r * r;   /* r² never overflows for n ≤ 2⁶⁴-1 */

    ensure_rows(r);
    return triangle[r][p];
}
