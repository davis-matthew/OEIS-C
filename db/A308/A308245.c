/*
 * A308245  G.f.: x * Product_{k>=1} 1/(1 - a(k)*(-x)^k)^((-1)^k).
 * https://oeis.org/A308245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   ipow_idx, mem
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
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

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
                seq_t pow1 = ipow_idx(cache[d1], k / d1);
                seq_t term1 = pow1 * (seq_t)d1;
                if ( ((k + d1) & 1) == 0 )
                    Sk += term1;
                else
                    Sk -= term1;

                idx_t d2 = k / d;
                if (d2 != d1)
                {
                    seq_t pow2 = ipow_idx(cache[d2], k / d2);
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
