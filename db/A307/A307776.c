/*
 * A307776  a(1) = 1; a(n+1) = Sum_{d|n} (-1)^(n/d+1)*a(d).
 * https://oeis.org/A307776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(1) = 1; a(n+1) = Σ_{d|n} (-1)^{(n/d)+1} * a(d)
 * Range:     i8 n <= 118, i16 n <= 510, i32 n <= 2566, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* the exact types seq_t and idx_t are supplied by the build system */
seq_t a(idx_t n)
{
    /* cache of computed values, 1‑indexed */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* largest index already computed */

    if (n == 0) {
        /* Undefined for n = 0; return 0 as a harmless default. */
        return (seq_t)0;
    }

    if (n > cache_size) {
        /* enlarge the cache to hold entries up to n (index 0 unused) */
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        /* compute all new terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)1;
            } else {
                idx_t m = i - 1;          /* m plays the role of n in the recurrence */
                seq_t sum = (seq_t)0;

                for (idx_t d = 1; d * d <= m; ++d) {
                    if (m % d == 0) {
                        /* first divisor d */
                        idx_t d1 = d;
                        idx_t q1 = m / d1;
                        seq_t sign1 = ((q1 + 1) & 1) ? (seq_t)(-1) : (seq_t)1;
                        sum += sign1 * cache[d1];

                        /* paired divisor d2, if distinct */
                        idx_t d2 = m / d1;
                        if (d2 != d1) {
                            idx_t q2 = m / d2;
                            seq_t sign2 = ((q2 + 1) & 1) ? (seq_t)(-1) : (seq_t)1;
                            sum += sign2 * cache[d2];
                        }
                    }
                }
                cache[i] = sum;
            }
        }
        cache_size = n;
    }

    return cache[n];
}
