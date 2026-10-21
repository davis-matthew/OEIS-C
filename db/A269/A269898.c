/*
 * A269898  Number of nX4 0..6 arrays with some element plus some horizontally
 *          or vertically adjacent neighbor totalling six exactly once.
 * https://oeis.org/A269898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1746*a(n-1) - 838311*a(n-2) + 67282834*a(n-3) -
 *            2128326885*a(n-4) + 29556635268*a(n-5) - 150523824676*a(n-6) for
 *            n > 7 Initial values: a(1)=756, a(2)=2005080, a(3)=2801265390,
 *            a(4)=3263186052840, a(5)=3482459146452300,
 *            a(6)=3529041005211929340, a(7)=3455975332370347120110
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms, 1‑based indexing */
    static seq_t *cache = NULL;        /* array of seq_t, cache[1..cached_n] */
    static idx_t  cached_n = 0;        /* largest n for which cache contains value */
    static size_t cache_cap = 0;       /* current allocated capacity of cache array */

    if (n < 1)                           /* undefined for n < 1, return 0 */
        return (seq_t)0;

    /* initial set of known terms (1..7) as strings for arbitrary precision */
    static const char *init_str[8] = {
        "",                               /* unused 0‑th element */
        "756",
        "2005080",
        "2801265390",
        "3263186052840",
        "3482459146452300",
        "3529041005211929340",
        "3455975332370347120110"
    };

    /* initialise cache with first 7 values on first call */
    if (cached_n == 0) {
        cache_cap = 8;                 /* capacity at least up to 7 */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;           /* allocation failure: give up gracefully */
        cache[0] = (seq_t)0;           /* unused */
        for (idx_t i = 1; i <= 7; ++i)
            cache[i] = seq_from_str(init_str[i]);
        cached_n = 7;
    }

    /* extend cache if necessary */
    if (n > cached_n) {
        /* enlarge cache while keeping it large enough for the new index */
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                /* double until big enough */
        if (new_cap != cache_cap) {
            /* allocate a new buffer and copy old contents */
            seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (!new_cache)
                return (seq_t)0;          /* allocation failure */
            for (size_t j = 0; j < cached_n + 1; ++j)
                new_cache[j] = cache[j];
            free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* compute missing values up to n using the recurrence */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t term;

            term  = (seq_t)1746   * cache[i - 1];
            term -= (seq_t)838311 * cache[i - 2];
            term += (seq_t)67282834  * cache[i - 3];
            term -= (seq_t)2128326885  * cache[i - 4];
            term += (seq_t)29556635268  * cache[i - 5];
            term -= (seq_t)150523824676 * cache[i - 6];

            cache[i] = term;
        }
        cached_n = n;
    }

    return cache[n];
}
