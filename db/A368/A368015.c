/*
 * A368015  Expansion of e.g.f. 1/(1 - exp(2*x) + exp(3*x)).
 * https://oeis.org/A368015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (2^k - 3^k) * binomial(n,k) * a(n-k), a(0) =
 *            1
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

static seq_t *cache = NULL;
static size_t   cache_cap = 0;
static idx_t    cache_len = 0;

static void
ensure_cache_cap(idx_t n)
{
    if (cache_cap > (size_t)n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t
a(idx_t n)
{
    if (n >= cache_len) {
        ensure_cache_cap(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t bin  = binomial(i, k);
                seq_t t    = pow2(k) - pow3(k);
                seq_t term = t * bin * cache[i - k];
                sum += term;
            }
            cache[i] = sum;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
