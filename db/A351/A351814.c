/*
 * A351814  G.f. A(x) satisfies A(x) = 1 + x * A(x/(1 - x)^4) / (1 - x).
 * https://oeis.org/A351814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(n+3*k-1,n-k-1) * a(k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k); */

static seq_t *cache = NULL;   /* dynamic cache for sequence values   */
static size_t cache_cap = 0;  /* current allocated size              */
static idx_t  max_cached = 0; /* largest index that has been computed*/

seq_t a(idx_t n)
{
    /* initialise cache for the first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;   /* memory allocation failed */
        cache[0] = 1;    /* a(0) */
        max_cached = 0;
    }

    /* expand cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)(n) + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;      /* allocation failed */
        cache    = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            idx_t n_arg = i + 3 * k - 1;
            idx_t k_arg = i - k - 1;
            sum += binomial(n_arg, k_arg) * cache[k];
        }
        cache[i] = sum;
    }
    max_cached = n;

    return cache[(size_t)n];
}
