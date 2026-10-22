/*
 * A301717  Partial sums of A301716.
 * https://oeis.org/A301717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) + a(n-5) - a(n-6) - a(n-8) +
 *            a(n-9) for n > 8, with a(0)=1, a(1)=7, a(2)=19, a(3)=37,
 *            a(4)=55, a(5)=85, a(6)=121, a(7)=157, a(8)=205
 * Range:     u8 n <= 9, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of cached terms */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initial values (needed for the recurrence) */
    static const seq_t base[9] = {
        (seq_t)1, (seq_t)7, (seq_t)19, (seq_t)37, (seq_t)55,
        (seq_t)85, (seq_t)121, (seq_t)157, (seq_t)205
    };

    /* one–time allocation and initialization */
    if (!cache) {
        cache_cap = 9;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure */
        for (size_t i = 0; i < 9; ++i) cache[i] = base[i];
        cache_len = 9;
    }

    /* answer is already cached? */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t needed = (size_t)n + 1;
        size_t new_cap = (needed > 32) ? needed * 2 : 32;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t val = cache[(size_t)(i - 1)];
        val += cache[(size_t)(i - 3)];
        val -= cache[(size_t)(i - 4)];
        val += cache[(size_t)(i - 5)];
        val -= cache[(size_t)(i - 6)];
        val -= cache[(size_t)(i - 8)];
        val += cache[(size_t)(i - 9)];
        cache[(size_t)i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
