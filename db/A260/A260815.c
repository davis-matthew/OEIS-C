/*
 * A260815  a(2) = 3; for n >= 3, a(n) = a(n-1) + gcd(n, a(n-1))^2.
 * https://oeis.org/A260815
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = a(n-1) + gcd(n, a(n-1))^2 a(2) = 3
 * Range:     u8 n <= 22, u16 n <= 1558, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache      = NULL;   /* cache[i] == a(i) */
static size_t  cache_len = 0;      /* highest index+1 that is cached */
static size_t  cache_cap = 0;      /* allocated capacity */

/* Memoised computation of sequence A260815 */
seq_t a(idx_t n)
{
    if (n < 2) {
        return 0; /* undefined for n < 2; callers should not use it */
    }

    /* grow cache to at least size n+1 */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp;
        if (cache == NULL) {
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (tmp == NULL) {
            /* out of memory – abort sequence generation */
            abort();
        }
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* initialise base term if needed */
    if (cache_len <= 2) {
        cache[2] = 3;
        cache_len = 3;
    }

    /* compute missing terms up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t prev = cache[k - 1];
        seq_t g    = gcd((seq_t)k, prev);
        cache[k]   = prev + g * g;
    }
    cache_len = n + 1;

    return cache[n];
}
