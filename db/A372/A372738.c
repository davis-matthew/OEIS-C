/*
 * A372738  Binomial transform of A369795.
 * https://oeis.org/A372738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + Sum_{j=1..n} (3^j-1)*C(n,j)*a(n-j) Initial value:
 *            a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* cache[0..cached_upto] */
    static idx_t cache_size = 0;     /* allocated number of elements */
    static idx_t cached_upto = 0;    /* highest n that has been computed */

    /* expand cache if necessary */
    if (cache_size <= n)
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;      /* out‑of‑memory → undefined value */

        if (cache)
        {
            /* copy already computed values into the new buffer */
            for (idx_t i = 0; i <= cached_upto; ++i)
                tmp[i] = cache[i];
            free(cache);
        }
        cache      = tmp;
        cache_size = new_size;
    }

    /* establish base case a(0)=1 (only once) */
    if (cached_upto == 0 && cache[0] == (seq_t)0)
        cache[0] = (seq_t)1;

    /* if the requested value is already known, return it */
    if (n <= cached_upto)
        return cache[n];

    /* compute terms from cached_upto+1 up to n (inclusive) */
    for (idx_t k = cached_upto + 1; k <= n; ++k)
    {
        /* start with the 2^k term */
        seq_t sum = pow2(k);

        /* add the binomial sum for this k */
        for (idx_t j = 1; j <= k; ++j)
        {
            seq_t B      = binomial(k, j);          /* C(k,j)    */
            seq_t p3j    = pow3(j);                 /* 3^j       */
            seq_t term   = (p3j - (seq_t)1) * B * cache[k - j];
            sum += term;
        }

        cache[k] = sum;
    }

    cached_upto = n;
    return cache[n];
}
