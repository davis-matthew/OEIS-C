/*
 * A301742  a(n) = [x^n] 1/(1 - n*Sum_{k>=1} x^(k^3)).
 * https://oeis.org/A301742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] 1/(1 - n*Sum_{k>=1} x^(k^3))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* cache[i] == 0 => not yet computed */
static size_t cache_cap = 0;     /* number of elements currently allocated */

/* Ensure cache has room for index n (including index 0) */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_cap)
        return;

    size_t new_cap = need;
    seq_t *new_cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
    if (!new_cache)
        abort();

    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;

    /* initialise a[0] on first allocation */
    if (need > 0 && cache[0] == 0)
        cache[0] = (seq_t)1;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    ensure_cache(n);

    if (cache[(size_t)n] != (seq_t)0)
        return cache[(size_t)n];

    /* Allocate DP array for this particular n */
    seq_t *dp = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp)
        abort();

    dp[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1;; ++k) {
            idx_t cube = k * k * k;
            if (cube > i)
                break;
            sum += dp[(size_t)i - cube];
        }
        dp[(size_t)i] = (seq_t)n * sum;
    }

    seq_t result = dp[(size_t)n];
    free(dp);

    cache[(size_t)n] = result;
    return result;
}
