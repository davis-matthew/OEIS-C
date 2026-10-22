/*
 * A390828  a(n) = Sum_{k=0..n} binomial(n+2*k+5,n-k) * Fibonacci(k+1).
 * https://oeis.org/A390828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A390828
   a(n) = Sum_{k=0..n} binomial(n+2*k+5, n-k) * Fibonacci(k+1)
   Recurrence:  a(n) = 7*a(n-1) - 17*a(n-2) + 23*a(n-3) - 16*a(n-4)
                                       + 6*a(n-5) - a(n-6)
   Initial values: a(0)=1, a(1)=7, a(2)=32, a(3)=128, a(4)=497, a(5)=1933
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"   /* mul_small(seq_t x, unsigned int m) */

static seq_t *cache = NULL;   /* cached sequence values     */
static idx_t   cache_len = 0; /* number of entries currently stored */

/* Ensure the cache contains a(0)..a(n); compute any missing terms.  */
static void ensure_cache(idx_t n)
{
    /* Initialise the first six values if the cache is empty.  */
    if (cache == NULL) {
        /* allocate space for at least the first 6 terms */
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (!cache) return;   /* allocation failed – caller will see 0 */
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)7;   /* a(1) */
        cache[2] = (seq_t)32;  /* a(2) */
        cache[3] = (seq_t)128; /* a(3) */
        cache[4] = (seq_t)497; /* a(4) */
        cache[5] = (seq_t)1933;/* a(5) */
        cache_len = 6;
    }

    /* Nothing to do if the requested index is already cached.  */
    if (n + 1 <= cache_len) return;

    /* Remember the old size before resizing.  */
    idx_t old_len = cache_len;

    /* Grow the cache to hold indices up to n (inclusive).  */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return;          /* reallocation failed – cache stays as is */
    cache = tmp;
    cache_len = n + 1;

    /* Compute the missing terms using the 6‑term recurrence.  */
    for (idx_t i = old_len; i <= n; ++i) {
        /* Skip the first six terms – they are already initialized.  */
        if (i <= 5) continue;

        seq_t pos = mul_small(cache[i - 1], 7);
        pos += mul_small(cache[i - 3], 23);
        pos += mul_small(cache[i - 5], 6);

        seq_t neg = mul_small(cache[i - 2], 17);
        neg += mul_small(cache[i - 4], 16);
        neg += cache[i - 6];

        /* The recurrence guarantees pos >= neg, so subtraction is safe. */
        cache[i] = pos - neg;
    }
}

/* Public interface required by the runner.  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
