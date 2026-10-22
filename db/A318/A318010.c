/*
 * A318010  Number of nX2 0..1 arrays with every element unequal to 0, 1, 2 or
 *          3 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + a(n-3) - a(n-4); initial values:
 *            a(1)=2, a(2)=8, a(3)=28, a(4)=97
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static memoization table */
    static seq_t *cache = NULL;
    static size_t capacity = 0;       /* current allocated size (indices 0..capacity-1) */
    static idx_t last_computed = 0;   /* largest index whose value is stored */

    if (n < 1) return (seq_t)0;               /* out of range */

    /* make sure we have at least space for indices up to n */
    if (capacity < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        /* zero the new part (unnecessary but safe) */
        if (new_cap > capacity) {
            size_t i;
            for (i = capacity; i < new_cap; ++i) tmp[i] = (seq_t)0;
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* make sure we have the base values stored (indices 1..4) */
    if (last_computed < 4) {
        /* ensure we have room for up to index 4 */
        if (capacity < 5) {
            size_t new_cap = 5;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            if (new_cap > capacity) {
                size_t i;
                for (i = capacity; i < new_cap; ++i) tmp[i] = (seq_t)0;
            }
            cache = tmp;
            capacity = new_cap;
        }
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)28;
        cache[4] = (seq_t)97;
        last_computed = 4;
    }

    if (n <= last_computed) return cache[n];

    /* build up the sequence up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        cache[i] = (seq_t)4 * a1 - (seq_t)2 * a2 + a3 - a4;
    }
    last_computed = n;

    return cache[n];
}
