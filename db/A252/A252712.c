/*
 * A252712  Number of (n+2) X (1+2) 0..3 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 6*a(n-3) - 7*a(n-4) - 7*a(n-5) * -
 *            5*a(n-6) + 12*a(n-7) + 12*a(n-8) - 12*a(n-9) for n>11 * Initial
 *            values (offset 1): * a(1)=39, a(2)=45, a(3)=54, a(4)=84,
 *            a(5)=111, a(6)=165, * a(7)=257, a(8)=376, a(9)=560, a(10)=930,
 *            a(11)=1365
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 43, u64 n <= 91, u128 n <= 187,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Handle indices below the offset – behaviour arbitrary. */
    if (n < 1) return 0;

    static seq_t *cache = NULL;          /* array of computed terms, 1‑based */
    static size_t alloc_cap = 0;         /* allocated array size (bytes) */
    static idx_t max_computed = 0;       /* highest index that has been computed */

    /* Initialise on first call (store terms 1..11). */
    if (cache == NULL) {
        alloc_cap = 12;                  /* capacity for indices 1..11 */
        cache = (seq_t *)malloc(alloc_cap * sizeof(seq_t));
        if (cache == NULL) abort();      /* allocation failure */
        max_computed = 11;
        cache[1] = 39;   cache[2] = 45;   cache[3] = 54;   cache[4] = 84;
        cache[5] = 111;  cache[6] = 165;  cache[7] = 257;  cache[8] = 376;
        cache[9] = 560;  cache[10] = 930; cache[11] = 1365;
    }

    /* Grow the array if necessary. */
    if ((idx_t)alloc_cap <= n) {
        /* Need at least indices 1..n, i.e. allocate n+1 slots. */
        ensure_capacity((size_t)n + 1, &alloc_cap, (void **)&cache, sizeof(seq_t));
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 11) {
            /* Should not happen because max_computed starts at 11, but guard anyway. */
            continue;
        }
        seq_t t = cache[i - 1] + cache[i - 2]
                  + (seq_t)6 * cache[i - 3]
                  - (seq_t)7 * cache[i - 4]
                  - (seq_t)7 * cache[i - 5]
                  - (seq_t)5 * cache[i - 6]
                  + (seq_t)12 * cache[i - 7]
                  + (seq_t)12 * cache[i - 8]
                  - (seq_t)12 * cache[i - 9];
        cache[i] = t;
    }

    max_computed = (max_computed > n) ? max_computed : n;
    return cache[n];
}
