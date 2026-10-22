/*
 * A301687  Partial sums of A301686.
 * https://oeis.org/A301687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) - 2*a(n-4) - a(n-6) + a(n-7) (valid for
 *            n > 6) Initial values: a(0)=1, a(1)=6, a(2)=13, a(3)=27,
 *            a(4)=44, a(5)=63, a(6)=91
 * Range:     u8 n <= 10, u16 n <= 175, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = *cap ? (*cap << 1) : 8;
        if (new_cap < needed)
            new_cap = needed;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t count = 0;   /* Number of values computed (cache[0..count-1] are valid) */

    static const seq_t init[7] = { (seq_t)1, (seq_t)6, (seq_t)13,
                                   (seq_t)27, (seq_t)44, (seq_t)63,
                                   (seq_t)91 };

    /* Return directly for the first seven terms */
    if (n < 7) return init[n];

    /* Initialize the cache if this is the first call */
    if (cache == NULL) {
        ensure_capacity(7, &cap, (void **)&cache, sizeof(seq_t));
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
        count = 7;    /* we have values for indices 0..6 */
    }

    /* Grow the allocation if needed */
    if ((size_t)(n + 1) > cap)
        ensure_capacity((size_t)(n + 1), &cap, (void **)&cache, sizeof(seq_t));

    /* Compute missing terms up to index n */
    for (size_t i = count; i <= (size_t)n; ++i) {
        /* recurrence:
           a(i) = a(i-1) + 2*a(i-3) - 2*a(i-4) - a(i-6) + a(i-7)
           Reordered to avoid intermediate underflow:
           a(i) = (a(i-1) + 2*a(i-3) + a(i-7)) - (2*a(i-4) + a(i-6))
        */
        seq_t part1 = cache[i-1] + (cache[i-3] << 1) + cache[i-7];
        seq_t part2 = (cache[i-4] << 1) + cache[i-6];
        cache[i] = part1 - part2;
    }

    count = (size_t)n + 1;
    return cache[(size_t)n];
}
