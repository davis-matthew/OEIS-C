/*
 * A373770  Expansion of e.g.f. exp(x^2 / (2 * (1 - x))) / (1 - x).
 * https://oeis.org/A373770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   2*a(n) = 2*(2*n-1)*a(n-1) - 2*(n-2)*(n-1)*a(n-2) - (n-2)*(n-1)*a(n-3)
   Initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=12 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc */

/* Grow the cache so it can hold indices up to n. */
static void grow_cache(idx_t n, seq_t **arr, size_t *cap)
{
    size_t mincap = (size_t)n + 1;
    if (*cap >= mincap)
        return;                       /* already big enough */

    if (*arr == NULL) {
        *arr = (seq_t *)malloc(mincap * sizeof(seq_t));
    } else {
        *arr = (seq_t *)realloc(*arr, mincap * sizeof(seq_t));
    }
    *cap = mincap;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* Cached values. */
    static size_t cap   = 0;      /* Current cache capacity. */
    static idx_t  next  = 0;      /* Next index to compute. */

    /* Make sure the cache is large enough. */
    grow_cache(n, &cache, &cap);

    /* Compute missing values up to n. */
    while (next <= n) {
        switch (next) {
            case 0: cache[0] = 1; break;
            case 1: cache[1] = 1; break;
            case 2: cache[2] = 3; break;
            case 3: cache[3] = 12; break;
            default: {
                idx_t i = next;
                seq_t part1 = ((seq_t)2 * (seq_t)(2 * i - 1)) * cache[i - 1];
                seq_t part2 = ((seq_t)2 * (seq_t)(i - 2) * (seq_t)(i - 1))
                               * cache[i - 2];
                seq_t part3 = ((seq_t)(i - 2) * (seq_t)(i - 1)) * cache[i - 3];
                cache[i] = (part1 - part2 - part3) / 2;
            }
        }
        ++next;
    }

    return cache[n];
}
