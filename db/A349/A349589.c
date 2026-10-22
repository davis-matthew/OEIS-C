/*
 * A349589  E.g.f. satisfies: A(x) * log(A(x)) = 1 - exp(-x*A(x)).
 * https://oeis.org/A349589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * (n-k+1)^(k-1) * Stirling2(n,k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   pow_seq_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow_seq_big.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Static cache for a(0..max_computed) */
    static seq_t *a_cache = NULL;
    static size_t a_len   = 0;   /* cache length (next free index) */

    /* Previous Stirling row cache */
    static seq_t *prev_row = NULL;
    static size_t prev_len = 0;   /* length of prev_row (row size) */

    /* Initialize caches on first call */
    if (a_len == 0) {
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        a_cache[0] = (seq_t)1;    /* a(0) = 1 */

        prev_row = (seq_t*)malloc(sizeof(seq_t));
        prev_row[0] = (seq_t)1;    /* S(0,0) = 1 */
        prev_len = 1;

        a_len = 1;
    }

    /* If n already computed, return cached value */
    if (n < a_len) {
        return a_cache[n];
    }

    /* Compute rows up to n, storing a values */
    for (idx_t m = a_len; m <= n; ++m) {
        size_t curr_len = (size_t)m + 1;
        seq_t *curr_row = (seq_t*)calloc(curr_len, sizeof(seq_t));
        if (!curr_row) return 0;   /* Abort on OOM */

        /* Compute Stirling row S(m,k) for k=1..m */
        for (size_t k = m; k >= 1; --k) {
            seq_t term1 = (k < prev_len) ? prev_row[k] : 0;
            seq_t term2 = prev_row[k - 1];
            curr_row[k] = (seq_t)k * term1 + term2;
        }
        /* curr_row[0] remains 0 */

        /* Compute a(m) using the summation formula */
        seq_t sum = 0;
        for (size_t k = 1; k <= (size_t)m; ++k) {
            seq_t base = (seq_t)(m - k + 1);
            seq_t pow = pow_seq_big(base, (idx_t)(k - 1));
            seq_t term = pow * curr_row[k];
            if (((int)(m - k)) & 1) term = -term;  /* (-1)^(m-k) */
            sum += term;
        }

        /* Expand cache and store */
        a_cache = (seq_t*)realloc(a_cache, (m + 1) * sizeof(seq_t));
        a_cache[m] = sum;

        /* Prepare for next iteration */
        free(prev_row);
        prev_row = curr_row;
        prev_len = curr_len;
        a_len = m + 1;
    }

    return a_cache[n];
}
