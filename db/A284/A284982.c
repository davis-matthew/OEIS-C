/*
 * A284982  Triangle read by rows: coefficients of the Laplacian polynomial of
 *          the n-cycle graph C_n.
 * https://oeis.org/A284982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is the n-th term of the triangle of coefficients of the
 *            Laplacian polynomial of the n‑cycle graph C_n. The polynomial
 *            for a cycle of size m is P_m(x) = det(L_m - xI) and satisfies
 *            for m ≥ 3 P_m(x) = (2 - x) P_{m-1}(x) - P_{m-2}(x) - 2x, with
 *            P_1(x) = -x and P_2(x) = -4x + x^2. The sequence a...
 * Range:     i8 n <= 29, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* --------------  internal state  ---------------- */

static seq_t  *cache   = NULL;   /* flat array holding all a(k) values in order */
static size_t  cache_len  = 0;   /* number of values currently cached       */
static size_t  cache_cap  = 0;   /* current capacity of the cache            */

static seq_t  *prev2   = NULL;   /* coefficients of P_{m-2}(x)   */
static seq_t  *prev1   = NULL;   /* coefficients of P_{m-1}(x)   */
static size_t  max_row  = 0;     /* highest row that has been computed  */

/* ------------------------------------------------ */

static inline size_t row_for_index(idx_t n)
/* Returns the row index m such that a(1..S(m-1)) are already in the
   cache and a(n) lies in row m.    */
{
    /* S(m) = m(m+3)/2  (# of terms in rows 1..m) */
    double val = sqrt(9.0 + 8.0 * (double)n);
    size_t m  = (size_t)((val - 3.0) * 0.5);   /* floor */
    while (m * (m + 3) / 2 < (size_t)n) ++m;
    return m;
}

/* Append the coefficients of a new row to the cache.
   row[0..len-1] must contain the coefficients in order. */
static void push_to_cache(const seq_t *row, size_t len)
{
    size_t new_len = cache_len + len;
    if (new_len > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap << 1 : 16;
        while (new_cap < new_len) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    for (size_t i = 0; i < len; ++i) cache[cache_len++] = row[i];
}

/* Compute rows up to and including row 'target'.
   Newly computed rows are appended to the cache. */
static void ensure_rows(size_t target)
{
    while (max_row < target) {
        ++max_row;
        if (max_row == 1) {
            /* Row 1: 0, -1 */
            seq_t *cur = (seq_t*)malloc(2 * sizeof(seq_t));
            cur[0] = 0;
            cur[1] = -1;
            if (prev1) free(prev1);
            prev1 = cur;
            prev2 = NULL;
            push_to_cache(cur, 2);
        } else if (max_row == 2) {
            /* Row 2: 0, -4, 1 */
            seq_t *cur = (seq_t*)malloc(3 * sizeof(seq_t));
            cur[0] = 0;
            cur[1] = -4;
            cur[2] = 1;
            seq_t *tmp = prev1;      /* previous row */
            prev2 = tmp;
            prev1 = cur;
            push_to_cache(cur, 3);
        } else {
            /* Row m (m ≥ 3) from recurrence:
               P_m(x) = (2 - x) P_{m-1}(x) - P_{m-2}(x) - 2x            */
            seq_t *cur = (seq_t*)malloc((max_row + 1) * sizeof(seq_t));
            cur[0] = 0;                                  /* constant term */
            for (size_t k = 1; k <= max_row; ++k) {
                seq_t term = 0;
                if (k <= max_row - 1)           term += (seq_t)2 * prev1[k];  /* 2 * P_{m-1}[k]   */
                term -= prev1[k - 1];                                /* - P_{m-1}[k-1]   */
                if (k <= max_row - 2)           term -= prev2[k];          /* - P_{m-2}[k]     */
                if (k == 1)                     term -= (seq_t)2;          /* - 2x             */
                cur[k] = term;
            }
            seq_t *tmp = prev2;
            prev2 = prev1;
            prev1 = cur;
            free(tmp);
            push_to_cache(cur, max_row + 1);
        }
    }
}

/* ---------------  public function  ---------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* undefined outside the offset */
    size_t idx = (size_t)n;
    size_t r   = row_for_index(idx);
    ensure_rows(r);
    return cache[idx - 1];
}
