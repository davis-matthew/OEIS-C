/*
 * A377109  a(n) = coefficient of the term that is independent of sqrt(2),
 *          sqrt(3), and sqrt(6) in the expansion of (2 + sqrt(2) +
 *          sqrt(3))^n.
 * https://oeis.org/A377109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A377109: a(n) = coefficient of the term independent of sqrt(2), sqrt(3), sqrt(6)
 * in (2 + sqrt(2) + sqrt(3))^n.
 * Recurrence: a(0)=1, a(1)=2, a(2)=9, a(3)=38,
 * a(n) = 8*a(n-1) - 14*a(n-2) - 8*a(n-3) + 23*a(n-4)  for n ≥ 4.
 */

#include <stdlib.h>
#include <stddef.h>

/* compute a(n) */
seq_t a(idx_t n)
{
    /* handle first four terms directly */
    if (n < 4) {
        static const seq_t basis[4] = {1, 2, 9, 38};
        return basis[n];
    }

    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size of cache */
    static idx_t computed_up_to = 3;   /* highest index computed so far */

    /* initialise cache if required */
    if (cache == NULL) {
        cache_cap = 8;  /* begin with capacity 8 to avoid frequent reallocs */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;   cache[1] = 2;
        cache[2] = 9;   cache[3] = 38;
    }

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        /* compute a(i) using a positive/negative split to stay non–negative */
        seq_t pos = cache[i - 1] * (seq_t)8 + cache[i - 4] * (seq_t)23;
        seq_t neg = cache[i - 2] * (seq_t)14 + cache[i - 3] * (seq_t)8;
        cache[i] = pos - neg;
    }
    computed_up_to = n;

    return cache[n];
}
