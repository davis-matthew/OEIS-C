/*
 * A360889  G.f. satisfies A(x) = 1 + x * (1 + x^3)^2 * A(x * (1 + x^3)).
 * https://oeis.org/A360889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/3)} binomial(n+1-3*k,k) *
 *            a(n-1-3*k); a(0)=1
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 27, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static idx_t max_computed = 0;

static void ensure_capacity(idx_t n) {
    if (n >= cache_len) {
        size_t new_len = cache_len ? cache_len : 1;
        while (new_len <= (size_t)n) new_len <<= 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }
}

seq_t a(idx_t n) {
    if (!cache) {
        cache_len = 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;
        max_computed = 0;
    }
    if (n == 0) return cache[0];
    if (n > max_computed) {
        ensure_capacity(n);
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            idx_t limit = (i - 1) / 3;
            seq_t sum = 0;
            for (idx_t k = 0; k <= limit; ++k) {
                seq_t term = binom(i + 1 - 3 * k, k);
                term = term * cache[i - 1 - 3 * k];
                sum += term;
            }
            cache[i] = sum;
        }
        max_computed = n;
    }
    return cache[n];
}
