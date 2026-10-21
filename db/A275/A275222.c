/*
 * A275222  Number of n X 2 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,1) or (-1,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-4) + a(n-5) (for n>6) Initial
 *            terms: a(1)=2, a(2)=6, a(3)=11, a(4)=27, a(5)=66, a(6)=162 This
 *            implementation memoises the sequence and grows the cache as
 *            needed (O(n) time, O(n) memory)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation cache: cache[i] holds a(i) for 0 ≤ i ≤ cache_max.
   Index 0 is unused (a(0) is not defined). */
static seq_t *cache = NULL;
static idx_t  cache_max = 0;

/* Ensure that the cache contains at least the term a(n). */
static void ensure_cache(idx_t n)
{
    /* Size needed is max(n,6)+1 so that indices 0..6 always exist. */
    idx_t needed = (n > 6) ? n + 1 : 7;

    if (cache == NULL) {
        /* Allocate fresh cache. */
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (cache == NULL) abort();             /* out of memory   */
        cache[0] = 0;                           /* unused, but set */
        cache[1] = 2;
        cache[2] = 6;
        cache[3] = 11;
        cache[4] = 27;
        cache[5] = 66;
        cache[6] = 162;
        cache_max = 6;
    } else if (needed > cache_max + 1) {
        /* Grow the cache to the required size. */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) abort();              /* out of memory   */
        cache = tmp;
        /* Initialise new entries only if the cache was freshly allocated */
        if (cache_max + 1 <= 6) {
            cache[1] = 2;
            cache[2] = 6;
            cache[3] = 11;
            cache[4] = 27;
            cache[5] = 66;
            cache[6] = 162;
        }
        /* cache_max remains unchanged until we fill missing terms. */
    }

    /* Compute missing terms up to n (recurrence valid for k>6). */
    if (n > cache_max) {
        for (idx_t k = cache_max + 1; k <= n; ++k) {
            seq_t t1 = cache[k - 1]; /* a(k-1) */
            seq_t t2 = cache[k - 2]; /* a(k-2) */
            seq_t t3 = cache[k - 4]; /* a(k-4) */
            seq_t t4 = cache[k - 5]; /* a(k-5) */

            /* a(k) = 3*t1 - t2 - 2*t3 + t4 */
            seq_t val = t1 + t1 + t1;     /* 3*t1   */
            val -= t2;                   /* - a(k-2) */
            val -= t3;
            val -= t3;                   /* - 2*a(k-4) */
            val += t4;                   /* + a(k-5) */

            cache[k] = val;
        }
        cache_max = n;
    }
}

/* Public function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* a(0) is not defined, but harmless */
    ensure_cache(n);
    return cache[n];
}
