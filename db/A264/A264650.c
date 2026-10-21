/*
 * A264650  Number of (n+1)X(3+1) arrays of permutations of 0..n*4+3 with each
 *          element having directed index change 1,1 2,2 -1,0 or 0,-1.
 * https://oeis.org/A264650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-2) +10*a(n-3) +4*a(n-4) -2*a(n-5) -a(n-6) +4*a(n-7)
 *            +18*a(n-8) +25*a(n-9) -8*a(n-10) -6*a(n-11) -10*a(n-12) +a(n-15)
 *            (empirical recurrence) Initial values: a(1)=0, a(2)=4, a(3)=4,
 *            a(4)=22, a(5)=65, a(6)=193, a(7)=628, a(8)=1905, a(9)=5929,
 *            a(10)=18406, a(11)=56934, a(12)=176612, a(13...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* First 15 terms needed to seed the recurrence. */
    static const seq_t init[15] = {
        0ULL, 4ULL, 4ULL, 22ULL, 65ULL, 193ULL, 628ULL,
        1905ULL, 5929ULL, 18406ULL, 56934ULL, 176612ULL,
        547378ULL, 1696245ULL, 5258441ULL
    };

    /* Static cache for computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* maximum index allocated (inclusive) */
    static idx_t computed = 0;    /* largest index with a computed value */

    /* Return known values directly. */
    if (n <= 15U)
        return init[n - 1];

    /* Initialise cache on first call. */
    if (!cache) {
        cache_cap = 16; /* ensure space for indices 1..15 (and 16 if needed) */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failed – return 0 as fail‑safe */
        for (idx_t i = 1; i <= 15; ++i)
            cache[i] = init[i - 1];
        computed = 15;
    }

    /* Grow cache if the requested n exceeds current capacity. */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while ((idx_t)new_cap < n)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!new_cache)
            return 0; /* allocation failed – return 0 as fail‑safe */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 2) val += (seq_t)6 * cache[i - 2];
        if (i >= 3) val += (seq_t)10 * cache[i - 3];
        if (i >= 4) val += (seq_t)4 * cache[i - 4];
        if (i >= 5) val -= (seq_t)2 * cache[i - 5];
        if (i >= 6) val -= cache[i - 6];
        if (i >= 7) val += (seq_t)4 * cache[i - 7];
        if (i >= 8) val += (seq_t)18 * cache[i - 8];
        if (i >= 9) val += (seq_t)25 * cache[i - 9];
        if (i >= 10) val -= (seq_t)8 * cache[i - 10];
        if (i >= 11) val -= (seq_t)6 * cache[i - 11];
        if (i >= 12) val -= (seq_t)10 * cache[i - 12];
        if (i >= 15) val += cache[i - 15];
        cache[i] = val;
    }
    computed = n;

    return cache[n];
}
