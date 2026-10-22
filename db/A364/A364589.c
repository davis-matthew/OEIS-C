/*
 * A364589  G.f. satisfies A(x) = 1/(1-x) + x^3*A(x)^3.
 * https://oeis.org/A364589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n-k,2*k) * binomial(3*k,k)
 *            / (2*k+1)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, ensure_cache, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=1, a(2)=1 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include "../utils/ensure_cache.h"

/* External linkage function to compute term a(n) */
seq_t a(idx_t n)
{
    /* Cache to avoid recomputation for multiple calls */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Allocate cache up to index n, initialized to 0 */
    ensure_cache(n, &cache, &cache_cap);

    /* Return cached value if already computed */
    if (cache[n] != 0)
        return cache[n];

    seq_t result = 0;
    for (idx_t k = 0; ; ++k)
    {
        if (3 * k > n)
            break;

        /* binomial(n-k, 2*k) */
        seq_t bin1 = binomial((idx_t)(n - k), (idx_t)(2 * k));

        /* binomial(3*k, k) */
        seq_t bin2 = binomial((idx_t)(3 * k), (idx_t)k);

        /* (bin1 * bin2) / (2*k + 1) */
        seq_t term = seq_mul(bin1, bin2);
        term = term / (2 * k + 1);

        result += term;
    }

    /* Store computed value in cache and return */
    cache[n] = result;
    return result;
}
