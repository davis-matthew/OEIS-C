/*
 * A278046  Let v = list of denominators of Farey series of order n (see
 *          A006843); a(n) = sum of products of adjacent terms of v.
 * https://oeis.org/A278046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (φ(n) * n²) / 2 + Σ_{k < n, gcd(k,n)=1} k² where
 *            φ is Euler's totient. The formula follows from the fact that
 *            inserting all fractions with denominator n into the Farey
 *            sequence increases the sum by the above Δ(n). The implementation
 *            computes intermediate values iteratively and mem...
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   igcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/igcd.h"

/* idx_t and seq_t are supplied by the build. Keep them generic. */
static seq_t *a_cache = NULL;      /* memoised sequence values */
static idx_t   cache_max = 0;      /* largest n for which a_cache[n] is valid */

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* not defined for n=0 but return 0 for safety */

    /* ensure cache contains at least a[1] */
    if (cache_max < 1) {
        a_cache = (seq_t *)malloc(2 * sizeof(seq_t)); /* indices 0 and 1 */
        a_cache[0] = 0;
        a_cache[1] = (seq_t)1;                       /* a(1) = 1 */
        cache_max = 1;
    }

    /* extend cache up to requested n */
    if (n > cache_max) {
        a_cache = (seq_t *)realloc(a_cache, (size_t)(n + 1) * sizeof(seq_t));
        for (idx_t m = cache_max + 1; m <= n; ++m) {
            seq_t sum_sq = 0;      /* Σ k² over coprime k */
            idx_t  phi   = 0;      /* φ(m) */

            for (idx_t k = 1; k < m; ++k) {
                if (igcd(k, m) == 1) {
                    ++phi;
                    /* k² fits in seq_t comfortably (k ≤ 1e4). */
                    sum_sq += (seq_t)k * (seq_t)k;
                }
            }

            /* Δ(m) = (φ(m) * m²)/2 + sum_sq */
            seq_t delta = ((seq_t)phi * (seq_t)m * (seq_t)m) / 2 + sum_sq;
            a_cache[m] = a_cache[m - 1] + delta;
        }
        cache_max = n;
    }

    return a_cache[n];
}
