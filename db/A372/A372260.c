/*
 * A372260  Triangle read by rows: T(n, k) = (T(n-1, k-1) + T(n-1, k)) * 2 * n
 *          with initial values T(n, 0) = Sum_{i=0..n} (-1)^(n-i) *
 *          binomial(n, i) * A001147(i) and T(i, j) = 0 if j > i.
 * https://oeis.org/A372260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is read row‑wise from the triangle T where T(0,0) = 1,
 *            T(1,0) = 0; T(r,0) = (2*r-2) * (T(r-1,0) + T(r-2,0)) for r > 1;
 *            T(r,k) = ( T(r-1,k-1) + T(r-1,k) ) * 2*r for 1 ≤ k ≤ r, with
 *            T(r,k) = 0 when k > r. The sequence a(n) obtains the entries
 *            T(row,col) in row‑major order
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached rows of the triangle.  Each row is an array of seq_t. */
static seq_t **triangle_cache = NULL;
static size_t cache_capacity = 0;   /* current allocated capacity of triangle_cache */
static size_t cache_rows = 0;       /* number of rows already computed */

/* Grow the triangle_cache array to hold at least new_cap rows. */
static void ensure_cache_capacity(size_t new_cap) {
    if (new_cap <= cache_capacity) return;
    size_t new_size = (cache_capacity == 0) ? 4 : cache_capacity * 2;
    if (new_size < new_cap) new_size = new_cap;
    /* cast so the code compiles as C++ as well */
    seq_t **tmp = (seq_t **)(realloc(triangle_cache,
                                     sizeof(seq_t *) * new_size));
    if (!tmp) abort();
    triangle_cache = tmp;
    cache_capacity = new_size;
}

/* Compute all rows up to and including row_index, storing them in the cache. */
static void compute_rows_up_to(size_t row_index) {
    for (size_t r = cache_rows; r <= row_index; ++r) {
        size_t len = r + 1;
        seq_t *row = (seq_t *)malloc(sizeof(seq_t) * len);
        if (!row) abort();

        /* First element of the new row */
        if (r == 0) {
            row[0] = (seq_t)1;
        } else if (r == 1) {
            row[0] = (seq_t)0;
        } else {
            seq_t lhs = triangle_cache[r-1][0];
            seq_t rhs = triangle_cache[r-2][0];
            row[0] = (seq_t)(2 * r - 2) * (lhs + rhs);
        }

        /* Remaining elements of the row */
        for (size_t k = 1; k <= r; ++k) {
            seq_t left  = triangle_cache[r-1][k-1];
            seq_t right = (k <= r-1) ? triangle_cache[r-1][k] : (seq_t)0;
            row[k] = (left + right) * (seq_t)(2 * r);
        }

        /* Store the new row in the cache */
        ensure_cache_capacity(cache_rows + 1);
        triangle_cache[cache_rows++] = row;
    }
}

/* Find the row index r such that the first element of row r is the
   first element whose cumulative count exceeds n. */
static size_t find_row_for_index(idx_t n) {
    size_t r = 0;
    idx_t cumulative = 0;
    for (;;) {
        cumulative += (idx_t)(r + 1);          /* add number of elements in row r */
        if (cumulative > n) break;
        ++r;
    }
    return r;
}

/*
 * Public function: returns a(n) where a(0)=1, a(1)=0, ...
 * The sequence is read from the triangular array row by row.
 */
seq_t a(idx_t n) {
    /* Determine which row contains index n */
    size_t row = find_row_for_index(n);

    /* Ensure all rows up to 'row' are computed and cached */
    if (row >= cache_rows) {
        compute_rows_up_to(row);
    }

    /* Number of elements in all preceding rows */
    idx_t pre_terms = (idx_t)row * (row + 1) / 2;
    idx_t col = n - pre_terms;

    return triangle_cache[row][(size_t)col];
}
