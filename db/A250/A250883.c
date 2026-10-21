/*
 * A250883  Number of (6+1) X (n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) for n>4
 *            a(1)=155080, a(2)=553736, a(3)=1350002, a(4)=2681528
 * Range:     u8 none, u16 none, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* initialise cache with first four terms */
    if (cache == NULL) {
        size_t init_cap = 5;          /* indices 0..4 */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        cache[0] = 0; /* unused */
        cache[1] = 155080;
        cache[2] = 553736;
        cache[3] = 1350002;
        cache[4] = 2681528;
        cache_size = 4;
    }

    /* extend cache if needed */
    if (n > cache_size) {
        size_t new_len = (size_t)n + 1;                 /* need indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;  /* realloc failed */
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* a(i) = 4*a(i-1) - 6*a(i-2) + 4*a(i-3) - a(i-4) */
            seq_t t1 = 4 * cache[i - 1];
            seq_t t2 = 4 * cache[i - 3];
            seq_t t3 = 6 * cache[i - 2];
            seq_t t4 = cache[i - 4];

            /* to avoid unsigned underflow, compute as (t1 + t2) - (t3 + t4) */
            seq_t sum1 = t1 + t2;
            seq_t sum2 = t3 + t4;
            cache[i] = sum1 - sum2;
        }
        cache_size = n;
    }

    return cache[n];
}
