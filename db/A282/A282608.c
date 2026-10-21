/*
 * A282608  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 489", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 16*a(n-4) - 16*a(n-6) for n > 13; Initial
 *            values: a(0)=1, a(1)=0, a(2)=7, a(3)=4, a(4)=27, a(5)=30,
 *            a(6)=115, a(7)=90, a(8)=441, a(9)=488, a(10)=1849, a(11)=1448,
 *            a(12)=7065, a(13)=7832
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = 0;
    static const seq_t init[] = {1,0,7,4,27,30,115,90,441,488,1849,1448,7065,7832};

    /* Ensure the cache is large enough */
    if (n + 1 > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        cache = new_cache;
        cache_size = n + 1;
    }

    /* Initialize the initial terms if needed */
    if (max_computed < 13) {
        idx_t limit = (n < 13) ? n : 13;
        for (idx_t i = max_computed; i <= limit; ++i) {
            cache[i] = init[i];
        }
        max_computed = limit;
    }

    /* Compute any missing terms up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* The recurrence only applies for indices > 13 */
        cache[i] = cache[i-2] + ((seq_t)16 * cache[i-4]) - ((seq_t)16 * cache[i-6]);
    }
    max_computed = n;

    return cache[n];
}
