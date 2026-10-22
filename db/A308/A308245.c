/*
 * A308245  G.f.: x * Product_{k>=1} 1/(1 - a(k)*(-x)^k)^((-1)^k).
 * https://oeis.org/A308245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   ensure_cache, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A308245
    Recurrence:
    a(1) = 1
    a(n+1) = (1/n) * sum_{k=1..n} ( sum_{d|k} (-1)^(k+d) * d * a(d)^(k/d) ) * a(n-k+1)
    Offset: 1
*/

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_cache.h"
#include "../utils/seq_pow.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used, safety only */

    ensure_cache(n, &cache, &cache_cap);

    /* initialise the first term */
    if (cache[1] == 0) cache[1] = 1;

    /* compute all values up to n that are still missing */
    for (idx_t i = 2; i <= n; ++i)
    {
        if (cache[i] != 0) continue;     /* already known */

        seq_t sum = 0;
        idx_t N = i - 1;                 /* the n in the recurrence */

        for (idx_t k = 1; k <= N; ++k)
        {
            /* inner sum over divisors of k */
            seq_t Sk = 0;

            for (idx_t d = 1; d * d <= k; ++d)
            {
                if (k % d != 0) continue;

                idx_t d1 = d;
                seq_t pow1 = seq_pow(cache[d1], k / d1);
                seq_t term1 = pow1 * (seq_t)d1;
                if ( ((k + d1) & 1) == 0 )
                    Sk += term1;
                else
                    Sk -= term1;

                idx_t d2 = k / d;
                if (d2 != d1)
                {
                    seq_t pow2 = seq_pow(cache[d2], k / d2);
                    seq_t term2 = pow2 * (seq_t)d2;
                    if ( ((k + d2) & 1) == 0 )
                        Sk += term2;
                    else
                        Sk -= term2;
                }
            }

            sum += Sk * cache[i - k];
        }

        cache[i] = sum / (seq_t)N;        /* division is exact */
    }

    return cache[n];
}
