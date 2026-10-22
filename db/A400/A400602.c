/*
 * A400602  a(1) = 1; a(n) = Sum_{k=1..n-1} a(gcd(n,k)) * a(k/gcd(n,k)).
 * https://oeis.org/A400602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(1) = 1; a(n) = Sum_{k=1..n-1} a(gcd(n,k)) * a(k/gcd(n,k))
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd_idx.h"

/* Integer GCD for idx_t values (used only for the recurrence). */

/* Cache for already computed values. */
static seq_t *cache      = NULL;   /* pointer to the cache array   */
static size_t cache_cap  = 0;      /* number of slots currently usable */

static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;                      /* already sufficient */

    size_t new_cap = ((size_t)n + 1) * 2;
    seq_t *tmp;

#if defined(__cplusplus)
    tmp = static_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
    tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!tmp)
        exit(EXIT_FAILURE);          /* memory allocation failed */

    /* Zero‑initialize new slots to indicate “uncomputed”. */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;

    cache     = tmp;
    cache_cap = new_cap;
}

/* Main function computing the sequence value. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* outside the defined range */

    /* Initialise cache on the first call. */
    if (!cache) {
        size_t init_cap = 1;         /* need at least slot 1 */
#if defined(__cplusplus)
        cache = static_cast<seq_t*>(calloc(init_cap, sizeof(seq_t)));
#else
        cache = calloc(init_cap, sizeof(seq_t));
#endif
        if (!cache)
            exit(EXIT_FAILURE);
        cache_cap = init_cap;
    }

    ensure_cache(n);

    /* If the value is already computed, return it. */
    if (cache[n] != 0)
        return cache[n];

    /* Compute all missing values up to n (including n). */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0)          /* already computed during a previous call */
            continue;

        if (i == 1) {
            cache[1] = 1;
            continue;
        }

        seq_t sum = 0;
        for (idx_t k = 1; k < i; ++k) {
            idx_t g = gcd_idx(i, k);
            sum += cache[g] * cache[k / g];
        }
        cache[i] = sum;
    }

    return cache[n];
}
