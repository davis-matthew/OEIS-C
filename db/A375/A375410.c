/*
 * A375410  Expansion of e.g.f. exp(-x * (1 - x)^2) / (1 - x).
 * https://oeis.org/A375410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 13, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) + (1-n)*a(n-1)+5*(1-n)*a(n-2)+7*(n-1)*(n-2)*a(n-3)-3*(n-1)*(n-2)*(n-3)*a(n-4)=0 */
/* Initial values: a(0)=1, a(1)=0, a(2)=5, a(3)=-4 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache     = NULL;
    static idx_t  cache_len = 0;      /* allocated size (number of slots)      */
    static idx_t  computed  = 0;      /* highest index for which a value exists */
    static int    init_done = 0;      /* flag whether base values were set       */

    /* ---------- initialise base values once ---------- */
    if (!init_done) {
        /* allocate space for first four entries */
        cache_len = (n < 4) ? 4 : (idx_t)4;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure (unlikely for small n) */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)-4;
        computed = 3;
        init_done = 1;
    }

    /* ---------- grow cache if needed ---------- */
    if (n + 1 > cache_len) {
        idx_t new_len = cache_len;
        while (new_len <= n) new_len <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failure */
        cache      = tmp;
        cache_len  = new_len;
    }

    /* ---------- compute missing values in order ---------- */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t k       = (seq_t)i;
        seq_t term1   = (k - 1)          * cache[i - 1];
        seq_t term2   = (seq_t)5 * (k - 1) * cache[i - 2];
        seq_t term3   = -(seq_t)7 * (k - 1) * (k - 2) * cache[i - 3];
        seq_t term4   =  (seq_t)3 * (k - 1) * (k - 2) * (k - 3) * cache[i - 4];
        cache[i] = term1 + term2 + term3 + term4;
    }
    computed = n;

    return cache[n];
}
