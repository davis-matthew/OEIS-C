/*
 * A253092  Log_3(A133579(n)).
 * https://oeis.org/A253092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1510, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   valuation_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A253092 (Log_3(A133579(n))):
   a(0) = 0
   a(1) = 0
   For n >= 2:
     if n % 3 == 0   : a(n) = a(n-1) - valuation_3(n)
     else             : a(n) = a(n-1) + 1
   where valuation_3(n) is the exponent of 3 in the prime factorization of n. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/valuation_3.h"

seq_t a(idx_t n)
{
    /* Static cache of already computed values (memoisation).  */
    static seq_t *cache = NULL;          /* array of computed values   */
    static size_t cache_cap = 0;         /* allocated capacity         */
    static size_t cache_size = 0;        /* number of valid entries    */

    /* Initialise cache on first call.  */
    if (cache_size == 0) {
        cache_cap = 2; /* need entries for a(0) and a(1) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out of memory → 0 */
        cache[0] = (seq_t)0;              /* a(0) = 0                */
        cache[1] = (seq_t)0;              /* a(1) = 0                */
        cache_size = 2;                   /* a(0) and a(1) computed */
    }

    /* Ensure cache large enough for index n.  */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* out of memory → 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return value if already computed.  */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Compute missing terms up to n.  */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[(size_t)(i - 1)];
        if (i % 3 == 0) {
            size_t v = valuation_3(i);
            cache[(size_t)i] = prev - (seq_t)v;
        } else {
            cache[(size_t)i] = prev + (seq_t)1;
        }
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
