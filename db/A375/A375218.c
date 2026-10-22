/*
 * A375218  a(n) = Sum_{k=0..floor(n/2)} (k+1) * binomial(k,n-2*k)^2.
 * https://oeis.org/A375218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (k+1)*binomial(k, n-2*k)^2
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Simple memoisation cache for correct O(1) retrieval after first calculation */
static seq_t *cache      = NULL;
static bool *computed    = NULL;
static idx_t cache_len   = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;

    idx_t new_len = n + 1U;
    cache      = (seq_t *)realloc(cache,      new_len * sizeof(seq_t));
    computed   = (bool *)realloc(computed,   new_len * sizeof(bool));

    /* initialise newly allocated slots */
    for (idx_t i = cache_len; i < new_len; ++i) {
        computed[i] = false;
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (computed[n]) return cache[n];

    seq_t sum = 0;
    idx_t k_limit = n >> 1;          /* floor(n/2) */
    for (idx_t k = 0; k <= k_limit; ++k) {
        idx_t r = n - (k << 1);      /* n - 2*k */
        if (r > k) continue;        /* binomial(k,r) = 0 in this case */

        seq_t c = binomial(k, r);    /* required binomial coefficient */
        if (c == 0) continue;       /* skip zero contribution */

        /* term = (k+1) * c^2 */
        seq_t term = ((seq_t)(k + 1U)) * c * c;
        sum = sum + term;
    }

    cache[n] = sum;
    computed[n] = true;
    return sum;
}
