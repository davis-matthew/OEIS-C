/*
 * A306921  Number of ways of breaking the binary expansion of n into
 *          consecutive blocks with no leading zeros.
 * https://oeis.org/A306921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   If n = k*2^e + {0,1} with k odd, e>0, then a(n)=a(k)*(e+1);
 *            a(0)=a(1)=1
 * Range:     u8 n <= 509, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Compute a(i) for all indices up to n (inclusive) and store in cache */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;

    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i <= 1)            /* base cases a(0)=a(1)=1 */
        {
            cache[i] = (seq_t)1;
            continue;
        }

        idx_t e, k;
        if ((i & 1ULL) == 0ULL)   /* even i */
        {
            e = __builtin_ctzll((unsigned long long)i);
            k = i >> e;
        }
        else                     /* odd i > 1 */
        {
            e = __builtin_ctzll((unsigned long long)(i - 1ULL));
            k = (i - 1ULL) >> e;
        }
        cache[i] = cache[k] * ((seq_t)(e + 1));
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
