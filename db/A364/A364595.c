/*
 * A364595  G.f. satisfies A(x) = 1/(1-x) + x^3*(1-x)*A(x)^3.
 * https://oeis.org/A364595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n-2k, k) * binomial(3k, k)
 *            / (2k+1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 30, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static bool *cached = NULL;
    static idx_t cache_size = 0;

    /* Allocate cache entries if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) abort();
        bool *ctmp = (bool *)realloc(cached, (size_t)new_size * sizeof(bool));
        if (!ctmp) abort();

        /* Mark new entries as not cached */
        for (idx_t i = cache_size; i < new_size; ++i) ctmp[i] = false;

        cache   = tmp;
        cached  = ctmp;
        cache_size = new_size;
    }

    /* Return cached value if available */
    if (cached[n]) return cache[n];

    /* Compute the term via the given sum formula */
    seq_t sum = 0;
    idx_t max_k = n / 3;
    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t t1 = binomial(n - 2 * k, k);
        seq_t t2 = binomial(3 * k, k);
        seq_t term = t1 * t2;
        term = term / (seq_t)(2 * k + 1);
        sum += term;
    }

    cache[n] = sum;
    cached[n] = true;
    return sum;
}
