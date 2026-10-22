/*
 * A360502  Concatenate the ternary strings for 1,2,...,n.
 * https://oeis.org/A360502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the base‑3 representations of 1,2,…,n,
 *            interpreted as a decimal integer
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   pow10, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* caches already computed values; thread‑local static to keep independent state */
static seq_t *cache      = NULL;   /* array of a(k), 1 ≤ k ≤ computed_up_to          */
static size_t cache_cap  = 0;      /* allocated elements in cache                   */
static idx_t  computed_up_to = 0;  /* highest n for which cache[n] is valid         */
static seq_t  cur          = 0;   /* a(computed_up_to)                           */

/* enlarge the cache to at least n+1 elements (n≥1) */
static void enlarge_cache(idx_t n)
{
    size_t newcap = (size_t)(n + 1);
    if (newcap <= cache_cap) return;

    seq_t *newcache = seq_alloc_new_array(newcap);
    if (!newcache) { /* allocation failure: abort safely */
        if (cache) seq_free_array(cache);
        cache = NULL; cache_cap = 0; computed_up_to = 0; cur = 0;
        return;
    }

    /* copy existing values */
    size_t copylen = cache_cap < newcap ? cache_cap : newcap;
    for (size_t i = 0; i < copylen; ++i) newcache[i] = cache[i];

    seq_free_array(cache);
    cache     = newcache;
    cache_cap = newcap;
}

/* compute a(n) for n ≥ 1 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for n == 0, but guard explicitly */

    /* ensure the cache is big enough */
    enlarge_cache(n);

    /* already computed? */
    if (computed_up_to >= n) return cache[(size_t)n];

    /* compute term for each i = computed_up_to + 1 ... n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        /* convert i to base‑3, build the decimal integer of its digits */
        idx_t  t    = i;
        seq_t  digits = 0;
        seq_t  mult   = 1;
        idx_t  len    = 0;

        while (t > 0) {
            seq_t digit = (seq_t)(t % 3);
            digits += digit * mult;
            mult   *= (seq_t)10;
            t      /= 3;
            ++len;
        }

        /* append the digits to the current value */
        seq_t pow10len = pow10((idx_t)len);
        cur = cur * pow10len + digits;
        cache[(size_t)i] = cur;
    }

    computed_up_to = n;
    return cache[(size_t)n];
}
