/*
 * A377448  E.g.f. satisfies A(x) = 1/(1 + A(x)^4 * log(1 - x)).
 * https://oeis.org/A377448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k = 0..n} (5*k)!/(4*k+1)! * |Stirling1(n,k)|
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ----- static state for memoisation ----- */
    static seq_t *cache      = NULL;   /* cache[n] = a(n)   */
    static idx_t  cache_len  = 0;       /* number of cached values   */
    static size_t cache_cap  = 0;       /* capacity of cache         */

    static seq_t *ratio      = NULL;   /* ratio[k] = (5k)!/(4k+1)! */
    static idx_t  ratio_len  = 0;       /* number of pre‑computed ratios */
    static size_t ratio_cap  = 0;       /* capacity of ratio array    */

    static seq_t *s1         = NULL;   /* current row of |Stirling1(n,k)| */
    static idx_t  s1_n       = 0;       /* current n of the Stirling row */
    static size_t s1_cap     = 0;       /* capacity of s1 array        */

    /* ----- initialise on first call ----- */
    if (cache_cap == 0) {
        /* cache[0] = 1 */
        cache_cap = 1;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        cache[0] = 1;
        cache_len = 1;

        /* ratio[0] = 1 */
        ratio_cap = 1;
        ratio = (seq_t *)realloc(NULL, ratio_cap * sizeof(seq_t));
        ratio[0] = 1;
        ratio_len = 1;

        /* Stirling row for n = 0: |S1(0,0)| = 1 */
        s1_cap = 1;
        s1 = (seq_t *)realloc(NULL, s1_cap * sizeof(seq_t));
        s1[0] = 1;
        s1_n = 0;
    }

    /* ----- ensure cache memory is big enough ----- */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ----- ensure ratio memory is big enough ----- */
    if ((size_t)n + 1 > ratio_cap) {
        size_t new_cap = ratio_cap ? ratio_cap * 2 : 1;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(ratio, new_cap * sizeof(seq_t));
        ratio = tmp;
        ratio_cap = new_cap;
    }

    /* ----- compute missing factorial ratios ----- */
    if (ratio_len <= n) {
        for (idx_t k = ratio_len; k <= n; ++k) {
            if (k == 0) {
                ratio[0] = 1;      /* (0)!/(1)! = 1 */
            } else {
                seq_t prod = 1;
                for (idx_t t = 4 * k + 2; t <= 5 * k; ++t)
                    prod = prod * (seq_t)t;
                ratio[k] = prod;
            }
        }
        ratio_len = n + 1;
    }

    /* ----- ensure Stirling table capacity ----- */
    if ((size_t)n + 1 > s1_cap) {
        size_t new_cap = s1_cap ? s1_cap * 2 : 1;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(s1, new_cap * sizeof(seq_t));
        s1 = tmp;
        s1_cap = new_cap;
    }

    /* ----- compute a(i) for each new index up to n ----- */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* advance Stirling row to i */
        while (s1_n < i) {
            idx_t new_n = s1_n + 1;
            /* update in reverse order to use old values */
            for (idx_t k = new_n; k >= 1; --k) {
                s1[k] = s1[k - 1] + (seq_t)(new_n - 1) * s1[k];
            }
            s1[0] = 0;
            ++s1_n;
        }

        /* compute sum for a(i) */
        seq_t sum = 0;
        for (idx_t k = 0; k <= i; ++k)
            sum = sum + s1[k] * ratio[k];
        cache[i] = sum;
    }
    cache_len = n + 1;
    return cache[n];
}
