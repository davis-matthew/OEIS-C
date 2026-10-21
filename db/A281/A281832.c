/*
 * A281832  Number of n X 3 0..1 arrays with no element equal to more than
 *          four of its king-move neighbors and with new values introduced in
 *          order 0 sequentially upwards.
 * https://oeis.org/A281832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 4*a(n-2) + 6*a(n-3) - 23*a(n-4) * - 25*a(n-5)
 *            - 15*a(n-6) - 28*a(n-7) for n > 8 * Initial values: a(1)=4,
 *            a(2)=31, a(3)=163, a(4)=926, a(5)=5315, * a(6)=30387,
 *            a(7)=174121, a(8)=998069
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* exported sequence function */
seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated size */
    static idx_t  computed = 0;       /* highest index with a value */

    /* ensure cache is big enough */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – return 0 (should never happen) */
            if (cache) free(cache);
            cache = NULL;
            cache_cap = 0;
            computed = 0;
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    if (computed < n) {
        /* initial values for indices 1..8 */
        static const seq_t init_vals[9] = {
            0, /* unused 0th index */
            4, 31, 163, 926, 5315,
            30387, 174121, 998069
        };
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i <= 8u) {
                cache[i] = init_vals[(size_t)i];
            } else {
                /* recurrence */
                seq_t pos = 5 * cache[i-1] + 4 * cache[i-2] + 6 * cache[i-3];
                seq_t neg = 23 * cache[i-4] + 25 * cache[i-5]
                            + 15 * cache[i-6] + 28 * cache[i-7];
                cache[i] = pos - neg;  /* guaranteed non‑negative for n>8 */
            }
        }
        computed = n;
    }

    return (n < 1u) ? (seq_t)0 : cache[n];
}
