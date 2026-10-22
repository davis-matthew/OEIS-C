/*
 * A352648  Expansion of e.g.f. 1/(1 - 2 * x * cosh(x)).
 * https://oeis.org/A352648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Sum_{k=0..floor((n-1)/2)} (2*k+1) * binomial(n,2*k+1)
 *            * a(n-2*k-1) a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;         /* array of computed terms      */
static size_t cache_cap = 0;        /* allocated capacity of cache */
static size_t cache_len = 0;        /* number of terms already computed */

static void ensure_cache(idx_t n)
{
    /* resize cache if needed so that it can hold terms up to n */
    if (n+1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < (size_t)(n+1)) new_cap *= 2;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);          /* cannot allocate */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    /* compute any missing terms up to n */
    if (cache_len <= n)
    {
        for (idx_t cur = cache_len; cur <= n; ++cur)
        {
            if (cur == 0)
            {
                cache[0] = (seq_t)1;
                continue;
            }

            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;          /* C(cur,0) */
            for (idx_t i = 1; i <= cur; ++i)
            {
                /* update binom to C(cur,i) */
                binom = binom * (seq_t)(cur - i + 1);
                binom = binom / (seq_t)i;
                if (i & 1)               /* i is odd */
                {
                    seq_t term = binom;
                    term = term * (seq_t)i;                 /* multiply by (2k+1) */
                    term = term * cache[cur - i];           /* * a(cur-i)   */
                    sum = sum + term;
                }
            }
            cache[cur] = sum * (seq_t)2;     /* a(cur) = 2*sum   */
        }
        cache_len = (size_t)n + 1;
    }
    return cache[n];
}
