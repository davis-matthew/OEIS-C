/*
 * A380385  Triangle read by rows. T(1, 1) = 1, T(n, k) = [n >=
 *          k](Sum_{i=1..k-1} T[n - i, k - 1] - Sum_{i=1..n-1} T[n - i, k]).
 * https://oeis.org/A380385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n‑th element of the triangle read by rows * *
 *            Triangle definition: * T(1,1) = 1 * For n > 1: T(n,1) = 0 * For
 *            n >= k >= 2: * T(n,k) = Σ_{i=1}^{k-1} T(n-i,k-1) - Σ_{i=1}^{n-1}
 *            T(n-i,k) * * The sequence a(n) is the row‑major order of this
 *            triangle. * For example: 1,0,1,0,-1,1,0,0,-1,1,0...
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *triangle_cache = NULL;      /* flattened triangle values */
static idx_t  cache_cap      = 0;         /* allocated capacity (number of terms) */
static idx_t  cache_rows     = 0;         /* number of rows fully computed */

/* triangular number: number of terms up to and including row r */
static inline idx_t triangular(idx_t r)
{
    /* Use seq_t arithmetic for correctness; results fit in idx_t
       because r is small in practice (≤ ~142 for indices ≤ 10⁴). */
    return (idx_t)(((seq_t)r * (seq_t)(r + 1)) / 2);
}

/* Ensure cache can hold at least 'need' terms; grow if necessary. */
static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    if (triangle_cache == NULL) {
        triangle_cache = (seq_t*)malloc(need * sizeof(seq_t));
    } else {
        triangle_cache = (seq_t*)realloc(triangle_cache, need * sizeof(seq_t));
    }
    /* Initialize the new part to zero (optional, but clean). */
    for (idx_t i = cache_cap; i < need; ++i)
        triangle_cache[i] = 0;
    cache_cap = need;
}

/* Compute rows until we have at least 'needed_index' terms (1‑based). */
static void ensure_terms(idx_t needed_index)
{
    /* Compute which row contains this index. */
    while (triangular(cache_rows) < needed_index) {
        /* Add next row (index cache_rows + 1). */
        idx_t r = cache_rows + 1;
        idx_t row_start = triangular(r - 1);                      /* 0‑based offset */
        idx_t row_end   = triangular(r);                          /* exclusive index */
        ensure_capacity(row_end);

        if (r == 1) {
            /* Base case: single element 1 */
            triangle_cache[0] = 1;
        } else {
            /* T(r,1) = 0 for r > 1 */
            triangle_cache[row_start] = 0;

            for (idx_t k = 2; k <= r; ++k) {
                seq_t sum1 = 0;
                for (idx_t i = 1; i <= k - 1; ++i) {
                    idx_t rr = r - i;
                    idx_t cc = k - 1;          /* column index (1‑based) */
                    if (rr >= cc) {
                        idx_t idx = triangular(rr - 1) + (cc - 1);
                        sum1 += triangle_cache[idx];
                    }
                }
                seq_t sum2 = 0;
                for (idx_t i = 1; i <= r - 1; ++i) {
                    idx_t rr = r - i;
                    idx_t cc = k;             /* column index (1‑based) */
                    if (rr >= cc) {
                        idx_t idx = triangular(rr - 1) + (cc - 1);
                        sum2 += triangle_cache[idx];
                    }
                }
                triangle_cache[row_start + (k - 1)] = sum1 - sum2;
            }
        }
        ++cache_rows;   /* we just finished computing row r */
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based index into the flattened triangle.  Assume n >= 1. */
    if (n == 0) return 0;  /* safety, though the spec guarantees n >= 1 */
    ensure_terms(n);       /* compute rows up to required index */
    return triangle_cache[n - 1];
}
