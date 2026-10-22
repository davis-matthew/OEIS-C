/*
 * A345140  a(1) = 1; a(n+1) = n + Sum_{d|n} a(d).
 * https://oeis.org/A345140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n+1)=n+Σ_{d|n}a(d)
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static storage for memoisation */
static seq_t *cache      = NULL;   /* values of a(n), 1‑indexed, 0‑unused */
static idx_t  cache_cap  = 0;      /* allocated entries (0 … cap‑1) */
static idx_t  max_comp   = 0;      /* largest n that has been computed */

/* ensure that the cache has room for at least n entries (1‑indexed) */
static void ensure_cache_for_n(idx_t n)
{
    if ((idx_t)(n + 1) <= cache_cap)
        return;                       /* already big enough */

    /* grow, doubling strategy */
    idx_t new_cap = (cache_cap == 0) ? 1 : (cache_cap * 2);
    while (new_cap < (idx_t)(n + 1))
        new_cap *= 2;

    /* reallocate */
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* unrecoverable, as per contest rules */

    /* zero‑initialize the new part */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;

    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined but safe for internal use */

    ensure_cache_for_n(n);

    /* initialise a(1) if needed */
    if (n == 1) {
        cache[1] = 1;
        max_comp = 1;
        return 1;
    }

    /* if we already have the value, simply return */
    if (cache[n] != 0)
        return cache[n];

    /* compute all missing values up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        /* i > 1 */
        idx_t k = i - 1;                /* we need Σ_{d | k} a(d) */
        seq_t sum = 0;
        for (idx_t d = 1; d <= k / d; ++d) {
            if (k % d == 0) {
                sum += cache[d];
                if (d != k / d)
                    sum += cache[k / d];
            }
        }
        cache[i] = (seq_t)k + sum;      /* cast k to seq_t before adding */
    }
    max_comp = n;
    return cache[n];
}
