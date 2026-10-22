/*
 * A322891  Triangle, read by rows, each row n being defined by g.f.
 *          Product_{k=1..n} (k + x + 2*k*x^2), for n >= 0.
 * https://oeis.org/A322891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th term from the triangle read row‑wise, where the
 *            (r,k)-entry T(r,k) is the coefficient of x^k in ∏_{m=1..r} (m +
 *            x + 2*m*x^2). Recurrence for r ≥ 1: T(r,k) = r*T(r-1,k) +
 *            T(r-1,k-1) + 2*r*T(r-1,k-2), with out‑of‑range indices treated
 *            as 0. The triangle rows have lengths 2*r+1, and...
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 105, u64 n <= 268,
 *            u128 n <= 750, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* External function signature */
seq_t a(idx_t n)
{
    /* static data to cache computed terms */
    static seq_t *cache = NULL;          /* array of all terms */
    static size_t capacity = 0;         /* allocated capacity */
    static idx_t size_cache = 0;        /* number of cached terms */
    static idx_t rows_computed = 0;     /* number of rows fully computed (next row to add) */

    /* Helper to grow the buffer, ensuring at least 'need' elements */
    if (cache == NULL) {
        /* start with capacity for the initial row */
        capacity = 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed; return 0 as error */
        /* row 0 */
        cache[0] = (seq_t)1;
        size_cache = 1;
        rows_computed = 1;            /* next row to compute will be row 1 */
    }

    /* Return cached value if available */
    if (n < size_cache)
        return cache[n];

    /* Compute the row number containing index n */
    idx_t r = (idx_t)floor(sqrt((long double)n));
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;

    /* Compute rows until at least row r is available */
    for (idx_t row = rows_computed; row <= r; ++row) {
        idx_t len_curr  = 2 * row + 1;         /* length of the new row   */
        idx_t len_prev  = 2 * row - 1;         /* length of previous row */
        idx_t prev_start = (row - 1) * (row - 1); /* index of first element of previous row */

        /* Ensure we have enough space */
        size_t required = size_cache + len_curr;
        if (required > capacity) {
            capacity = required * 2;
            cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            if (!cache) return (seq_t)0;      /* allocation failed */
        }

        /* Compute the new row using the recurrence */
        for (idx_t j = 0; j < len_curr; ++j) {
            seq_t val = 0;

            if (j < len_prev) {
                val += (seq_t)row * cache[prev_start + j];
            }
            if (j > 0 && (j - 1) < len_prev) {
                val += cache[prev_start + (j - 1)];
            }
            if (j > 1 && (j - 2) < len_prev) {
                val += ((seq_t)(2 * row)) * cache[prev_start + (j - 2)];
            }

            cache[size_cache + j] = val;
        }

        size_cache += len_curr;
        rows_computed = row + 1; /* next row to compute */
    }

    /* After filling, return the requested term */
    if (n < size_cache)
        return cache[n];
    /* Theoretically unreachable */
    return (seq_t)0;
}
