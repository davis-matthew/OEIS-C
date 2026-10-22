/*
 * A346078  G.f. A(x) satisfies: A(x) = 1 + x - x^2 * A(x/(1 - x)) / (1 - x).
 * https://oeis.org/A346078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=a(1)=1; for n>=2: a(n) = -sum_{k=0}^{n-2}
 *            binomial(n-2,k)*a(k)
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 20, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static inline seq_t mul128(seq_t a, seq_t b)
{
    /* Use a 128‑bit temporary to avoid overflow for built‑in integer types. */
    return (seq_t)(((__int128)a) * b);
}

/* Computes term a(n) of OEIS A346078. */
seq_t a(idx_t n)
{
    /* Static cache for memoization. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;  /* Number of terms actually computed. */

    /* Ensure the cache is large enough for indices 0 .. n. */
    ensure_cache(n, &cache, &cache_cap);

    if (cache_len <= n)
    {
        /* Compute missing terms up to n. */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            seq_t val;
            if (i == 0 || i == 1)
                val = 1;
            else
            {
                idx_t m = i - 2;        /* Number of binomial factors. */
                seq_t bin = 1;          /* Current binomial coefficient C(m, k). */
                seq_t sum = 0;          /* Accumulated sum. */
                for (idx_t k = 0; k <= m; ++k)
                {
                    if (k > 0)
                    {
                        bin = mul128(bin, (seq_t)(m - k + 1));
                        bin = bin / k;
                    }
                    sum += mul128(cache[k], bin);
                }
                val = -sum;
            }
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
