/*
 * A377754  Number of edge cuts in the 2n-crossed prism graph.
 * https://oeis.org/A377754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 112*a(n-1) - 3705*a(n-2) + 41870*a(n-3) - 87500*a(n-4) +
 *            37704*a(n-5) - 4608*a(n-6); initial values: a(1)=31, a(2)=3013,
 *            a(3)=230903, a(4)=15918887, a(5)=1050866239, a(6)=68122980063
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated size (>= computed index + 1) */
    static size_t computed = 0;  /* highest index whose value is known */

    if (n < 1)
        return (seq_t)0;        /* behaviour below offset is irrelevant */

    /* Ensure the cache is large enough for n and for the 6th term */
    size_t needed = (size_t)n + 1;
    if (needed < 7) needed = 7;          /* we need at least up to a(6) */
    if (needed > cap) {
        size_t new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                       /* allocation failed */
            free(cache);
            cache = NULL;
            cap   = 0;
            computed = 0;
            return (seq_t)0;
        }
        /* initialise newly allocated cells to zero */
        if (cap < new_cap)
            memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise base cases once */
    if (computed == 0) {
        cache[1] = (seq_t)31ULL;
        cache[2] = (seq_t)3013ULL;
        cache[3] = (seq_t)230903ULL;
        cache[4] = (seq_t)15918887ULL;
        cache[5] = (seq_t)1050866239ULL;
        cache[6] = (seq_t)68122980063ULL;
        computed = 6;
    }

    if (n <= computed)
        return cache[(size_t)n];

    /* compute missing terms up to n using the recurrence */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        cache[i] =
            (seq_t)112 * cache[i-1] -
            (seq_t)3705 * cache[i-2] +
            (seq_t)41870 * cache[i-3] -
            (seq_t)87500 * cache[i-4] +
            (seq_t)37704 * cache[i-5] -
            (seq_t)4608 * cache[i-6];
    }
    computed = (size_t)n;

    return cache[(size_t)n];
}
