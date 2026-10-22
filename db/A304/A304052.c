/*
 * A304052  Number of nX2 0..1 arrays with every element unequal to 1, 2, 4 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+3*a(n-2)+8*a(n-3)-4*a(n-4)-16*a(n-5) (n>6),
 *            a(1)=1,a(2)=3,a(3)=11,a(4)=10,a(5)=51,a(6)=165
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t computed = 0;

    /* initial values: indices 1..6 */
    static const seq_t init_vals[7] = {
        (seq_t)0,    /* unused (index 0) */
        (seq_t)1,    /* a(1) */
        (seq_t)3,    /* a(2) */
        (seq_t)11,   /* a(3) */
        (seq_t)10,   /* a(4) */
        (seq_t)51,   /* a(5) */
        (seq_t)165   /* a(6) */
    };

    if (n <= 6u)                /* offset 1 */
        return init_vals[(size_t)n];

    /* Ensure cache large enough for indices up to n */
    if (cap <= (size_t)n) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            abort();          /* memory allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise the first six terms once */
    if (computed < 6) {
        for (size_t i = 1; i <= 6; ++i)
            cache[i] = init_vals[i];
        computed = 6;
    }

    /* Compute missing values up to n using the recurrence */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        seq_t term = cache[i-1]                     /* a(i-1) */
                   + cache[i-2] * (seq_t)3           /* +3*a(i-2) */
                   + cache[i-3] * (seq_t)8           /* +8*a(i-3) */
                   - cache[i-4] * (seq_t)4           /* -4*a(i-4) */
                   - cache[i-5] * (seq_t)16;         /* -16*a(i-5) */
        cache[i] = term;
    }
    computed = (size_t)n;

    return cache[(size_t)n];
}
