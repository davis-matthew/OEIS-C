/*
 * A262600  Number of Dyck paths of semilength n and height exactly 4.
 * https://oeis.org/A262600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1)-16*a(n-2)+13*a(n-3)-3*a(n-4) for n>4,
 *            a(0)=a(1)=a(2)=a(3)=0, a(4)=1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation ------------------------------------- */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* next free index (size of cache) */

    /* initialise cache on first call ----------------------------------- */
    if (cache_len == 0) {
        cache_len = 5;                /* indices 0..4 needed for base values */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) { exit(1); }

        /* base values: a(0)=a(1)=a(2)=a(3)=0, a(4)=1 */
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)0;
        cache[4] = (seq_t)1;
    }

    /* return cached value if already computed ---------------------------- */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* enlarge cache to reach index n ----------------------------------- */
    size_t old_len = cache_len;
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) { exit(1); }
    cache = tmp;
    cache_len = new_len;

    /* compute new entries using the recurrence -------------------------- */
    for (idx_t i = (idx_t)old_len; i <= n; ++i) {
        cache[(size_t)i] = (seq_t)7  * cache[(size_t)i - 1]
                         - (seq_t)16 * cache[(size_t)i - 2]
                         + (seq_t)13 * cache[(size_t)i - 3]
                         - (seq_t)3  * cache[(size_t)i - 4];
    }

    return cache[(size_t)n];
}
