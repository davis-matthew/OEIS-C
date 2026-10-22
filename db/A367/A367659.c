/*
 * A367659  G.f. A(x) satisfies A(x) = 1 / (1 - x - x * A(x^3)).
 * https://oeis.org/A367659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n‑1) + Σ_{k=0..⌊(n‑1)/3⌋} a(k) * a(n-1-3*k)
 *            (recurrence derived from A(x)=1/(1-x-xA(x³))
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 55, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* memoized values */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t cache_len = 0;      /* number of values stored (cache_len = highest index + 1) */

static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    /* grow capacity */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t newcap = cache_cap ? cache_cap << 1 : 4;
        while (newcap < needed)
            newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }

    /* compute missing values */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else {
            cache[i] = cache[i - 1];
            idx_t maxk = (i - 1) / 3;
            for (idx_t k = 0; k <= maxk; ++k) {
                cache[i] += cache[k] * cache[i - 1 - 3 * k];
            }
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
