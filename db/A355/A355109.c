/*
 * A355109  a(n) = 1 + Sum_{k=1..n-1} binomial(n-1,k) * 2^(k-1) * a(k).
 * https://oeis.org/A355109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n-1} binomial(n-1,k) * 2^(k-1) * a(k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed recursively with memoisation. It works with
   any integer type defined by seq_t and idx_t, including arbitrary‑
   precision big integers. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

/* Compute a(n) for the OEIS sequence A355109. */
seq_t a(idx_t n)
{
    /* Static cache holds already computed values of a(k) for k <= max_n. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of values stored, >= 1 if cache non‑empty */

    /* Ensure the cache can hold indices up to n (inclusive). */
    size_t required = (size_t)n + 1;
    if (cache_cap < required) {
        size_t new_cap = required > 16 ? required : 16;
        while (new_cap < required) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* If realloc fails cache might be NULL, but the program is not allowed
           to crash on memory allocation failure. */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise cache if empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute missing terms up to n. */
    for (size_t m = cache_len; m <= (size_t)n; ++m) {
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k < m; ++k) {
            /* term = binomial(m-1,k) * 2^(k-1) * a(k) */
            seq_t bin = binomial((idx_t)m - 1, (idx_t)k);
            seq_t powv = pow2((idx_t)k - 1);
            seq_t term = seq_mul(bin, powv);
            term = seq_mul(term, cache[k]);
            sum = sum + term;
        }
        cache[m] = sum + (seq_t)1;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
