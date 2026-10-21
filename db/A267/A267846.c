/*
 * A267846  Binary representation of the n-th iteration of the "Rule 227"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 101*a(n-1)-100*a(n-2) Initial terms: a(0)=1, a(1)=100,
 *            a(2)=11010, a(3)=1111010
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t ready = 0; /* highest index computed */

    /* Ensure the array can hold indices up to n. */
    if (cache == NULL) {
        cap = n + 1;
        if (cap < 4) cap = 4;                     /* store first four terms */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (cache == NULL) return 0;              /* allocation failure */
        cache[0] = 1;       /* a(0) */
        cache[1] = 100;     /* a(1) */
        cache[2] = 11010;   /* a(2) */
        cache[3] = 1111010; /* a(3) */
        ready = 3;
    } else if (cap <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (tmp == NULL) return 0;                /* allocation failure */
        cache = tmp;
        cap = n + 1;
    }

    /* Compute any missing terms up to n. */
    if (ready < n) {
        for (idx_t i = ready + 1; i <= n; ++i) {
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            seq_t prod1 = prev1 * 101;           /* 101 * a(n-1) */
            seq_t prod2 = prev2 * 100;           /* 100 * a(n-2) */
            cache[i] = prod1 - prod2;           /* a(n) */
        }
        ready = n;
    }

    return cache[n];
}
