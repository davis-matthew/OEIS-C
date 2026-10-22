/*
 * A319094  Triangle read by rows: T(0,0) = 1; T(n,k) = T(n-1, k) + T(n-1,
 *          k-1) + 2 T(n-1, k-2) + T(n-1, k-3) + T(n-1, k-4) + T(n-1, k-5) +
 *          T(n-1, k-6) for k = 0..6*n; T(n,k)=0 for n or k < 0.
 * https://oeis.org/A319094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A319094:  a(n) is the n‑th term when the triangle
 *  T(r,k) (coefficients of (1+x+2x^2+x^3+x^4+x^5+x^6)^r) is read
 *  row by row.  T(0,0)=1,  T(r,k)=T(r-1,k)+T(r-1,k-1)+2T(r-1,k-2)
 *  +T(r-1,k-3)+T(r-1,k-4)+T(r-1,k-5)+T(r-1,k-6) with
 *  non‑negative indices.  The routine builds the triangle
 *  on demand and caches all rows up to the largest row required.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the smallest row index r such that the index n lies in
 * row r (0‑based).  Each row r has length 6r+1 and the cumulative
 * count of elements up to row r is 3r^2+4r+1.
 */
static idx_t row_of_index(idx_t n)
{
    idx_t low = 0, high = 1;

    /* Find a high bound such that start_of_row(high) > n. */
    while (1) {
        __int128 t = (__int128)3 * high * high - 2 * high;
        if (t > n) break;
        high <<= 1;
        if (high > n + 1) {   /* avoid infinite loop for very large n */
            high = n + 1;
            break;
        }
    }

    /* Binary search for exact r.  We maintain
     * low <= r < high  and start_of_row(low) <= n < start_of_row(high). */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 t = (__int128)3 * mid * mid - 2 * mid;
        if (t <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Cached rows of the triangle.  rows[i] points to an array of length
 * 6*i+1 containing T(i,0..6i).  The arrays are allocated on demand. */
static seq_t **rows = NULL;
static size_t rows_capacity = 0;
static size_t rows_count = 0;

/* Ensure that rows array has space for at least nrows rows. */
static void ensure_rows_capacity(size_t nrows)
{
    if (nrows <= rows_capacity) return;
    size_t new_cap = rows_capacity ? rows_capacity * 2 : 8;
    if (new_cap < nrows) new_cap = nrows;
    seq_t **new_rows = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
    if (!new_rows) exit(1);          /* allocation failure */
    rows = new_rows;
    rows_capacity = new_cap;
}

/* Build triangle rows up to and including row r.  Previously built
 * rows are preserved and reused. */
static void build_rows_up_to(idx_t r)
{
    for (idx_t m = rows_count; m <= r; ++m) {
        size_t len = 6 * m + 1;
        seq_t *cur = (seq_t *)malloc(len * sizeof(seq_t));
        if (!cur) exit(1);               /* allocation failure */
        if (m == 0) {                    /* base row */
            cur[0] = (seq_t)1;
        } else {
            size_t prev_len = 6 * (m - 1) + 1;
            seq_t *prev = rows[m - 1];
            for (size_t i = 0; i < len; ++i) {
                seq_t val = 0;
                /* offset 0 */
                if (i < prev_len) val += prev[i];
                /* offset 1 */
                if (i >= 1 && i - 1 < prev_len) val += prev[i - 1];
                /* offset 2 (multiplied by 2) */
                if (i >= 2 && i - 2 < prev_len) val += prev[i - 2] * 2;
                /* offset 3 */
                if (i >= 3 && i - 3 < prev_len) val += prev[i - 3];
                /* offset 4 */
                if (i >= 4 && i - 4 < prev_len) val += prev[i - 4];
                /* offset 5 */
                if (i >= 5 && i - 5 < prev_len) val += prev[i - 5];
                /* offset 6 */
                if (i >= 6 && i - 6 < prev_len) val += prev[i - 6];
                cur[i] = val;
            }
        }

        ensure_rows_capacity((size_t)(m + 1));
        rows[(size_t)m] = cur;
        ++rows_count;
    }
}

seq_t a(idx_t n)
{
    /* Determine row r and column k of flat index n. */
    idx_t r = row_of_index(n);
    idx_t start_of_row = (idx_t)(((__int128)3 * r * r - 2 * r));
    idx_t k = n - start_of_row;

    /* Build rows up to r if necessary. */
    if (rows_count <= r) build_rows_up_to(r);

    return rows[(size_t)r][(size_t)k];
}
