/*
 * A279162  Number of n X 2 0..1 arrays with no element equal to a strict
 *          majority of its king-move neighbors, with the exception of exactly
 *          two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A279162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 19*a(n-2) + 31*a(n-3) - 52*a(n-4) + 82*a(n-5)
 *            * - 84*a(n-6) + 84*a(n-7) - 96*a(n-8) + 56*a(n-9) - 32*a(n-10) +
 *            32*a(n-11); * a(1)=1, a(2)=0, a(3)=0, a(4)=8, a(5)=24, a(6)=88,
 *            a(7)=284, * a(8)=772, a(9)=2000, a(10)=5008, a(11)=12060
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* base values for n = 1..11 */
    static const seq_t base[12] = {
        0,          /* dummy to make indices 1‑based */
        1,          /* a(1) */
        0,          /* a(2) */
        0,          /* a(3) */
        8,          /* a(4) */
        24,         /* a(5) */
        88,         /* a(6) */
        284,        /* a(7) */
        772,        /* a(8) */
        2000,       /* a(9) */
        5008,       /* a(10) */
        12060       /* a(11) */
    };

    if (n <= 11)
        return base[(idx_t)n];

    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity (in seq_t units) */
    static idx_t cache_len = 0;   /* highest index that has a value */

    /* ensure capacity for n+1 entries */
    if (cache_cap < n + 1) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 12)
            new_cap = 12;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                /* allocation failed */
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* initialise base values if not already done */
    if (cache_len < 11) {
        for (idx_t i = 1; i <= 11; ++i)
            cache[i] = base[i];
        cache_len = 11;
    }

    /* compute missing terms up to n */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t pos = 0;
        pos += 7  * cache[k - 1];
        pos += 31 * cache[k - 3];
        pos += 82 * cache[k - 5];
        pos += 84 * cache[k - 7];
        pos += 56 * cache[k - 9];
        pos += 32 * cache[k - 11];

        seq_t neg = 0;
        neg += 19 * cache[k - 2];
        neg += 52 * cache[k - 4];
        neg += 84 * cache[k - 6];
        neg += 96 * cache[k - 8];
        neg += 32 * cache[k - 10];

        cache[k] = pos - neg;
    }

    cache_len = n;
    return cache[n];
}
