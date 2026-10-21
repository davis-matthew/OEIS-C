/*
 * A280932  a(n) = 2*F(n-1) + 2*F(n-3) + 10*F(n-5) + 9*F(n-8) where n >= 8 and
 *          F = A000045.
 * https://oeis.org/A280932
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 56*F(n-7) + 41*F(n-8) (n ≥ 8) Recurrence: a(n) = a(n-1) +
 *            a(n-2), a(8)=56, a(9)=97
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;          /* undefined below offset */

    /* static cache: stores a(8)..a(current_max) */
    static seq_t *cache = NULL;
    static idx_t cache_start = 8;   /* i.e. cache[0] corresponds to a(8)   */
    static idx_t cache_end   = 9;   /* last computed index (inclusive)   */
    static idx_t cap = 0;           /* allocated capacity                */

    /* first time: allocate and seed the cache */
    if (!cache) {
        cap = 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache)
            return 0;  /* allocation failure – unlikely in tests */

        cache[0] = (seq_t)56;   /* a(8)  */
        cache[1] = (seq_t)97;   /* a(9)  */
        cache_start = 8;
        cache_end   = 9;
    }

    /* if the requested n is already cached, return it */
    if (n <= cache_end)
        return cache[n - cache_start];

    /* extend the cache capacity if necessary */
    idx_t needed_len = n - cache_start + 1;
    if (needed_len > cap) {
        idx_t new_cap = cap;
        while (new_cap < needed_len)
            new_cap <<= 1;          /* double until large enough */

        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return 0;              /* allocation failure */

        cache = new_cache;
        cap   = new_cap;
    }

    /* compute missing terms using the simple recurrence */
    for (idx_t i = cache_end + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-2) */
        cache[i - cache_start] =
            cache[(i - 1) - cache_start] + cache[(i - 2) - cache_start];
    }

    cache_end = n;
    return cache[n - cache_start];
}
