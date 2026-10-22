/*
 * A352207  a(1) = 2, a(2) = 48; for n >= 1, a(n+2) = c(n)*a(n+1)^2/a(n) where
 *          c(n) = 256*(2*n+3)^2*(4*n+1)*(4*n+3)^2*(4*n+5) /
 *          (27*(3*n+1)*(3*n+2)^2*(3*n+4)^2*(3*n+5)).
 * https://oeis.org/A352207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(2) = 48; for n ≥ 1: a(n+2) = c(n) * a(n+1)^2 / a(n),
 *            where c(n) = 256·(2n+3)^2·(4n+1)·(4n+3)^2·(4n+5) /
 *            [27·(3n+1)·(3n+2)^2·(3n+4)^2·(3n+5)]
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* `gcd` is provided by the utils library. */
#include <stddef.h>
#include "../utils/gcd.h"

/* memoised, O(n) computation of the sequence.
 * Works for any number of calls in arbitrary order. */
seq_t a(idx_t n)
{
    /* Cache data.  We index from 1; cache[0] is unused. */
    static seq_t *cache   = NULL;   /* array of seq_t values   */
    static size_t cache_len = 0;    /* max index already stored */
    static size_t cache_cap = 0;    /* allocated capacity      */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = (size_t)n + 10;
        if (cache_cap < 10) cache_cap = 10;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = 0;         /* unused */
        cache_len = 0;
    }

    /* Extend the cache if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t idx = cache_len + 1; idx <= (size_t)n; ++idx) {
        seq_t val;
        if (idx == 1) {
            val = (seq_t)2;
        } else if (idx == 2) {
            val = (seq_t)48;
        } else {
            /* recurrence uses n = idx-2 */
            idx_t k = (idx_t)(idx - 2);

            /* previous terms */
            seq_t a_nm1 = cache[idx - 1];   /* a(k+2)   */
            seq_t a_nm2 = cache[idx - 2];   /* a(k+1)   */

            /* Numerator and denominator of c(k) */
            seq_t num = (seq_t)256;
            seq_t two_n3 = (seq_t)(2 * k + 3);
            num *= two_n3 * two_n3;                /* (2k+3)^2 */
            seq_t four_n1 = (seq_t)(4 * k + 1);
            seq_t four_n3 = (seq_t)(4 * k + 3);
            seq_t four_n5 = (seq_t)(4 * k + 5);
            num *= four_n1;
            num *= four_n3 * four_n3;              /* (4k+3)^2 */
            num *= four_n5;                       /* (4k+5)   */

            seq_t den = (seq_t)27;
            seq_t three_n1 = (seq_t)(3 * k + 1);
            seq_t three_n2 = (seq_t)(3 * k + 2);
            seq_t three_n4 = (seq_t)(3 * k + 4);
            seq_t three_n5 = (seq_t)(3 * k + 5);
            den *= three_n1;
            den *= three_n2 * three_n2;            /* (3k+2)^2 */
            den *= three_n4 * three_n4;            /* (3k+4)^2 */
            den *= three_n5;                       /* (3k+5)   */

            /* Reduce the fraction with the surrounding terms.
             * We use a small table of factors and cancel gcds. */
            seq_t num_factors[4] = {a_nm1, a_nm1, num, (seq_t)1};
            seq_t den_factors[3] = {a_nm2, den, (seq_t)1};

            /* cancel all possible gcds between numerator and denominator factors */
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 3; ++j) {
                    seq_t g = gcd(num_factors[i], den_factors[j]);
                    if (g > (seq_t)1) {
                        num_factors[i] /= g;
                        den_factors[j] /= g;
                    }
                }
            }

            /* Multiply remaining numerator factors. */
            val = (seq_t)1;
            for (int i = 0; i < 4; ++i)
                val *= num_factors[i];

            /* Divide by remaining denominator factors. */
            for (int j = 0; j < 3; ++j)
                if (den_factors[j] > (seq_t)1)
                    val /= den_factors[j];
        }

        cache[idx] = val;
    }

    cache_len = (size_t)n;
    return cache[(size_t)n];
}
