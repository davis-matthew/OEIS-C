/*
 * A321848  E.g.f.: exp(x/(1-5*x)).
 * https://oeis.org/A321848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A321848: e.g.f. exp(x/(1-5*x))
 *  Recurrence:   a(0)=1, a(1)=1
 *                 a(n) = (10*n-9)*a(n-1) - 25*(n-2)*(n-1)*a(n-2)   for n>=2
 */

#include <stddef.h>                     // for size_t
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;   // array of computed values
static size_t cache_cap = 0;  // allocated capacity
static size_t cache_len = 0;  // number of values stored (next free index)

/* Grow the cache to at least 'n' and compute missing entries */
static void ensure_up_to(idx_t n)
{
    /* enlarge capacity if needed */
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
            seq_free_array(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            /* recurrence:
             * a(n) = (10*n-9)*a(n-1) - 25*(n-2)*(n-1)*a(n-2)
             */
            seq_t coeff1 = 10 * (seq_t)i - (seq_t)9;
            seq_t term1 = coeff1 * cache[i - 1];

            seq_t coeff2 = 25 * (seq_t)(i - 2) * (seq_t)(i - 1);
            seq_t term2 = coeff2 * cache[i - 2];

            cache[i] = term1 - term2;
        }
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[n];
}
