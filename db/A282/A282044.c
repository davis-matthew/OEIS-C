/*
 * A282044  Reduced Kronecker coefficients for the case a=2, b=3, i=4.
 * https://oeis.org/A282044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-3) - 3*a(n-5) + 4*a(n-6) + (n==4||n==6?1:0),
   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=2. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: computed values up to cache_size-1 */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;   /* number of elements stored (next index to compute) */
    static size_t cache_cap  = 0;   /* current capacity of cache */

    /* Ensure capacity for index n */
    size_t idx = (size_t)n;
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 8;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure – unlikely in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first six values if cache is empty */
    if (cache_size == 0) {
        /* a(0) … a(3) are zero */
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)0;
        cache[4] = (seq_t)1;            /* a(4) = 1 */
        cache[5] = (seq_t)2;            /* a(5) = 2 */
        cache_size = 6;
    }

    /* Compute terms up to n */
    while (cache_size <= idx) {
        idx_t k = (idx_t)cache_size;
        seq_t val;

        if (k < 6) {
            /* The first six values are already set */
            val = cache[k];
        } else {
            /* Apply the recurrence */
            val = 2 * cache[k-1];
            val -= cache[k-3];
            val -= 3 * cache[k-5];
            val += 4 * cache[k-6];
            if (k == 4 || k == 6) val += (seq_t)1;
        }

        cache[cache_size++] = val;
    }

    return cache[idx];
}
