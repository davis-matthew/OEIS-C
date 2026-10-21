/*
 * A287828  Number of sequences over the alphabet {0,1,...,9} such that no two
 *          consecutive terms have distance 4.
 * https://oeis.org/A287828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 14*a(n-3) for n > 3, a(0) = 1, a(1) = 10, a(2)
 *            = 88, a(3) = 776
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure cache is initialised with the first four values */
    if (cache_len == 0) {
        cache = seq_alloc_new_array(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)88;
        cache[3] = (seq_t)776;
        cache_len = 4;
    }

    /* If n is already computed, return it directly */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to accommodate n+1 elements */
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = seq_alloc_new_array(new_len);

    /* Copy existing values */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* Compute the sequence up to n using the recurrence */
    seq_t nine   = (seq_t)9;
    seq_t fourteen = (seq_t)14;

    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 9*a(i-1) – 14*a(i-3) */
        new_cache[i] = nine * new_cache[i - 1] - fourteen * new_cache[i - 3];
    }

    /* Replace old cache */
    seq_free_array(cache);
    cache      = new_cache;
    cache_len  = (idx_t)new_len;

    return cache[n];
}
