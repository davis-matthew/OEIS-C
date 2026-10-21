/*
 * A247121  Number of tilings of a 5 X 2n rectangle using 2n pentominoes of
 *          shapes P, U.
 * https://oeis.org/A247121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=12, a(3)=56; for n≥4:
 *            a(n)=2*a(n-1)+8*a(n-2)+20*a(n-3)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated size of cache */
    static size_t last = 3;     /* index of last computed term in cache */
    static const seq_t base[4] = {
        (seq_t)1, (seq_t)2, (seq_t)12, (seq_t)56
    };

    /* trivial case: n < 4 */
    if ((idx_t)n < 4)
        return base[(size_t)n];

    /* initialise cache on first call */
    if (!cache) {
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        if (!cache) abort();               /* allocation failure */
        for (size_t i = 0; i < 4; ++i)
            cache[i] = base[i];
        cap = 4;
        last = 3;
    }

    /* enlarge cache if needed */
    if ((size_t)n + 1 > cap) {
        seq_t *tmp = (seq_t*)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) abort();                 /* reallocation failure */
        cache = tmp;
        cap = (size_t)n + 1;
    }

    /* compute missing terms up to n */
    if ((size_t)n > last) {
        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            cache[i] =
                cache[i - 1] * (seq_t)2
                + cache[i - 2] * (seq_t)8
                + cache[i - 3] * (seq_t)20;
        }
        last = (size_t)n;
    }

    return cache[(size_t)n];
}
