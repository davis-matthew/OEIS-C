/*
 * A323937  Irregular triangle read by rows: Crude total numbers J(n,k) of
 *          k-6-catapolyheptagons.
 * https://oeis.org/A323937
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, more
 * Formula:   a(n) = J(r,c) where J(1,1)=1, J(1,2)=2, J(1,3)=1 and
 *            J(r,c)=4*J(r-1,c)+3*J(r-1,c-1) for r>1, 1≤c≤r+2. The triangle
 *            has length r+2 in row r (rows start at r=1)
 * Range:     u8 n <= 14, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t **rows = NULL;           /* rows[1] .. rows[max_row] */
static idx_t rows_alloc = 0;          /* number of rows already computed */

/* Number of terms up to and including row r = Σ_{i=1..r} (i+2)
   = r(r+1)/2 + 2r = (r^2+5r)/2. */
static inline idx_t C(idx_t r) { return (r * r + 5 * r) / 2; }

/* Ensure that rows up to max_r are allocated and filled. */
static void ensure_rows(idx_t max_r)
{
    if (rows_alloc >= max_r) return;

    /* Grow the array of row pointers. */
    size_t new_cap = (size_t)max_r + 1;           /* index 0 unused */
    seq_t **tmp = NULL;
#if defined(__cplusplus)
    tmp = reinterpret_cast<seq_t**>(realloc(rows, new_cap * sizeof(seq_t*)));
#else
    tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
#endif
    if (!tmp) abort();
    rows = tmp;

    /* Compute each new row. */
    for (idx_t r = rows_alloc + 1; r <= max_r; ++r) {
        size_t len = (size_t)r + 3;             /* indices 0..r+2, we use 1..r+2 */
        seq_t *cur = NULL;
#if defined(__cplusplus)
        cur = reinterpret_cast<seq_t*>(malloc(len * sizeof(seq_t)));
#else
        cur = (seq_t*)malloc(len * sizeof(seq_t));
#endif
        if (!cur) abort();
        rows[r] = cur;

        if (r == 1) {
            cur[1] = 1;  cur[2] = 2;  cur[3] = 1;
        } else {
            seq_t *prev = rows[r - 1];
            /* first element: 4*prev[1] (prev[0] treated as 0) */
            cur[1] = (seq_t)(4 * prev[1]);

            /* interior elements */
            for (idx_t k = 2; k <= r + 2; ++k) {
                seq_t v1 = (k <= r + 1) ? prev[k] : 0;   /* prev[k] may be out of bounds */
                seq_t v2 = prev[k - 1];
                cur[k] = (seq_t)(4 * v1 + 3 * v2);
            }
        }
    }
    rows_alloc = max_r;
}

/* Convert the overall index n (≥2) to triangle coordinates (row, col). */
static void get_row_col(idx_t n, idx_t *row, idx_t *col)
{
    if (n < 2) { *row = 0; *col = 0; return; }

    idx_t m = n - 1;           /* 1‑based position in the triangular array */

    /* Binary search for smallest r with C(r) ≥ m. */
    idx_t lo = 1, hi = 1;
    while (C(hi) < m) hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (C(mid) >= m) hi = mid;
        else             lo = mid + 1;
    }
    idx_t r = lo;
    idx_t prevC = (r == 0) ? 0 : C(r - 1);
    idx_t k = m - prevC;

    *row = r;
    *col = k;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    idx_t row, col;
    get_row_col(n, &row, &col);

    ensure_rows(row);
    return rows[row][col];
}
