/*
 * A352041  a(0) = 1; a(n) = Sum_{k=0..floor(n/4)} binomial(n-3*k,k) * a(k).
 * https://oeis.org/A352041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 62, u64 n <= 121,
 *            u128 n <= 238, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A352041:  a(0)=1, a(n)=∑_{k=0..⌊n/4⌋} binomial(n‑3k,k)*a(k) */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size      */
    static idx_t  last = 0;         /* largest n computed   */
    static int    initialized = 0;  /* cache initialized?  */

    if (!initialized) {
        /* start with space for at least a(0) */
        size_t init_cap = 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        memset(cache, 0, init_cap * sizeof(seq_t));
        cap = init_cap;
        /* initialise a(0) */
        cache[0] = (seq_t)1;
        last = 0;
        initialized = 1;
    }

    if (cap <= (size_t)n) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        memset(tmp, 0, new_cap * sizeof(seq_t));
        memcpy(tmp, cache, cap * sizeof(seq_t));
        free(cache);
        cache = tmp;
        cap = new_cap;
    }

    /* compute values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        idx_t maxk = i / 4;
        seq_t sum = 0;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t bin = binomial(i - 3 * k, k);
            seq_t prod = bin * cache[k];
            sum += prod;
        }
        cache[i] = sum;
    }

    last = n;
    return cache[n];
}
