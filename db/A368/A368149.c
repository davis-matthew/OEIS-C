/*
 * A368149  Triangular array T(n,k), read by rows: coefficients of strong
 *          divisibility sequence of polynomials p(1,x) = 1, p(2,x) = 1 + 2*x,
 *          p(n,x) = u*p(n-1,x) + v*p(n-2,x) for n >= 3, where u = p(2,x), v =
 *          1 - x^2.
 * https://oeis.org/A368149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = n-th coefficient in the flattened triangle of
 *            strong‑divisibility polynomials p(n,x). The coefficients T(n,k)
 *            satisfy T(1,0) = 1; T(2,0) = 1, T(2,1) = 2; T(n,k) = T(n‑1,k) +
 *            2*T(n‑1,k‑1) + T(n‑2,k) – T(n‑2,k‑2) (n≥3) Each row n has length
 *            n, and the sequence a is obtained by concatenating...
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for incremental generation */
    static seq_t *cache      = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;

    static seq_t *row_prev2 = NULL;          /* p(n‑2) */
    static seq_t *row_prev1 = NULL;          /* p(n‑1) */
    static idx_t  prev2_len = 0;
    static idx_t  prev1_len = 0;
    static idx_t  rows_done = 0;             /* number of rows already generated */

    /* Ensure cache has room for len more elements */
    if (cache_len + (size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < cache_len + (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – return 0 as a fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate triangle rows until the cache holds at least n terms */
    while (cache_len < (size_t)n) {
        idx_t row_len = rows_done + 1;          /* current row number = rows_done + 1 */
        seq_t *row_curr = (seq_t *)malloc((size_t)row_len * sizeof(seq_t));
        if (!row_curr) return (seq_t)0;        /* allocation failure */

        if (row_len == 1) {
            row_curr[0] = (seq_t)1;
        } else if (row_len == 2) {
            row_curr[0] = (seq_t)1;
            row_curr[1] = (seq_t)2;
        } else {
            for (idx_t k = 0; k < row_len; ++k) {
                seq_t term_a = 0, term_b = 0, term_c = 0, term_d = 0;

                if (k < prev1_len) term_a = row_prev1[k];
                if (k > 0 && (k - 1) < prev1_len) term_b = row_prev1[k - 1];
                if (k < prev2_len) term_c = row_prev2[k];
                if (k > 1 && (k - 2) < prev2_len) term_d = row_prev2[k - 2];

                seq_t two_b = term_b + term_b;     /* 2 * term_b */
                seq_t tmp = term_a + two_b + term_c;
                row_curr[k] = tmp - term_d;
            }
        }

        /* Append the new row to the cache */
        for (idx_t i = 0; i < row_len; ++i) {
            cache[cache_len++] = row_curr[i];
        }

        /* Shift row pointers: current becomes previous row, etc. */
        if (row_prev2) free(row_prev2);
        row_prev2 = row_prev1;
        prev2_len = prev1_len;

        row_prev1 = row_curr;
        prev1_len = row_len;

        ++rows_done;
    }

    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
