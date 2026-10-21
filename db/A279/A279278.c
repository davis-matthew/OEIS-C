/*
 * A279278  Expansion of Product_{k>=1} (1 + x^(k*(k+1)*(k+2)/6)).
 * https://oeis.org/A279278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into distinct tetrahedral numbers
 *            (OEIS A279278). This function returns a(idx_t n), where a(0)=1
 *            and subsequent terms are computed via subset‑sum DP
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp_cache = NULL; /* cache of computed a(n) up to current limit */
static idx_t  cache_len = 0;   /* number of indices cached (max index + 1) */
static idx_t  cache_cap = 0;   /* allocated size of dp_cache */

/* Ensure dp_cache contains a[0..n].  Recompute all values if n > cache_len. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;           /* already computed */

    /* allocate or grow cache */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)realloc(dp_cache, new_cap * sizeof(seq_t));
        dp_cache = new_cache;
        cache_cap = new_cap;
    }

    /* recompute all a[0..n] from scratch */
    for (idx_t i = 0; i <= n; ++i) dp_cache[i] = 0;
    dp_cache[0] = 1;

    /* iterate over tetrahedral numbers t_k = k(k+1)(k+2)/6 */
    for (idx_t k = 1; ; ++k) {
        seq_t t = (seq_t)k * (k + 1) * (k + 2) / 6;   /* division exact */
        if (t > (seq_t)n) break;                     /* no larger t needed */

        idx_t tj = (idx_t)t;                         /* safe: t <= n */
        for (idx_t j = n; j >= tj; --j) {
            dp_cache[j] += dp_cache[j - tj];
        }
    }

    cache_len = n + 1;   /* indices 0..n are valid */
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return dp_cache[n];
}
