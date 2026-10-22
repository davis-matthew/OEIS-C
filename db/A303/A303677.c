/*
 * A303677  Number of nX2 0..1 arrays with every element unequal to 0, 1 or 3
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-3) - 2*a(n-4) for n > 5; initial values:
 *            a(1) = 2, a(2) = 5, a(3) = 7, a(4) = 17, a(5) = 31
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* trivial guard for n == 0, though not part of the sequence */
    if (n == 0) return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;    /* 1‑based indices, cache[0] unused */
    static size_t computed = 0;    /* largest n for which cache[n] is valid */
    static size_t capacity  = 0;   /* allocated size of cache   */

    /* Helper: reallocate cache to hold at least 'need' entries (1‑based) */
    if (capacity < n) {
        size_t new_cap = (capacity == 0) ? (n > 5 ? n : 5) : capacity * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise base cases if not yet done */
    if (computed < 5) {
        /* Ensure we have space for indices up to 5 */
        if (capacity < 5) {
            seq_t *tmp = (seq_t *)realloc(cache, 6 * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            capacity = 5;
        }
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)17;
        cache[5] = (seq_t)31;
        computed = 5;
    }

    /* Compute up to n if necessary */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i <= 5) continue;  /* already set */
            /* a(i) = a(i-1) + 4*a(i-3) - 2*a(i-4) */
            seq_t term = cache[i - 1] + (seq_t)4 * cache[i - 3]
                         - (seq_t)2 * cache[i - 4];
            cache[i] = term;
        }
        computed = n;
    }

    return cache[n];
}
