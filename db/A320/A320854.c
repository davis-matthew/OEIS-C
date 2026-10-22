/*
 * A320854  Number of 8-element subsets of [n] whose sum is a triangular
 *          number.
 * https://oeis.org/A320854
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 94, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: counts 8‑element subsets of [n] whose sum is a triangular number. */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static polynomials for Gaussian binomials [n choose k]_q  (k ≤ 8) */
    static seq_t *poly[9] = {0};
    static size_t poly_len[9] = {0};
    static size_t poly_cap[9] = {0};

    /* memoization cache for sequence values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;

    /* initialise when first called */
    if (!poly[0]) {
        poly[0] = (seq_t*)malloc(sizeof(seq_t));
        poly[0][0] = 1;
        poly_len[0] = poly_cap[0] = 1;
        for (int k = 1; k < 9; ++k)
            poly[k] = NULL;
    }

    if (!cache) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;
    }

    /* extend cache array if necessary */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return cached value if already computed */
    if (n <= cache_max) return cache[n];

    for (idx_t cur = cache_max + 1; cur <= n; ++cur) {
        seq_t contrib = 0;

        if (cur >= 8) {
            idx_t maxSum = 8 * cur - 28;
            for (idx_t m = 1; ; ++m) {
                seq_t T = (seq_t)m * (seq_t)(m + 1) / 2;
                if (T > (seq_t)maxSum) break;
                if (T < (seq_t)cur + 28) continue;          /* minimal sum uses cur+28 */
                seq_t s_seq = T - (seq_t)cur - 28;
                if (s_seq < (seq_t)poly_len[7]) {
                    idx_t s = (idx_t)s_seq;
                    contrib += poly[7][s];
                }
            }
        }

        cache[cur] = cache[cur - 1] + contrib;

        /* update Gaussian binomial polynomials for this cur */
        for (idx_t k = (cur < 8 ? cur : 8); k >= 1; --k) {
            size_t shift   = (size_t)(cur - k);
            size_t len_k1  = poly_len[k - 1];
            size_t new_len = shift + len_k1;

            if (new_len > poly_cap[k]) {
                seq_t *tmp =
                    (seq_t*)realloc(poly[k], new_len * sizeof(seq_t));
                if (!tmp) exit(1);
                memset(tmp + poly_cap[k], 0,
                       (new_len - poly_cap[k]) * sizeof(seq_t));
                poly[k] = tmp;
                poly_cap[k] = new_len;
            }

            for (size_t i = 0; i < len_k1; ++i)
                poly[k][shift + i] += poly[k - 1][i];
            poly_len[k] = new_len;
        }
    }

    cache_max = n;
    return cache[n];
}
