/*
 * A273147  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 597", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)-a(n-6)-a(n-7) for
 *            n>6; a(0)=7, a(1)=12, a(2)=21, a(3)=16, a(4)=47, a(5)=12,
 *            a(6)=77
 * Range:     i8 n <= 9, i16 n <= 127, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Validated: standard, pass, 63 terms through n = 117 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Base values for indices 0..6 */
    static const seq_t base[7] = {7, 12, 21, 16, 47, 12, 77};

    /* Memoization storage */
    static seq_t *cache   = NULL;   /* array holding computed values */
    static size_t cache_size = 0;   /* number of allocated slots */
    static idx_t   next_compute = 7;/* next index to compute (>=7) */

    if (n < 7)                      /* simple lookup for the first seven terms */
        return base[n];

    /* Ensure the cache is allocated and big enough */
    if (!cache) {
        cache_size = (size_t)(n + 1);
        cache = mem_new(seq_t, cache_size);
        if (!cache) exit(1);       /* out of memory */
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = base[i];
        /* next_compute is already 7 */
    } else if ((size_t)(n + 1) > cache_size) {
        seq_t *tmp = mem_resize(seq_t, cache, (n + 1));
        if (!tmp) exit(1);         /* out of memory */
        cache = tmp;
        cache_size = (size_t)(n + 1);
    }

    /* Compute any missing terms up to n */
    for (idx_t i = next_compute; i <= n; ++i) {
        cache[i] = -cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5]
                   - cache[i-6] - cache[i-7];
    }
    next_compute = n + 1;           /* all indices up to n are now valid */

    return cache[n];
}
