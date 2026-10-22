/*
 * A308077  G.f. A(x) satisfies: A(x) = x - A(x^2) + A(x^3) - A(x^4) + A(x^5)
 *          - A(x^6) + ...
 * https://oeis.org/A308077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n , d<n} (-1)^(n/d+1) * a(d) (a(1)=1)
 * Range:     i8 n <= 269, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* size_t */
#include <stdlib.h>        /* malloc / free */
#include "../utils/alloc_cache.h"   /* alloc_cache */
#include "../utils/free_cache.h"    /* free_cache */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for out‑of‑range calls */

    /* static cache of computed values, 0‑based: cache[i-1] == a(i) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;             /* capacity of cache array */
    static idx_t max_n  = 0;             /* largest n already computed */

    /* grow cache if necessary */
    if ((size_t)n > cap) {
        seq_t *tmp = alloc_cache(n);
        if (!tmp) return 0;              /* allocation failed – return 0 */
        for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
        free_cache(cache);
        cache = tmp;
        cap   = n;
    }

    /* compute missing entries up to n */
    if (max_n < n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i == 1) {
                cache[0] = 1;            /* a(1) = 1 */
            } else {
                seq_t sum = 0;
                /* enumerate divisors d of i, d < i */
                for (idx_t d = 1; d * d <= i; ++d) {
                    if (i % d == 0) {
                        idx_t d1 = d;
                        idx_t d2 = i / d;

                        /* divisor d1 */
                        if (d1 < i) {
                            seq_t term = cache[d1 - 1];
                            idx_t t = i / d1;  /* n/d */
                            if (((t + 1) & 1) == 0)
                                sum += term;  /* even exponent -> + */
                            else
                                sum -= term;  /* odd  exponent -> - */
                        }

                        /* divisor d2 (distinct from d1) */
                        if (d2 != d1 && d2 < i) {
                            seq_t term = cache[d2 - 1];
                            idx_t t = i / d2;  /* n/d2 */
                            if (((t + 1) & 1) == 0)
                                sum += term;
                            else
                                sum -= term;
                        }
                    }
                }
                cache[i - 1] = sum;
            }
        }
        max_n = n;
    }

    return cache[n - 1];
}
