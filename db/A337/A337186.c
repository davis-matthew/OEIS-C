/*
 * A337186  a(n) = 1 + Sum_{k=0..n-2} binomial(n-2,k) * a(k).
 * https://oeis.org/A337186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-2} binomial(n-2,k) * a(k) Initial terms:
 *            a(0) = 1, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* binomial(idx_t, idx_t) */

static seq_t *cache     = NULL;   /* cached values of a(n)          */
static size_t cap       = 0;      /* capacity of the cache array    */
static size_t last_used = 0;      /* highest index that has been computed */

/* Ensure the cache can hold values up to index n.
   Returns non‑zero on success, zero on allocation failure. */
static int
ensure_capacity(idx_t n)
{
    if (cap <= (size_t)n) {
        size_t newcap = (size_t)n + 1;
        if (newcap < 8)          /* grow in blocks of at least 8 */
            newcap = 8;
        seq_t *tmp = (seq_t *)realloc(cache,
                                     newcap * sizeof(seq_t));
        if (!tmp)            /* allocation failed */
            return 0;
        cache = tmp;
        cap   = newcap;
    }
    return 1;
}

seq_t
a(idx_t n)
{
    /* First‑time initialisation of the cache. */
    if (!cache) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;    /* allocation failure – unlikely */
        cache[0] = (seq_t)1;    /* a(0) */
        cache[1] = (seq_t)1;    /* a(1) */
        last_used = 1;
    }

    /* Fast paths for the first two terms. */
    if (n == 0)
        return cache[0];
    if (n == 1)
        return cache[1];

    /* Grow cache if necessary. */
    if (!ensure_capacity(n))
        return (seq_t)0;    /* allocation failure */

    /* Compute missing values up to n. */
    for (size_t idx = last_used + 1; idx <= (size_t)n; ++idx) {
        seq_t sum = (seq_t)0;
        idx_t m2  = (idx_t)idx - 2;      /* n-2 in the recurrence   */
        for (idx_t k = 0; k <= m2; ++k) {
            sum += binomial(m2, k) * cache[k];
        }
        cache[idx] = sum + (seq_t)1;
    }

    last_used = (size_t)n;
    return cache[n];
}
