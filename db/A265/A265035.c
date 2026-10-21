/*
 * A265035  Coordination sequence of 2-uniform tiling {3.4.6.4, 4.6.12} with
 *          respect to a point of type 4.6.12.
 * https://oeis.org/A265035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 3*a(n-3) - a(n-4) for n ≥ 9 with
 *            initial terms: a(0)=1, a(1)=3, a(2)=6, a(3)=9, a(4)=11, a(5)=14,
 *            a(6)=17, a(7)=21, a(8)=25
 * Range:     u8 n <= 85, u16 n <= 8918, u32 n <= 8918, u64 n <= 8918,
 *            u128 n <= 8918, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;   /* number of computed entries */

    /* initial 9 terms */
    static const seq_t init[9] = {
        (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)9, (seq_t)11,
        (seq_t)14, (seq_t)17, (seq_t)21, (seq_t)25
    };

    /* if n is within the initial block, return it directly */
    if (n < 9)
        return init[(size_t)n];

    /* first time: allocate and copy the initial block */
    if (cache == NULL) {
        /* start with at least 9 entries, but also enough for n if n<9 */
        size_t initial_cap = (n + 1 > 9) ? (n + 1) : 9;
        cache = (seq_t*)malloc(initial_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed, return 0 as fallback */
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init[i];
        cap = initial_cap;
        len = 9;
    }

    /* grow capacity if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1; /* double until sufficient */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* realloc failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute new entries up to n using the recurrence */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        /* recurrence: a(i) = 3*a(i-1) - 4*a(i-2) + 3*a(i-3) - a(i-4) */
        seq_t val = cache[(size_t)(i - 1)] * (seq_t)3;
        val += cache[(size_t)(i - 3)] * (seq_t)3;
        val -= cache[(size_t)(i - 2)] * (seq_t)4;
        val -= cache[(size_t)(i - 4)];
        cache[(size_t)i] = val;
    }
    len = n + 1;

    return cache[(size_t)n];
}
