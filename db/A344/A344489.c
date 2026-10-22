/*
 * A344489  a(n) = 1 + Sum_{k=0..n-2} binomial(n-1,k) * a(k).
 * https://oeis.org/A344489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-2} binomial(n-1,k) * a(k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache          = NULL; /* array of computed values          */
    static size_t cache_cap      = 0;    /* current allocated capacity        */
    static idx_t  last_computed  = 0;    /* highest index already computed    */

    /* Allocate or grow the cache on the first call or when needed. */
    if (cache == NULL) {
        size_t initial_cap = (size_t)(n + 1);
        cache = seq_alloc(initial_cap);
        if (!cache) return (seq_t)0;          /* allocation failure            */
        cache_cap = initial_cap;
    } else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) return (seq_t)0;            /* allocation failure            */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first two terms if not already done. */
    if (last_computed == 0) {
        cache[0] = (seq_t)1;
        if (n == 0) return cache[0];
        last_computed = 0;
    }
    if (last_computed == 1 || (last_computed == 0 && n >= 1)) {
        cache[1] = (seq_t)1;
        if (n == 1) {
            last_computed = 1;
            return cache[1];
        }
        last_computed = 1;
    }

    /* Compute terms up to n in increasing order. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1; /* C(i-1,0) */

        for (idx_t k = 0; k <= i - 2; ++k) {
            sum += bin * cache[k];
            if (k < i - 2) {                 /* prepare next binomial */
                bin = bin * (seq_t)(i - 1 - k);
                bin = bin / (seq_t)(k + 1);
            }
        }
        cache[i] = sum + (seq_t)1;
    }
    last_computed = n;
    return cache[n];
}
