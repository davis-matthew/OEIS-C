/*
 * A343978  Number of ordered 6-tuples (a,b,c,d,e,f) with gcd(a,b,c,d,e,f)=1
 *          (1<= {a,b,c,d,e,f} <= n).
 * https://oeis.org/A343978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = Sum_{k=1..n} mu(k) * floor(n/k)^6 Recurrence: a(1)=1,
 *            a(n)=n^6 - sum_{k=2..n} a(floor(n/k))
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1630,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A343978.  The function may be called repeatedly with
   arbitrary n; previously computed values are cached. */
#include <stdlib.h>

static seq_t *cache      = NULL;      /* cache[0] unused, cache[1] = a(1) */
static idx_t  cache_cap  = 0;         /* number of allocated slots */

seq_t a(idx_t n)
{
    /* n must be >= 1 for meaningful values; but return 0 for n==0 */
    if (n == 0)
        return (seq_t)0;

    /* Ensure the cache is large enough for indices up to n */
    idx_t old_cap = cache_cap;
    idx_t new_cap = n + 1;
    if (old_cap < new_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failure – give up */
            return (seq_t)0;
        cache = tmp;
        /* initialise newly allocated slots to zero */
        for (idx_t i = old_cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    idx_t start = (old_cap > 1) ? old_cap : 1;   /* first index not yet computed */
    for (idx_t m = start; m <= n; ++m) {
        /* m^6 */
        seq_t x   = (seq_t)m;
        seq_t x2  = x * x;      /* m^2 */
        seq_t x4  = x2 * x2;    /* m^4 */
        seq_t x6  = x4 * x2;    /* m^6 */

        /* subtract sum_{k=2..m} a(floor(m/k)) using grouping */
        seq_t sub = (seq_t)0;
        idx_t l = 2;
        while (l <= m) {
            idx_t t = m / l;           /* floor(m/l) */
            idx_t r = m / t;           /* last l giving same t */
            idx_t cnt = r - l + 1;     /* count of k's for this t */
            seq_t add = cache[t];
            add *= (seq_t)cnt;
            sub += add;
            l = r + 1;
        }

        cache[m] = x6 - sub;
    }
    return cache[n];
}
