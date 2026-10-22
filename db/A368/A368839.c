/*
 * A368839  a(n) = n! * (n+1)! * (n+2)! * Sum_{k=0..n} 1/(k! * (k+1)! *
 *          (k+2)!).
 * https://oeis.org/A368839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*a(n-1) + 1; a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t* cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Extend the cache if needed */
    if ((size_t)(n + 1) > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t* new_cache = seq_alloc_new_array(new_len);

        /* Copy previously computed terms */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* Free old cache if it existed */
        if (cache)
            seq_free_array(cache);

        /* Compute missing terms using the recurrence */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            size_t ui = (size_t)i;
            if (i == 0) {
                new_cache[ui] = 1;
            } else {
                seq_t prod = (seq_t)i;
                prod *= (seq_t)(i + 1);
                prod *= (seq_t)(i + 2);
                prod *= new_cache[ui - 1];
                prod += 1;
                new_cache[ui] = prod;
            }
        }

        /* Install new cache */
        cache     = new_cache;
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
