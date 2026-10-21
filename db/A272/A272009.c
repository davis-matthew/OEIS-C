/*
 * A272009  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 413", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3) - 3*a(n-4) + 3*a(n-5) +
 *            a(n-6) - a(n-7) Initial values: a(0)=1, a(1)=9, a(2)=14,
 *            a(3)=62, a(4)=67, a(5)=187, a(6)=192
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* base values for indices 0..6 */
static const seq_t base[7] = {
    (seq_t)1,   /* a(0) */
    (seq_t)9,   /* a(1) */
    (seq_t)14,  /* a(2) */
    (seq_t)62,  /* a(3) */
    (seq_t)67,  /* a(4) */
    (seq_t)187, /* a(5) */
    (seq_t)192  /* a(6) */
};

#ifndef __cplusplus
/* C implementation – use a dynamic array allocated with malloc/realloc */
static seq_t *cache       = NULL;  /* array of computed values */
static size_t cache_cap   = 0;     /* allocated capacity (in elements) */
static idx_t computed_up_to = 0;   /* largest index computed so far */

/* grow the array of cached values until it can hold indices up to ``need`` */
static void ensure_capacity(idx_t need)
{
    size_t min_cap = (size_t)need + 1;               /* indices 0..need inclusive */
    if (cache_cap < min_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
        while (new_cap < min_cap) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                          /* allocation failed */

        /* initialise new entries to zero (safe for integral types) */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;

        cache     = tmp;
        cache_cap = new_cap;
    }
}
#endif

seq_t a(idx_t n)
{
    if (n <= 6)                    /* base case */
        return base[(size_t)n];

#ifndef __cplusplus
    /* initialise cache on first call */
    if (!cache) {
        ensure_capacity(6);
        for (size_t i = 0; i <= 6; ++i)
            cache[i] = base[i];
        computed_up_to = 6;
    }

    /* already computed? */
    if (n <= computed_up_to)
        return cache[(size_t)n];

    /* compute forward to the requested index */
    ensure_capacity(n);
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        cache[i] = cache[i - 1]
                 + (seq_t)3 * cache[i - 2]
                 - (seq_t)3 * cache[i - 3]
                 - (seq_t)3 * cache[i - 4]
                 + (seq_t)3 * cache[i - 5]
                 + cache[i - 6]
                 - cache[i - 7];
    }
    computed_up_to = n;
    return cache[(size_t)n];
#else
    /* C++ implementation – use a static std::vector for automatic memory */
    static std::vector<seq_t> cache;

    if (cache.empty()) {                  /* first call – initialise */
        cache.resize(7);
        for (size_t i = 0; i < 7; ++i)
            cache[i] = base[i];
    }

    if (n < cache.size())
        return cache[(size_t)n];

    size_t old_size = cache.size();
    cache.resize((size_t)n + 1);           /* grow to hold indices 0..n */

    for (size_t i = old_size; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 1]
                 + (seq_t)3 * cache[i - 2]
                 - (seq_t)3 * cache[i - 3]
                 - (seq_t)3 * cache[i - 4]
                 + (seq_t)3 * cache[i - 5]
                 + cache[i - 6]
                 - cache[i - 7];
    }
    return cache[(size_t)n];
#endif
}
