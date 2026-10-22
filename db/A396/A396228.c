/*
 * A396228  a(1) = 1; a(n+1) = -Sum_{k=1..n} (-1)^k * a(k) * floor(n/k).
 * https://oeis.org/A396228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n) = -∑_{k=1}^{n-1} (-1)^k * a(k) * floor((n-1)/k)
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoisation */
static idx_t cache_cap = 0;   /* allocated capacity of cache[] */
static idx_t max_computed = 0;/* largest n already calculated */
static seq_t *cache = NULL;   /* 1‑based sequence storage */

/* expand cache to hold at least 'need' entries (1‑based) */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap)
        return;                     /* already enough space */

    idx_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need)
        new_cap <<= 1;               /* grow geometrically */

    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp)
        abort();                     /* allocation failed */

    cache = tmp;
    cache_cap = new_cap;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1 */

    ensure_cache(n);

    if (n <= max_computed)
        return cache[n];              /* already known */

    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        if (i == 1) {                  /* a(1) = 1 */
            cache[1] = (seq_t)1;
            continue;
        }

        idx_t N = i - 1;               /* recurrence uses N = i-1 */
        seq_t sum = (seq_t)0;

        for (idx_t k = 1; k <= N; ++k)
        {
            seq_t term = cache[k] * (seq_t)(N / k);  /* a(k) * floor(N/k) */
            if (k & 1)                      /* (-1)^k */
                term = -term;
            sum += term;
        }

        cache[i] = -sum;                 /* a(i) = -sum */
    }

    max_computed = n;
    return cache[n];
}
