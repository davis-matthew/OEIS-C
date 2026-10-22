/*
 * A341988  Number of ways to write n as an ordered sum of 9 primes (counting
 *          1 as a prime).
 * https://oeis.org/A341988
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (x + Σ_{p prime} x^p)^9, i.e.
 *            number of ways to write n as an ordered sum of 9 primes,
 *            counting 1 as a prime
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static seq_t *a_cache = NULL;   /* stores a[n] for 0 <= n <= cache_max */
static idx_t  cache_max = 0;    /* highest index for which a_cache is valid */

seq_t a(idx_t n)
{
    /* defined for n >= 9; behavior for smaller n is irrelevant */
    if (n < 9)
        return (seq_t)0;

    /* recompute cache up to n if necessary */
    if (n > cache_max)
    {
        idx_t new_max = n;

        /* obtain list of primes ≤ new_max */
        size_t prime_cnt = 0;
        uint64_t *primes = NULL;
        build_primes_upto(new_max, &primes, &prime_cnt);

        /* dp[t][s] : number of ways to reach sum s using exactly t primes */
        size_t msize = (size_t)new_max + 1;          /* indices 0..new_max */
        size_t total  = 10 * msize;                  /* t = 0..9 */

        seq_t *dp = (seq_t*)calloc(total, sizeof(seq_t));   /* automatically zeroed */
        dp[0] = (seq_t)1;   /* dp[0][0] = 1 */

        /* dynamic programming */
        for (idx_t t = 1; t <= 9; ++t)
        {
            size_t t_offset = t * msize;
            size_t t1_offset = (t - 1) * msize;

            /* contribution from real primes */
            for (size_t pi = 0; pi < prime_cnt; ++pi)
            {
                idx_t p = (idx_t)primes[pi];
                if (p > new_max) break;
                for (idx_t s = p; s <= new_max; ++s)
                    dp[t_offset + s] += dp[t1_offset + s - p];
            }

            /* contribution from the “prime” 1 */
            for (idx_t s = 1; s <= new_max; ++s)
                dp[t_offset + s] += dp[t1_offset + s - 1];
        }

        /* allocate new cache of a[n] values */
        seq_t *new_cache = (seq_t*)calloc((size_t)new_max + 1, sizeof(seq_t));
        for (idx_t s = 9; s <= new_max; ++s)
            new_cache[s] = dp[9 * msize + s];

        /* replace old cache */
        if (a_cache)
            free(a_cache);
        a_cache   = new_cache;
        cache_max = new_max;

        free(dp);
        free(primes);
    }

    return a_cache[n];
}
