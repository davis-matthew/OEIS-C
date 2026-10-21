/*
 * A253455  Number of (n+1) X (7+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) for n>9. * Initial
 *            values: * a(1)=48293, a(2)=74324, a(3)=89556, a(4)=114717, *
 *            a(5)=170184, a(6)=285681, a(7)=526430, a(8)=1036512,
 *            a(9)=2142374. * * The recurrence is implemented as a(n) =
 *            6*a(n-1)+6*a(n-3)-11*a(n-2) * to avoid underflow when seq_t is
 *            an u...
 * Range:     u8 none, u16 n <= 1, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base values for n = 1..9 (index 0 unused). */
    static const seq_t base[10] = {
        0,      /* placeholder for index 0 */
        48293,  74324,  89556,  114717,
        170184, 285681, 526430, 1036512, 2142374
    };

    if (n <= 6)
        return base[n];

    /* Required for recurrence n >= 7 (but we need base up to 9). */
    if (n <= 9)
        return base[n];

    /* Cached array of computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_cap  = 0;   /* allocated size (indices 0..cache_cap-1) */
    static idx_t cache_last = 0;   /* highest index that has been computed */

    /* Allocate or grow cache if needed. */
    if (!cache || cache_cap <= n) {
        idx_t new_cap = n + 1; /* reserve indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort(); /* memory allocation failure */

        /* If cache was newly allocated, copy base values. */
        if (!cache) {
            for (idx_t i = 1; i <= 9 && i <= n; ++i)
                new_cache[i] = base[i];
        } else {
            /* reallocation preserves existing contents; but ensure
               base values are present up to index 9. */
            for (idx_t i = 1; i <= 9 && i <= n; ++i)
                new_cache[i] = base[i];
        }

        cache     = new_cache;
        cache_cap = new_cap;
        if (cache_last < 9)
            cache_last = 9;
    }

    /* Compute values from cache_last+1 up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t t1 = 6 * cache[i - 1];
        seq_t t2 = 6 * cache[i - 3];
        seq_t t3 = 11 * cache[i - 2];
        cache[i] = t1 + t2 - t3; /* guaranteed non‑negative */
    }
    cache_last = n;
    return cache[n];
}
