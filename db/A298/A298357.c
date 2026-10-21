/*
 * A298357  a(n) = a(n-1) + a(n-2) + a([n/2]) + a([n/3]) + ... + a([n/n]),
 *          where a(0) = 1, a(1) = 2, a(2) = 3.
 * https://oeis.org/A298357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a([n/2]) + a([n/3]) + ... + a([n/n]),
 *            a(0) = 1, a(1) = 2, a(2) = 3
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms      */
static idx_t  cap   = 0;            /* current capacity of cache    */
static idx_t  high  = 0;            /* highest index that has been computed */

/* Compute the n-th term of sequence A298357.  Results are cached
   so that subsequent calls work in O(1) after the necessary
   precomputation.  The algorithm uses the grouping trick:
      sum_{k=2}^{n} a(floor(n/k)) =
      sum_{v=1}^{floor(n/2)} (floor(n/v) - floor(n/(v+1))) * a(v)   */
seq_t a(idx_t n)
{
    /* initialise cache once */
    if (cache == NULL) {
        cap  = 1;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        high = 0;
    }

    /* grow cache to cover n */
    if (n + 1 > cap) {
        idx_t newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();     /* allocation failed */
        cache = tmp;
        for (idx_t i = cap; i < newcap; ++i) cache[i] = (seq_t)0;
        cap = newcap;
    }

    /* if already computed, return it */
    if (n <= high) return cache[n];

    /* compute missing terms sequentially up to n */
    for (idx_t i = high + 1; i <= n; ++i) {
        if (i == 1) { cache[i] = (seq_t)2; continue; }   /* a(1) = 2 */
        if (i == 2) { cache[i] = (seq_t)3; continue; }   /* a(2) = 3 */

        seq_t sum = cache[i - 1] + cache[i - 2];

        idx_t half = i / 2;                      /* all floor(i/k) for k>=2 are <= i/2 */
        for (idx_t v = 1; v <= half; ++v) {
            idx_t l   = i / v;
            idx_t r   = i / (v + 1);
            idx_t cnt = l - r;                    /* how many k give floor(i/k) == v */
            sum += cache[v] * (seq_t)cnt;
        }
        cache[i] = sum;
    }

    high = n;
    return cache[n];
}
