/*
 * A377368  a(n) = A255978(n) + 1.
 * https://oeis.org/A377368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4) - a(n-5) for n > 4
 *            Initial values: a(0)=1, a(1)=2, a(2)=2, a(3)=5, a(4)=6
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, NULL */

/* Compute the n-th term of sequence A377368.  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored terms */
    static idx_t  max_n = (idx_t)-1;   /* highest index computed, -1 means not yet initialised */
    static size_t cap   = 0;        /* allocated capacity in slots */

    /* Initialise cache if first call.  */
    if (cache == NULL) {
        size_t init_cap = 5;                /* at least 5 elements for the first 5 terms */
        cache = (seq_t *)malloc(sizeof(seq_t) * init_cap);
        if (!cache) return 0;               /* allocation failure – return 0 as sentinel */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 2;
        cache[3] = 5;
        cache[4] = 6;
        cap   = init_cap;
        max_n = 4;
    }

    /* Grow the cache if needed.  */
    size_t needed = (size_t)n + 1;          /* we need indices 0 … n inclusive */
    if (needed > cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap < needed) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;           /* allocation failure – return 0 as sentinel */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms using the recurrence.  */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                         - cache[i-4] - cache[i-5];
        }
        max_n = n;
    }

    return cache[n];
}
