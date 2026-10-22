/*
 * A316689  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2,
 *          6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of n×4 0..1 arrays with every element unequal to
 * 0, 1, 2, 6, 7 or 8 king‑move adjacent elements, with upper left
 * element zero.
 *
 * Recurrence (valid for n > 11):
 *   a(n) = a(n-1) + 2·a(n-2) + 5·a(n-3) + a(n-4)
 *          – 2·a(n-5) – 6·a(n-6) – 4·a(n-7)
 *
 * Initial values:
 *   a(1)=8, a(2)=5, a(3)=9, a(4)=23, a(5)=44, a(6)=125,
 *   a(7)=313, a(8)=773, a(9)=1964, a(10)=4948, a(11)=12456
 *
 * The function caches already computed terms so that it works in any
 * call order.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

static const seq_t base_values[12] = {
    0,      /* unused index 0 */
    8, 5, 9, 23, 44, 125, 313, 773, 1964, 4948, 12456
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* offset is 1 */

    if (n <= 11)
        return base_values[n];

    /* static cache for computed values */
    static seq_t *cache = NULL;   /* array [0..max] */
    static size_t cap = 0;        /* allocated capacity */
    static idx_t maxComputed = 0; /* largest n computed */

    /* first allocation and initialisation */
    if (cache == NULL) {
        /* start with a size that fits the first 11 values */
        cap = 32;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;     /* allocation failure (unlikely) */
        for (idx_t i = 1; i <= 11; ++i)
            cache[i] = base_values[i];
        maxComputed = 11;
    }

    /* grow the cache if necessary */
    if (n >= (idx_t)cap) {
        size_t newcap = cap ? cap * 2 : 32;
        while ((idx_t)newcap <= n)
            newcap *= 2;
        seq_t *ptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!ptr)
            return (seq_t)0;     /* allocation failure */
        cache = ptr;
        cap = newcap;
    }

    /* if already computed, return */
    if (n <= maxComputed)
        return cache[n];

    /* compute terms up to n */
    for (idx_t i = maxComputed + 1; i <= n; ++i) {
        seq_t term = cache[i - 1]
                   + (cache[i - 2] * (seq_t)2)
                   + (cache[i - 3] * (seq_t)5)
                   + cache[i - 4]
                   - (cache[i - 5] * (seq_t)2)
                   - (cache[i - 6] * (seq_t)6)
                   - (cache[i - 7] * (seq_t)4);
        cache[i] = term;
    }
    maxComputed = n;
    return cache[n];
}
