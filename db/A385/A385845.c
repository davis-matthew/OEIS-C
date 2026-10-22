/*
 * A385845  G.f. A(x) satisfies A(x) = 1/((1 - x) * (1 - x^4*A'''(x))).
 * https://oeis.org/A385845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=0}^{n-1} (2k - 3k² + k³) * a(k) * a(n-1-k) ;
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   alloc_seq_array, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* current capacity of the cache array        */
    static bool init    = false;/* whether initial values have been set       */
    static idx_t max_computed = 0; /* highest index that has been computed */

    /* Ensure the cache can hold indices up to n.  */
    ensure_cache(n, &cache, &cap);

    /* Initialize the first four terms only once.  */
    if (!init)
    {
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        max_computed = 3;
        init = true;
    }

    /* If the requested term is already known, return it.  */
    if (n <= max_computed)
        return cache[n];

    /* Compute the missing terms up to n, in increasing order.  */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t sum = 0;

        for (idx_t k = 0; k <= i - 1; ++k)
        {
            /* coefficient = 2k - 3k² + k³ */
            seq_t coeff = (seq_t)( (seq_t)2 * k
                                  - (seq_t)3 * k * k
                                  + (seq_t)k * k * k );

            seq_t prod = cache[k] * cache[(i - 1) - k];
            prod *= coeff;
            sum  += prod;
        }

        cache[i] = sum + 1;
    }

    max_computed = n;
    return cache[n];
}
