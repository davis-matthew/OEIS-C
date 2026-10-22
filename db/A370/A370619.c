/*
 * A370619  Coefficient of x^n in the expansion of ( (1-x) / (1-x-x^2)
 *          )^(2*n).
 * https://oeis.org/A370619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2*n+k-1,k) *
 *            binomial(n-k-1,n-2*k) * Initial values: a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return 1;

    /* Ensure the cache is initialized (holds a(0) = 1) */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        cache[0] = 1;
        cache_size = 1;
    }

    /* Return already computed value if available */
    if (n < cache_size)
        return cache[n];

    /* Extend the cache to accommodate up to n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return 0;              /* allocation failure */
    cache = tmp;

    /* Compute terms from cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        idx_t maxk = i / 2;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t b1 = binomial(2 * i + k - 1, k);
            seq_t b2 = binomial(i - k - 1, i - 2 * k);
            sum += b1 * b2;
        }
        cache[i] = sum;
    }

    cache_size = n + 1;
    return cache[n];
}
