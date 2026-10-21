/*
 * A296690  a(1) = a(2) = a(3) = 2, a(4) = 4, a(5) = a(6) = 5; a(n) =
 *          a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 6.
 * https://oeis.org/A296690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1) = a(2) = a(3) = 2, a(4) = 4, a(5) = a(6) = 5; a(n) =
 *            a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 6
 * Range:     u8 n <= 325, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cap = 0;        /* current allocation size */
    static idx_t computed = 0;   /* largest n already computed */

    /* initial allocation */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* grow cache if needed */
    while (cap < n) {
        cap *= 2;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* compute missing terms in order up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1 || i == 2 || i == 3) {
            cache[i - 1] = 2;
        } else if (i == 4) {
            cache[i - 1] = 4;
        } else if (i == 5 || i == 6) {
            cache[i - 1] = 5;
        } else {
            /* indices for recursive accesses */
            idx_t idx1 = i - (idx_t)cache[i - 2]; /* a(i-1) */
            idx_t idx2 = i - (idx_t)cache[i - 3]; /* a(i-2) */
            idx_t idx3 = i - (idx_t)cache[i - 4]; /* a(i-3) */

            /* all idxX are >= 1, so subtract 1 for 0‑based array */
            cache[i - 1] = cache[idx1 - 1] + cache[idx2 - 1] + cache[idx3 - 1];
        }
    }

    computed = n;
    return cache[n - 1];
}
