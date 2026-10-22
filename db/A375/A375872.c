/*
 * A375872  E.g.f. satisfies A(x) = exp( 4 * (exp(x*A(x)^(3/4)) - 1) ).
 * https://oeis.org/A375872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * Σ_{k=0..n} (3*n+4)^(k-1) * Stirling2(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------------------- static cache ---------------------- */
    static seq_t *cache      = NULL;   /* cached a(k) values           */
    static idx_t cache_cap   = 0;      /* capacity of cache            */
    static idx_t cache_size  = 0;      /* number of cached values      */

    /* -------------------- Stirling tables --------------------- */
    static seq_t *prev       = NULL;   /* Stirling row for n-1         */
    static seq_t *cur        = NULL;   /* temporary Stirling row       */
    static idx_t prev_len    = 0;      /* current length of prev       */
    static idx_t cur_len     = 0;      /* current length of cur        */
    static idx_t prev_cap    = 0;      /* capacity of prev buffer      */
    static idx_t cur_cap     = 0;      /* capacity of cur buffer       */

    /* --------------------- first call init ------------------- */
    if (cache == NULL) {
        /* cache for sequence values */
        cache_cap  = 1;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]   = (seq_t)1;          /* a(0) = 1                    */
        cache_size = 1;

        /* Stirling buffer init */
        prev_cap = 1;
        prev     = (seq_t *)malloc(prev_cap * sizeof(seq_t));
        prev[0]  = (seq_t)1;            /* S(0,0) = 1                  */
        prev_len = 1;

        cur_cap = 1;
        cur     = (seq_t *)malloc(cur_cap * sizeof(seq_t));
        cur_len = 0;
    }

    /* ------------------- requests below cache ---------------- */
    if (n < cache_size)
        return cache[n];

    /* ------------------- extend cache if needed -------------- */
    if (n >= cache_cap) {
        cache_cap = n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* ------------------- compute missing values --------------- */
    for (idx_t k = cache_size; k <= n; ++k) {
        /* ensure cur has room for k+1 elements */
        if (k + 1 > cur_cap) {
            cur_cap = k + 1;
            cur = (seq_t *)realloc(cur, cur_cap * sizeof(seq_t));
        }

        cur[0] = (seq_t)0;                   /* S(k,0) = 0 for k>0 */

        seq_t sum   = (seq_t)0;
        seq_t power = (seq_t)1;              /* base^(j-1)   */
        seq_t base  = (seq_t)(3 * k + 4);    /* (3*k+4)      */

        for (idx_t j = 1; j <= k; ++j) {
            /* Stirling recurrence: S(k,j) = j*S(k-1,j) + S(k-1,j-1) */
            seq_t term_j = (j < prev_len) ? (seq_t)j * prev[j] : (seq_t)0;
            seq_t term_j1= (j - 1 < prev_len) ? prev[j - 1] : (seq_t)0;
            cur[j] = term_j + term_j1;

            sum += power * cur[j];
            power *= base;
        }

        cur_len = k + 1;
        cache[cache_size] = (seq_t)4 * sum;
        ++cache_size;

        /* ------------------- rotate buffers ------------------- */
        seq_t *tmp_buf   = prev;
        prev             = cur;
        cur              = tmp_buf;

        idx_t tmp_cap    = prev_cap;
        prev_cap         = cur_cap;
        cur_cap          = tmp_cap;

        idx_t tmp_len    = prev_len;
        prev_len         = cur_len;
        cur_len          = tmp_len;
    }

    return cache[n];
}
