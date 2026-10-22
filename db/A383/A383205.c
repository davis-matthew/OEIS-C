/*
 * A383205  Expansion of e.g.f. f(x)^3 * exp(f(x)) / 6, where f(x) = (exp(2*x)
 *          - 1)/2.
 * https://oeis.org/A383205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=3..n} 2^(n-k) * binomial(k,3) * Stirling2(n,k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static state for memoisation */
    static seq_t *a_cache      = NULL;    /* cache of a(n) values       */
    static idx_t cache_len     = 0;       /* allocated length of a_cache*/
    static seq_t *prev         = NULL;    /* Stirling numbers of previous row  */
    static seq_t *cur          = NULL;    /* buffer for current Stirling row   */
    static idx_t arr_len       = 0;       /* allocated length of prev/cur      */
    static idx_t computed_max  = 0;       /* largest n already computed       */
    static int   initialized   = 0;       /* flag for first initialisation    */

    /* One‑time initialisation of the arrays */
    if (!initialized) {
        cache_len = 1;
        a_cache   = (seq_t *)calloc(cache_len, sizeof(seq_t));
        if (!a_cache) return (seq_t)0;

        arr_len  = 1;
        prev = (seq_t *)calloc(arr_len, sizeof(seq_t));
        cur  = (seq_t *)calloc(arr_len, sizeof(seq_t));
        if (!prev || !cur) {
            free(a_cache);
            return (seq_t)0;
        }
        prev[0] = 1;      /* S(0,0) = 1                           */
        initialized = 1;
    }

    /* Ensure the cache is large enough for index n */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        if (!a_cache) return (seq_t)0;
        for (idx_t i = cache_len; i < new_len; ++i)
            a_cache[i] = 0;
        cache_len = new_len;
    }

    /* Ensure the Stirling buffers are large enough for the next row */
    if (arr_len <= n) {
        idx_t new_len = n + 1;
        prev = (seq_t *)realloc(prev, new_len * sizeof(seq_t));
        cur  = (seq_t *)realloc(cur,  new_len * sizeof(seq_t));
        if (!prev || !cur) return (seq_t)0;
        arr_len = new_len;
    }

    /* Return cached value if already computed */
    if (n <= computed_max)
        return a_cache[n];

    /* Compute values for rows computed_max+1 .. n */
    for (idx_t next = computed_max + 1; next <= n; ++next) {
        /* Stirling2 recurrence: S(next,k) = k*S(next-1,k) + S(next-1,k-1) */
        cur[0] = 0;
        for (idx_t k = 1; k < next; ++k)
            cur[k] = prev[k] * (seq_t)k + prev[k - 1];
        cur[next] = prev[next - 1];   /* S(next,next) = 1 */

        /* Compute a(next) if needed */
        if (next < 3) {
            a_cache[next] = 0;
        } else {
            seq_t sum  = 0;
            seq_t pow2 = 1;             /* 2^(next-next)                        */
            for (idx_t k = next; k >= 3; --k) {
                seq_t binom = ((seq_t)k * (k - 1) * (k - 2)) / 6;   /* binomial(k,3) */
                seq_t term  = pow2 * binom * cur[k];
                sum += term;
                pow2 *= (seq_t)2;                /* next power of two */
            }
            a_cache[next] = sum;
        }

        /* Swap buffers for next iteration */
        seq_t *tmp  = prev;
        prev = cur;
        cur  = tmp;
    }

    computed_max = n;
    return a_cache[n];
}
