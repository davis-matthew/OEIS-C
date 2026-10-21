/*
 * A267905  Number of n X 1 0..2 arrays with every repeated value in every row
 *          unequal to the previous repeated value, and in every column equal
 *          to the previous repeated value, and new values introduced in
 *          row-major sequential order.
 * https://oeis.org/A267905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + a(n-3) + 2*a(n-4) with a(1)=1,
 *            a(2)=2, a(3)=5, a(4)=13, n ≥ 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: seq_t and idx_t are supplied by the build system. */

#include <stdlib.h>

/* External linkage function required by the test harness */
seq_t a(idx_t n)
{
    /* Guard against the offset: a(0) is undefined, return 0 */
    if (n == 0) return 0;

    /* Static cache: 1‑based indexing, cache[0] unused */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* number of allocated elements (≥ n+1) */
    static idx_t last = 0;       /* highest n already computed */

    /* Expand cache if necessary */
    if (n + 1 > capacity) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – give up */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 4) { /* initialise the base cases */
            switch (i) {
                case 1: cache[i] = 1; break;
                case 2: cache[i] = 2; break;
                case 3: cache[i] = 5; break;
                case 4: cache[i] = 13; break;
            }
        } else { /* recurrence for i ≥ 5 */
            seq_t t1 = cache[i - 1] * 5;        /* 5*a(i-1) */
            seq_t t2 = cache[i - 3];            /* a(i-3)   */
            seq_t t3 = cache[i - 4] * 2;        /* 2*a(i-4) */
            seq_t sum = t1 + t2 + t3;           /* top terms */
            seq_t sub = cache[i - 2] * 7;       /* 7*a(i-2) */
            cache[i] = sum - sub;               /* final value */
        }
    }

    last = n;
    return cache[n];
}
