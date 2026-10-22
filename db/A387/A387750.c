/*
 * A387750  a(n) = (1/7) * Sum_{k=0..n-1} binomial(7*n,7*k+3).
 * https://oeis.org/A387750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 71*a(n-1) + 7585*a(n-2) - 36991*a(n-3) - 128*a(n-4)
 *            Initial values: a(0)=0, a(1)=5, a(2)=195, a(3)=51433 This
 *            recurrence is equivalent to a(n) = (1/7)*∑_{k=0}^{n-1}
 *            binomial(7n,7k+3)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached values */
    static size_t cache_capacity = 0;/* allocated size */
    static idx_t computed_up_to = 0; /* next index to compute (0..computed_up_to-1 are valid) */

    /* initialization: allocate and set the first four terms */
    if (cache == NULL) {
        size_t init = 4;                          /* we will store indices 0..3 */
        cache = (seq_t *)calloc(init, sizeof(seq_t));
        if (cache == NULL) abort();
        cache_capacity = init;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)195;
        cache[3] = (seq_t)51433;
        computed_up_to = 4;                       /* indices 0..3 are valid */
    }

    if (n < (idx_t)computed_up_to)              /* already computed */
        return cache[n];

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_capacity) {
        size_t new_cap = cache_capacity;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = computed_up_to; i <= n; ++i) {
        /* cache[i] = 71*cache[i-1] + 7585*cache[i-2]
                     - 36991*cache[i-3] - 128*cache[i-4] */
        seq_t term = (seq_t)71 * cache[i-1]
                   + (seq_t)7585 * cache[i-2]
                   - (seq_t)36991 * cache[i-3]
                   - (seq_t)128 * cache[i-4];
        cache[i] = term;
    }
    computed_up_to = n + 1;
    return cache[n];
}
