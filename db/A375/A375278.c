/*
 * A375278  Expansion of 1/((1 - x - x^3)^2 - 4*x^4).
 * https://oeis.org/A375278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) + 2*a(n-4) - a(n-6), with
 *            initial values a(0)=1, a(1)=2, a(2)=3, a(3)=6, a(4)=15, a(5)=34
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_index = (idx_t)-1;

    /* first time: allocate and initialise the first 6 terms */
    if (!cache) {
        cap = 6;                    /* indices 0 .. 5 */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) abort();        /* safety */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)15;
        cache[5] = (seq_t)34;
        max_index = 5;
    }

    /* if the requested index is already computed, return it */
    if (n <= (idx_t)max_index)
        return cache[n];

    /* grow the cache if necessary */
    size_t required = (size_t)n + 1;
    if (required > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < required) new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = max_index + 1; i <= n; ++i) {
        cache[i] = (seq_t)(
            (seq_t)2 * cache[i-1]
          - cache[i-2]
          + (seq_t)2 * cache[i-3]
          + (seq_t)2 * cache[i-4]
          - cache[i-6] );
    }
    max_index = n;
    return cache[n];
}
