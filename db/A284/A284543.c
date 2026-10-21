/*
 * A284543  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 974", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A284543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-4) - 2*a(n-5) for n > 8 Initial values:
 *            a(0)=1, a(1)=3, a(2)=7, a(3)=11, a(4)=23, a(5)=43, a(6)=71,
 *            a(7)=219, a(8)=503
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute element a(n) of sequence A284543 */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static bool init = false;
    static const seq_t init_vals[9] = {
        1, 3, 7, 11, 23, 43, 71, 219, 503
    };

    /* initialise cache with first 9 terms on first call */
    if (!init) {
        cache_cap = 9;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure – caller will ignore */
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init_vals[i];
        init = true;
    }

    /* enlarge cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap)
            new_cap = new_cap ? new_cap * 2 : 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure – caller will ignore */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = 9; i <= n; ++i) {
        seq_t v1 = cache[i - 1];   /* a(i-1) */
        seq_t v4 = cache[i - 4];   /* a(i-4) */
        seq_t v5 = cache[i - 5];   /* a(i-5) */
        /* a(i) = 2*a(i-1) + a(i-4) - 2*a(i-5) */
        seq_t val = v1 + v1;   /* 2*a(i-1) */
        val = val + v4;        /* + a(i-4) */
        seq_t temp = v5 + v5;  /* 2*a(i-5) */
        val = val - temp;      /* - 2*a(i-5) */
        cache[i] = val;
    }

    return cache[n];
}
