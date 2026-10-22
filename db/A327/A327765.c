/*
 * A327765  a(n) is the trace of the n-th power of the 2 X 2 matrix [1 2; 3
 *          4].
 * https://oeis.org/A327765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 2*a(n-2), a(0)=2, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised recurrence calculation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed = 0;          /* number of values present in cache */

    /* Resize cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        if (new_size < 2u) new_size = 2u;
        while (new_size <= n) new_size <<= 1;   /* grow geometrically */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                       /* allocation failure */
        cache      = tmp;
        cache_size = new_size;
    }

    /* Initialise base cases if not yet done */
    if (computed == 0) {
        cache[0] = (seq_t)2;    /* a(0) */
        cache[1] = (seq_t)5;    /* a(1) */
        computed = 2;
    }

    /* If already computed, just return */
    if (n < computed) return cache[n];

    /* Compute new values up to n using the recurrence */
    for (idx_t i = computed; i <= n; ++i) {
        /* i >= 2 in this loop */
        cache[i] = (seq_t)5 * cache[i-1] + (seq_t)2 * cache[i-2];
    }

    computed = n + 1;
    return cache[n];
}
