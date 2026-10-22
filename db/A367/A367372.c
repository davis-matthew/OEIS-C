/*
 * A367372  Expansion of the e.g.f. (exp(x) / (4 - 3*exp(x)))^(1/2).
 * https://oeis.org/A367372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=2*a(n-1)+3*Sum_{k=1}^{n-1} binomial(n-1,k)*a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation storage */
    static size_t cache_size = 0; /* current allocated size */

    /* Allocate or grow the cache on first use and on larger requests */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)calloc(1, sizeof(seq_t));
    } else if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed – abort */
            abort();
        }
        cache = tmp;
        /* initialise any newly added elements to 0 */
        for (size_t i = cache_size; i < new_size; ++i) cache[i] = 0;
        cache_size = new_size;
    }

    /* Return cached value if already computed */
    if (cache[n] != 0) return cache[n];

    /* Base case */
    if (n == 0) {
        cache[0] = 1;
        return 1;
    }

    /* Compute the sum part of the recurrence */
    seq_t sum = 0;
    idx_t m = n - 1;                    /* n-1 is the upper bound for k */
    for (idx_t k = 1; k <= m; ++k) {
        seq_t c    = binomial(m, k);      /* binomial coefficient (n-1 choose k) */
        seq_t term = c * cache[n - k];    /* a(n-k) has already been computed */
        sum += term;
    }

    /* Apply the recurrence */
    seq_t result = cache[n - 1] * 2 + sum * 3;
    cache[n] = result;
    return result;
}
