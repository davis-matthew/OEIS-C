/*
 * A356619  a(n) = number of k-tuples (u(1), u(2), ..., u(k)) with 1 <= u(1) <
 *          u(2) < ... < u(k) <= n such that u(i) - u(i-1) <= 3 for i =
 *          2,...,k.
 * https://oeis.org/A356619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-4) + a(n-5) a(0)=0, a(1)=1,
 *            a(2)=4, a(3)=11, a(4)=25
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute a(n) for all n >= 0, memoised in a static cache */
#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding the computed values   */
static size_t cache_cap = 0;     /* capacity (number of allocated slots) */
static size_t cache_used = 0;    /* number of valid entries stored      */

/* Seed values for n=0..4 */
static const seq_t seed[5] = {0, 1, 4, 11, 25};

/* Adjust the cache so that it can hold indices up to new_cap-1 */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();      /* out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n) for any n >= 0, using the recurrence relation. */
seq_t a(idx_t n)
{
    /* Convert idx_t to size_t for array indexing – safe for the
       anticipated range of n (up to about 10 000). */
    size_t sn = (size_t)n;

    /* Grow the cache and compute missing entries if necessary */
    if (sn + 1 > cache_used) {
        /* Ensure storage for all indices up to sn */
        if (sn + 1 > cache_cap) ensure_capacity(sn + 1);

        /* Determine from which index to start filling */
        size_t start = cache_used;      /* next index to compute */
        if (start < 5) start = 0;       /* seed needs to be written first */

        for (size_t i = start; i <= sn; ++i) {
            if (i < 5) {
                cache[i] = seed[i];
            } else {
                /* a(i) = 3*a(i-1) - 2*a(i-2) - a(i-4) + a(i-5) */
                cache[i] = 3 * cache[i-1]
                           - 2 * cache[i-2]
                           - cache[i-4]
                           + cache[i-5];
            }
        }
        cache_used = sn + 1;   /* all indices up to sn are now valid */
    }
    return cache[sn];
}
