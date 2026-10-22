/*
 * A329701  Number of excursions of length n with Motzkin-steps avoiding the
 *          consecutive steps UH and HU.
 * https://oeis.org/A329701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) satisfies the quadratic functional equation * (x^2 - x^3)
 *            A(x)^2 - (1 - x + x^3) A(x) + 1 = 0 * leading to the recurrence
 *            * a(0) = 1, * a(1) = 1, a(2) = 2, * for n ≥ 1 a(n) = a(n-1) -
 *            a(n-3) * + Σ_{i=0}^{n-2} a(i) * a(n-2-i) * - Σ_{i=0}^{n-3} a(i)
 *            * a(n-3-i) (empty sums are 0). * This recurr...
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 32, u64 n <= 59, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* a(idx_t n) – return the nth term of OEIS A329701 */
extern seq_t a(idx_t n) {
    /* static cache of already‑computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries stored (max index + 1) */

    /* If we haven't computed up to n, extend the cache */
    if (n + 1 > cache_len) {
        /* grow the array */
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        /* initialise the first term if this is the first call */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        /* compute new entries one by one, using the recurrence */
        for (idx_t cur = cache_len; cur <= n; ++cur) {
            seq_t conv_n2 = (seq_t)0;
            if (cur >= 2) {
                for (idx_t i = 0; i + 2 <= cur; ++i) {
                    conv_n2 += cache[i] * cache[cur - 2 - i];
                }
            }

            seq_t conv_n3 = (seq_t)0;
            if (cur >= 3) {
                for (idx_t i = 0; i + 3 <= cur; ++i) {
                    conv_n3 += cache[i] * cache[cur - 3 - i];
                }
            }

            seq_t val = conv_n2 - conv_n3;
            if (cur >= 1) val += cache[cur - 1];
            if (cur >= 3) val -= cache[cur - 3];

            cache[cur] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
