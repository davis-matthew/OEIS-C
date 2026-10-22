/*
 * A301417  Sums of positive coefficients in generalized Chebyshev polynomials
 *          of the first kind, for a family of 4 data.
 * https://oeis.org/A301417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A301417
 * Recurrence: a(n+5) = 5*a(n+4) + 2*a(n+3) – 2*a(n+2) – 3*a(n+1) – a(n),
 * with a(1)=1, a(2)=4, a(3)=19, a(4)=98, a(5)=516.
 * The function memoizes previously computed values, so calls in any order
 * are efficient. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached sequence values, 1‑indexed. */
    static seq_t *cache = NULL;      /* storage for computed terms */
    static idx_t  alloc_size = 0;    /* number of elements allocated (idx 0 unused) */
    static idx_t  computed_up_to = 0;/* largest index for which cache[i] is valid */

    /* Grow the cache if necessary. */
    if (cache == NULL) {
        alloc_size = n + 1;
        cache = (seq_t *)malloc(alloc_size * sizeof(seq_t));
    } else if (alloc_size <= n) {
        alloc_size = n + 1;
        cache = (seq_t *)realloc(cache, alloc_size * sizeof(seq_t));
    }

    /* Compute missing terms up to n. */
    if (computed_up_to < n) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            seq_t val;
            switch (i) {
                case 1: val = 1; break;
                case 2: val = 4; break;
                case 3: val = 19; break;
                case 4: val = 98; break;
                case 5: val = 516; break;
                default:
                    /* a(i) = 5*a(i-1) + 2*a(i-2) – 2*a(i-3) – 3*a(i-4) – a(i-5) */
                    val = 5 * cache[i-1] + 2 * cache[i-2]
                        - 2 * cache[i-3] - 3 * cache[i-4] - cache[i-5];
                    break;
            }
            cache[i] = val;
        }
        computed_up_to = n;
    }

    return cache[n];
}
