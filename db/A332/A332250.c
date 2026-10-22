/*
 * A332250  a(n) is the Y-coordinate of the n-th point of the quadratic Koch
 *          curve. Sequence A332249 gives X-coordinates.
 * https://oeis.org/A332250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0, a(n+1)=a(n)+h(n) where h(n)= 1 if (#1's in base‑5 of n
 *            minus #3's) ≡ 1 (mod 4), -1 if ≡ 3 (mod 4), 0 otherwise The
 *            value a(n) is the Y‑coordinate of the quadratic Koch curve
 * Range:     u8 n <= 6311, u16 n <= 9733, u32 n <= 9733, u64 n <= 9733,
 *            u128 n <= 9733, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] = a(i) for i = 0 .. cache_len-1.
       cache_len is number of elements currently stored. */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;
    static idx_t cache_cap   = 0;

    /* Ensure we have space for indices up to n. */
    if (n >= cache_len) {
        idx_t need = n + 1;                 /* at least 0 .. n inclusive */
        if (cache_cap < need) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
            if (new_cap < need) new_cap = need;
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) return (seq_t)0;   /* OOM – unlikely */
            /* Copy existing values */
            for (idx_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
            cache     = new_cache;
            cache_cap = new_cap;
        }

        /* Initialise a(0) if the cache has just been created. */
        if (cache_len == 0) {
            cache[0] = (seq_t)0;
            cache_len = 1;
        }

        /* Accumulated value for the last computed index. */
        seq_t acc = cache[cache_len - 1];

        /* Compute a(n) for all missing indices. */
        for (idx_t i = cache_len - 1; i < n; ++i) {
            /* compute t(i) = (#1's) – (#3's) in base‑5 of i */
            uint64_t x = i;
            int cnt1 = 0, cnt3 = 0;
            while (x) {
                int d = x % 5;
                if (d == 1) ++cnt1;
                else if (d == 3) ++cnt3;
                x /= 5;
            }
            int t = cnt1 - cnt3;
            int mod4 = ((t % 4) + 4) % 4;
            int contrib = 0;
            if (mod4 == 1) contrib = 1;
            else if (mod4 == 3) contrib = -1;
            acc += (seq_t)contrib;
            cache[i + 1] = acc;
        }

        cache_len = n + 1;
    }
    return cache[n];
}
