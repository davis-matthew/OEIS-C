/*
 * A398865  a(n) is the number of tilings of a 9 X 10n rectangle into
 *          P-pentominoes (no reflections, only rotations).
 * https://oeis.org/A398865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2) + 9*a(n-3) - 8*a(n-4) - 3*a(n-5) -
 *            a(n-6) Initial values: a(0)=1, a(1)=3, a(2)=11, a(3)=56,
 *            a(4)=289, a(5)=1470
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (needed > *cap)
    {
        size_t new_cap = (*cap == 0) ? 16 : *cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *ptr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t computed = 0;   /* number of values already computed */

    /* Base terms (index 0..5) */
    const seq_t init[6] = {
        (seq_t)1,   /* a(0) */
        (seq_t)3,   /* a(1) */
        (seq_t)11,  /* a(2) */
        (seq_t)56,  /* a(3) */
        (seq_t)289, /* a(4) */
        (seq_t)1470 /* a(5) */
    };
    const size_t init_len = 6;

    /* If the caller requests a small index, just return the base value. */
    if (n < (idx_t)init_len)
        return init[(size_t)n];

    /* First time we need values beyond the base terms, set up the cache. */
    if (!cache)
    {
        ensure_capacity(init_len, &cap, &cache);
        for (size_t i = 0; i < init_len; ++i) cache[i] = init[i];
        computed = init_len;
    }

    size_t ns = (size_t)n;
    if (ns + 1 > cap)          /* make sure room for the requested index */
        ensure_capacity(ns + 1, &cap, &cache);

    /* Compute missing terms up to ns. */
    if (computed <= ns)
    {
        for (size_t i = computed; i <= ns; ++i)
        {
            seq_t term = (seq_t)6 * cache[i - 1]
                       - (seq_t)6 * cache[i - 2]
                       + (seq_t)9 * cache[i - 3]
                       - (seq_t)8 * cache[i - 4]
                       - (seq_t)3 * cache[i - 5]
                       -      cache[i - 6];
            cache[i] = term;
        }
        computed = ns + 1;
    }

    return cache[ns];
}
