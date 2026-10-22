/*
 * A327474  Number of distinct means of subsets of {1..n}, where {} has mean
 *          0.
 * https://oeis.org/A327474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=4, a(3)=6; a(n) = 2*a(n-1) - a(n-2) +
 *            phi(n-1) for n > 3 where phi(m) is Euler's totient function of m
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoized values of the sequence   */
    static size_t cap   = 0;      /* allocated capacity for cache[]     */
    static idx_t  len   = 0;      /* number of valid entries in cache   */

    /* Ensure capacity for indices up to n.  Allocate at least 4 for the base. */
    size_t need = (size_t)(n + 1);
    if (need < 4) need = 4;
    if (cap < need) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return 0;       /* allocation failed – should not happen */
        cache = tmp;
        cap = need;
    }

    /* Initialise the four base values once. */
    if (len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)6;
        len = 4;
    }

    /* Quick return for n < 4, after the base has been set. */
    if (n < 4) return cache[n];

    /* Extend the cached sequence up to index n. */
    for (idx_t i = len; i <= n; ++i) {
        /* a(i) = 2*a(i-1) - a(i-2) + phi(i-1) */
        seq_t val = cache[i-1];
        val += cache[i-1];     /* 2*a(i-1) */
        val -= cache[i-2];     /* - a(i-2) */
        val += phi(i-1);       /* + phi(i-1) */
        cache[i] = val;
    }
    len = n + 1;

    return cache[n];
}
