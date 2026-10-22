/*
 * A315532  Coordination sequence Gal.6.662.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) + 4*a(n-3) - 5*a(n-4) + 6*a(n-5) -
 *            6*a(n-6) + 6*a(n-7) - 6*a(n-8) + 6*a(n-9) - 5*a(n-10) +
 *            4*a(n-11) - 3*a(n-12) + 2*a(n-13) - a(n-14); for n > 14 Initial
 *            terms (a(0) … a(14)): 1, 6, 11, 17, 22, 30, 34, 37, 45, 48, 56,
 *            66, 67, 71, 78
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 15 known initial values */
    static const seq_t init[15] =
    {
        1ULL, 6ULL, 11ULL, 17ULL, 22ULL, 30ULL,
        34ULL, 37ULL, 45ULL, 48ULL, 56ULL, 66ULL,
        67ULL, 71ULL, 78ULL
    };

    /* Cache to store computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Direct lookup for the initial 15 terms */
    if (n < 15)
        return init[n];

    /* Initialize cache with the 15 initial terms on first use */
    if (cache_len == 0)
    {
        cache_len = 15;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failure: caller may interpret 0 as error */
        for (idx_t i = 0; i < 15; ++i)
            cache[i] = init[i];
    }

    /* Expand cache to accommodate index n if necessary */
    if (cache_len <= n)
    {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure */
        /* Zero‑initialize new entries to avoid garbage (not strictly needed) */
        for (idx_t i = cache_len; i < new_len; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute missing entries up to index n using the linear recurrence */
    for (idx_t i = 15; i <= n; ++i)
    {
        seq_t pos = 0, neg = 0;

        pos += (seq_t)2 * cache[i - 1];
        neg += (seq_t)3 * cache[i - 2];
        pos += (seq_t)4 * cache[i - 3];
        neg += (seq_t)5 * cache[i - 4];
        pos += (seq_t)6 * cache[i - 5];
        neg += (seq_t)6 * cache[i - 6];
        pos += (seq_t)6 * cache[i - 7];
        neg += (seq_t)6 * cache[i - 8];
        pos += (seq_t)6 * cache[i - 9];
        neg += (seq_t)5 * cache[i -10];
        pos += (seq_t)4 * cache[i -11];
        neg += (seq_t)3 * cache[i -12];
        pos += (seq_t)2 * cache[i -13];
        neg += cache[i -14];

        cache[i] = pos - neg;
    }

    return cache[n];
}
