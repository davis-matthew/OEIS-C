/*
 * A254705  Number of length 7+4 0..n arrays with every five consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A254705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) Initial
 *            values: a(1)=334, a(2)=9106, a(3)=87892, a(4)=489135,
 *            a(5)=1954106, a(6)=6255136, a(7)=17080712, a(8)=41376909,
 *            a(9)=91273510
 * Range:     u8 none, u16 n <= 2, u32 n <= 15, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    /* 1‑based indices – index 0 is unused */
    static const seq_t init[10] = {
        0,
        (seq_t)334,          /* a(1) */
        (seq_t)9106,         /* a(2) */
        (seq_t)87892,        /* a(3) */
        (seq_t)489135,       /* a(4) */
        (seq_t)1954106,      /* a(5) */
        (seq_t)6255136,      /* a(6) */
        (seq_t)17080712,     /* a(7) */
        (seq_t)41376909,     /* a(8) */
        (seq_t)91273510      /* a(9) */
    };

    /* Return known initial values directly */
    if (n <= 9) {
        return init[(size_t)n];
    }

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_size = 10;                 /* indices 0..9 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure */
        for (size_t i = 1; i <= 9; ++i) cache[i] = init[i];
    }

    /* Extend cache if necessary and compute new terms */
    if ((size_t)n >= cache_size) {
        size_t old_size = cache_size;
        size_t new_size = (size_t)n + 1; /* must hold index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure */
        cache = tmp;
        cache_size = new_size;
        /* Compute terms from old_size up to n inclusive */
        for (size_t i = old_size; i <= (size_t)n; ++i) {
            /* compute a(i) = 9*a(i-1) - 36*a(i-2) + 84*a(i-3) - 126*a(i-4)
               + 126*a(i-5) - 84*a(i-6) + 36*a(i-7) - 9*a(i-8) + a(i-9) */
            seq_t add = (seq_t)9   * cache[i-1]
                      +        (seq_t)84  * cache[i-3]
                      +        (seq_t)126 * cache[i-5]
                      +        (seq_t)36  * cache[i-7]
                      +                     cache[i-9];
            seq_t sub = (seq_t)36  * cache[i-2]
                      +        (seq_t)126 * cache[i-4]
                      +        (seq_t)84  * cache[i-6]
                      +        (seq_t)9   * cache[i-8];
            cache[i] = add - sub;
        }
    }

    return cache[(size_t)n];
}
