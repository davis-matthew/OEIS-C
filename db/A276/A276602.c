/*
 * A276602  Values of k such that k^2 + 10 is a triangular number (A000217).
 * https://oeis.org/A276602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(1)=0, a(2)=9
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)9;

    /* Static cache for memoisation */
    static seq_t *cache      = NULL; /* array of stored values */
    static size_t cache_size = 0;    /* current allocated size */
    static idx_t last_computed = 2;  /* highest n that has been computed */
    static idx_t base_init = 0;      /* flag to init the first two values */

    /* Ensure the cache is large enough to store up to index n */
    size_t required = (size_t)n + 1; /* indices 0..n inclusive */
    if (cache_size < required) {
        size_t new_size = cache_size ? cache_size << 1 : required;
        if (new_size < required) new_size = required;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();           /* allocation error */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise the first two values once, if not already done */
    if (!base_init) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)9;
        base_init = 1;
        if (last_computed < 2) last_computed = 2;
    }

    /* Compute remaining terms up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)(6 * cache[i - 1] - cache[i - 2]);
    }
    last_computed = n;

    return cache[n];
}
