/*
 * A376038  E.g.f. satisfies A(x) = (-log(1 - x / (1 - A(x))^3)) * (1 -
 *          A(x))^2.
 * https://oeis.org/A376038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (3n-k-2)!/(3n-2k-1)! · |Stirling1(n,k)|
 *            Offset: 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   ensure_cap_ptr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_cap_ptr.h"

/* Return the n‑th term of OEIS A376038.
   The implementation is memoised – calls may arrive in any order. */
seq_t a(idx_t n) {
    /* Memoised cache of already computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cached_max = (idx_t)-1;   /* no values stored yet */

    /* Storage for two consecutive rows of unsigned Stirling numbers
       of the first kind (absolute values). */
    static seq_t *prev = NULL;
    static size_t prev_cap = 0;
    static seq_t *cur  = NULL;
    static size_t cur_cap  = 0;

    /* Initialisation on first call. */
    if (!cache) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_cap = 2;
        cached_max = 1;

        prev = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!prev) abort();
        prev[0] = (seq_t)0;
        prev[1] = (seq_t)1;
        prev_cap = 2;

        cur = NULL;
        cur_cap = 0;
    }

    if (n <= cached_max)          /* already known value */
        return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t m = cached_max + 1; m <= n; ++m) {
        /* Prepare current row of Stirling numbers (size m+1). */
        ensure_cap_ptr(&cur, &cur_cap, (size_t)(m + 1));
        /* cur[0] is always 0 */
        cur[0] = (seq_t)0;
        /* cur[1] = (m-1) * prev[1] */
        cur[1] = (seq_t)(m - 1) * prev[1];
        for (idx_t k = 2; k <= m; ++k) {
            seq_t val_prev_k_minus = prev[k - 1];
            seq_t val_prev_k = (k < prev_cap) ? prev[k] : (seq_t)0;
            cur[k] = val_prev_k_minus + (seq_t)(m - 1) * val_prev_k;
        }

        /* Compute a(m).  r starts at r_1 = 1. */
        seq_t sum = (seq_t)0;
        seq_t r   = (seq_t)1;
        for (idx_t k = 1; k <= m; ++k) {
            sum += cur[k] * r;
            if (k < m) {
                seq_t num1 = (seq_t)(3 * m - 2 * k - 1);
                seq_t num2 = (seq_t)(3 * m - 2 * k - 2);
                seq_t den  = (seq_t)(3 * m - k - 2);
                r = (r * num1 * num2) / den;   /* exact division */
            }
        }

        /* Expand cache if necessary. */
        ensure_cap_ptr(&cache, &cache_cap, (size_t)(m + 1));
        cache[m] = sum;

        /* Swap prev and cur for next iteration. */
        seq_t *tmp       = prev;
        prev              = cur;
        cur               = tmp;
        size_t tmp_cap = prev_cap;
        prev_cap       = cur_cap;
        cur_cap        = tmp_cap;
    }

    cached_max = n;
    return cache[n];
}
