/*
 * A373651  Expansion of (1 - 2*x + 3*x^2)/(1 - 2*x - 3*x^2)^(5/2).
 * https://oeis.org/A373651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+2)/n^2) * ((2n-1)*a(n-1) + 3*(n+1)*a(n-2)) with
 *            initial values a(0) = 1, a(1) = 3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache && new_len > 0) {
            /* allocation failure – return zero as a be‑cause */
            return (seq_t)0;
        }
        cache = new_cache;
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)3;
            } else {
                seq_t term1 = ((seq_t)(2 * i - 1)) * cache[i - 1];
                seq_t term2 = ((seq_t)(3 * (i + 1))) * cache[i - 2];
                seq_t sum   = term1 + term2;
                seq_t num   = ((seq_t)(i + 2)) * sum;
                seq_t den   = (seq_t)i * (seq_t)i;
                cache[i]    = num / den;   /* division is exact */
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
