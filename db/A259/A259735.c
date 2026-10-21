/*
 * A259735  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00010101 or 01010101.
 * https://oeis.org/A259735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 3*a(n-4) + 2*a(n-6) for n>7 Initial values:
 *            a(1)=20, a(2)=22, a(3)=42, a(4)=62, a(5)=101, a(6)=152, a(7)=265
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 43, u64 n <= 91, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage, per requirement */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;         /* stored values */
    static idx_t  cache_cap = 0;        /* allocated size      */
    static idx_t  computed = 0;         /* highest index ready */

    /* initialise the first seven terms */
    if (cache_cap == 0) {
        cache_cap = 8;                  /* indices 0..7 used */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* unlikely for valid input */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)20;
        cache[2] = (seq_t)22;
        cache[3] = (seq_t)42;
        cache[4] = (seq_t)62;
        cache[5] = (seq_t)101;
        cache[6] = (seq_t)152;
        cache[7] = (seq_t)265;
        computed = 7;
    }

    /* If the requested index is already known, return it */
    if (n <= computed)
        return cache[(size_t)n];

    /* Grow the array if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* memory error – fallback */
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        if (i < 8) {
            /* All values 1..7 already set during init */
            continue;
        }
        /* a(i) = a(i-2) + 3*a(i-4) + 2*a(i-6) */
        seq_t val = 0;
        val += cache[i - 2];
        val += (cache[i - 4] * (seq_t)3);
        val += (cache[i - 6] * (seq_t)2);
        cache[i] = val;
    }
    computed = n;
    return cache[(size_t)n];
}
