/*
 * A351345  G.f. A(x) satisfies: A(x) = 1 + x + x^2 + x^3 + x^4 + x^5 + x^6 *
 *          A(x/(1 - 2*x)) / (1 - 2*x).
 * https://oeis.org/A351345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n <= 5; for n >= 6, a(n) = Σ_{k=0}^{n-6}
 *            binomial(n-6,k) * 2^k * a(n-k-6)
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 23, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity of cache */
static idx_t cache_len = 0;    /* number of valid entries in cache */

/* Helper: ensure cache has at least n+1 entries allocated
   and that the first 6 terms (a[0]..a[5]) are initialized to 1. */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache == NULL) {
        /* allocate at least capacity 6 so indices 0..5 exist */
        size_t cap = need < 6 ? 6 : need;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) {
            /* allocation failed – return 0 on all subsequent calls */
            return;
        }
        /* initialize the first 6 terms */
        for (idx_t i = 0; i < 6; ++i) cache[i] = (seq_t)1;
        cache_cap = cap;
        cache_len = 6;
    }
    if (need > cache_cap) {
        size_t new_cap = need;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* realloc failed – keep old cache, may give wrong results afterwards */
            return;
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache == NULL) return (seq_t)0;  /* allocation failure */

    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 5) {
                cache[i] = (seq_t)1;
                continue;
            }
            idx_t m = i - 6;
            seq_t binom = (seq_t)1;          /* C(m,0) */
            seq_t power2 = (seq_t)1;         /* 2^0 */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= m; ++k) {
                idx_t idx = i - 6 - k;      /* index for a(i-k-6) */
                sum += binom * power2 * cache[idx];
                if (k < m) {
                    /* update binom to C(m,k+1) */
                    seq_t numerator = binom * (seq_t)(m - k);
                    binom = numerator / (seq_t)(k + 1);
                }
                power2 = power2 * (seq_t)2;
            }
            cache[i] = sum;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
