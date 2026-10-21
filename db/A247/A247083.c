/*
 * A247083  a(n) = 0 for n <= 0: Starting with n=1, a(n) = 1 + the sum of the
 *          digital sums of a(0) through a(n-3).
 * https://oeis.org/A247083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 + Sum_{k=0..n-3} digsum(a(k)) Recurrence: a(n) = a(n-1)
 *            + digsum(a(n-3)) for n >= 4 Initial values: a(0)=0,
 *            a(1)=a(2)=a(3)=1 This implementation memoizes computed terms and
 *            can handle non‑monotonic calls to a(n)
 * Range:     u8 n <= 35, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"

static seq_t *cache    = NULL;   /* memoized terms            */
static idx_t  cap      = 0;      /* allocated capacity        */
static idx_t  max_comp = 0;      /* highest index computed   */

/* Ensure the cache has room for index n */
static void ensure_capacity(idx_t n)
{
    if (n < cap) return;
    idx_t new_cap = cap ? cap << 1 : 4;   /* start with 4 entries */
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                   /* allocation failure    */
    cache = tmp;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Lazy initialization of cache */
    if (!cache) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;
        cache[1] = cache[2] = cache[3] = (seq_t)1;
        max_comp = 3;
    }

    if (n <= max_comp)    /* already known */
        return cache[n];

    ensure_capacity(n);

    /* Compute missing terms up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        /* i >= 4 here, so i-3 is valid */
        cache[i] = cache[i - 1] + digit_sum(cache[i - 3]);
    }
    max_comp = n;
    return cache[n];
}
