/*
 * A255630  Number of n-length ternary words avoiding runs of zeros of length
 *          1 (mod 3).
 * https://oeis.org/A255630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 2*a(n+2) + 3*a(n), a(0) = 1, a(1) = 2, a(2) = 5
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;       /* allocated size of cache array      */
    static size_t computed = 0;       /* number of terms that are already
                                        * computed and stored in cache    */

    /* Ensure the array is large enough for indices 0..n. */
    size_t need = (size_t)n + 1;
    if (capacity < need)
        ensure_cache((idx_t)need, &cache, &capacity);

    /* Initialise the first three values if they have not been set yet. */
    size_t base_end = (need < 3 ? need : 3);   /* number of base terms to set   */
    for (size_t i = computed; i < base_end; ++i) {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = (seq_t)2;
        else
            cache[2] = (seq_t)5;
    }
    if (computed < base_end)
        computed = base_end;

    /* If the requested n is already computed, just return it. */
    if ((size_t)n < computed)
        return cache[(size_t)n];

    /* Compute remaining terms up to n using the recurrence. */
    size_t start = computed;
    if (start < 3)
        start = 3;
    for (size_t i = start; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)2 * cache[i - 1]
                 + (seq_t)3 * cache[i - 3];
    }

    /* Update the count of computed terms. */
    computed = need;
    return cache[(size_t)n];
}
