/*
 * A281474  Number of 6 X n 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A281474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 43, u64 n <= 95, u128 n <= 198,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-6) - a(n-7)
 * for n > 12, with initial values: 
 * a(1)=32, a(2)=857, a(3)=283, a(4)=405, a(5)=534, a(6)=706,
 * a(7)=924, a(8)=1190, a(9)=1602, a(10)=2290, a(11)=3374, a(12)=5036.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] unused, cache[1] = a(1) … */
static idx_t  cache_len = 0;     /* number of valid entries in cache (>= cache_len) */

seq_t a(idx_t n)
{
    /* Allocate initial cache if needed */
    if (cache == NULL)
    {
        /* Need indices 0..12 inclusive */
        size_t init_len = 13;
        cache = (seq_t*)malloc(init_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        cache_len = (idx_t)init_len;

        /* Set initial values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)32;
        cache[2] = (seq_t)857;
        cache[3] = (seq_t)283;
        cache[4] = (seq_t)405;
        cache[5] = (seq_t)534;
        cache[6] = (seq_t)706;
        cache[7] = (seq_t)924;
        cache[8] = (seq_t)1190;
        cache[9] = (seq_t)1602;
        cache[10] = (seq_t)2290;
        cache[11] = (seq_t)3374;
        cache[12] = (seq_t)5036;
    }

    /* Extend cache up to n if necessary */
    if (n >= cache_len)
    {
        size_t old_len = (size_t)cache_len;
        size_t new_len = (size_t)n + 1;  /* need indices 0..n inclusive */

        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        cache_len = (idx_t)new_len;

        /* Compute missing terms */
        for (size_t i = old_len; i <= (size_t)n; ++i)
        {
            if (i <= 12) continue;  /* initial values already set */

            cache[i] = 2 * cache[i-1] - cache[i-2] + cache[i-3]
                     - cache[i-4] + cache[i-6] - cache[i-7];
        }
    }

    return cache[(size_t)n];
}
