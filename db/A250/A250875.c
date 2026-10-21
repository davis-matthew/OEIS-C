/*
 * A250875  Number of (n+1) X (6+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 35*a(n-2) + 50*a(n-3) - 24*a(n-4) for n≥5
 *            Initial values: a(1)=3680, a(2)=18520, a(3)=88212, a(4)=400496
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 26, u128 n <= 58,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed_max = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          // undefined offset, return 0 safely

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache_size = 5;                   /* indices 0..4 (0 unused) */
        cache = alloc_seq_array(cache_size);
        cache[1] = (seq_t)3680;
        cache[2] = (seq_t)18520;
        cache[3] = (seq_t)88212;
        cache[4] = (seq_t)400496;
        computed_max = 4;
    }

    /* Grow cache if needed to reach requested n */
    if (n > computed_max) {
        if (n >= cache_size) {
            size_t new_size = cache_size * 2;
            if (new_size <= (size_t)n) new_size = (size_t)n + 1;
            seq_t *new_cache = alloc_seq_array(new_size);
            for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
            /* old cache is kept to avoid freeing; not needed by spec */
            cache = new_cache;
            cache_size = new_size;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t t1 = cache[i-1] * (seq_t)10;
            seq_t t2 = cache[i-2] * (seq_t)35;
            seq_t t3 = cache[i-3] * (seq_t)50;
            seq_t t4 = cache[i-4] * (seq_t)24;
            cache[i] = t1 - t2 + t3 - t4;
        }
        computed_max = n;
    }

    return cache[n];
}
