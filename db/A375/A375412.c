/*
 * A375412  Expansion of e.g.f. exp(x^2 * (1 - x)) / (1 - x).
 * https://oeis.org/A375412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n * n! * Sum_{k=0..floor(n/2)} binomial(k-1,
 *            n-2*k)/k!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (OEIS A375412):                         */
/*   a(0) = 1,  a(1) = 1,  a(2) = 4,  a(3) = 6,        */
/*   a(n) = n*a(n-1) + (2n-2)*a(n-2)                     */
/*           - 5*(n-1)*(n-2)*a(n-3)                     */
/*           + 3*(n-1)*(n-2)*(n-3)*a(n-4)   for n >= 4 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache      = NULL;   /* cache of computed a(n) values       */
static size_t cache_cap  = 0;      /* capacity of 'cache' array           */
static idx_t  computed   = 0;      /* number of entries currently known   */

seq_t a(idx_t n)
{
    /* Allocate initial cache if this is the first call. */
    if (cache == NULL) {
        cache_cap = 16;
        cache     = seq_alloc(cache_cap);
    }

    /* Grow the cache if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* Compute missing values. */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;
        } else if (i == 1) {
            val = 1;
        } else if (i == 2) {
            val = 4;
        } else if (i == 3) {
            val = 6;
        } else {
            seq_t sn     = (seq_t)i;
            seq_t sn_1   = sn - 1;         /* i-1   */
            seq_t sn_2   = sn_1 - 1;       /* i-2   */
            seq_t sn_3   = sn_2 - 1;       /* i-3   */

            /* a(n) = n*a(n-1) + (2n-2)*a(n-2)
               - 5*(n-1)*(n-2)*a(n-3) + 3*(n-1)*(n-2)*(n-3)*a(n-4) */
            seq_t term1 = sn * cache[i-1];
            seq_t term2 = (seq_t)(2 * (i - 1)) * cache[i-2];
            seq_t term3 = (seq_t)5 * sn_1 * sn_2 * cache[i-3];
            seq_t term4 = (seq_t)3 * sn_1 * sn_2 * sn_3 * cache[i-4];

            val = term1 + term2 - term3 + term4;
        }
        cache[i] = val;
    }

    computed = n + 1;   /* up to and including n are now known */
    return cache[n];
}
