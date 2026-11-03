/*
 * A349920  Dirichlet g.f.: Product_{k>=2} (1 - mu(k)^2 * k^(-s)).
 * https://oeis.org/A349920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Dirichlet g.f.: ∏_{k≥2} (1 - μ(k)² k^{-s})
 * Range:     i8 n <= 20000, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 117 terms through n = 9793 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as the coefficients of this Dirichlet series.   */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static buffers */
    static seq_t  *cache  = NULL;            /* a[1..max]    */
    static unsigned char *mu2  = NULL;      /* 1 if square‑free, else 0 */
    static idx_t  max_cache = 0;             /* largest n cached */

    /* extend cache if required */
    if (n > max_cache)
    {
        idx_t N = n;

        /* allocate new buffers */
        cache = mem_resize(seq_t, cache, (N + 1));
        mu2   = (unsigned char *)realloc(mu2, (N + 1) * sizeof(unsigned char));

        /* initialise arrays (no value needed for index 0) */
        for (idx_t i = 0; i <= N; ++i) {
            cache[i] = 0;
            mu2[i]   = 1;            /* presume square‑free */
        }
        mu2[0] = 0; mu2[1] = 1;    /* μ²(1)=1 but unused */

        /* mark non‑square‑free numbers: those divisible by p² */
        for (idx_t p = 2; p * p <= N; ++p) {
            idx_t pp = p * p;
            for (idx_t j = pp; j <= N; j += pp)
                mu2[j] = 0;
        }

        /* Dirichlet series coefficients: a[1] = 1.
           Each square‑free k contributes (1 - k^{-s}).  */
        cache[1] = 1;
        for (idx_t k = 2; k <= N; ++k)
        {
            if (!mu2[k]) continue;          /* skip non‑square‑free k */
            for (idx_t m = N; m >= k; --m)
            {
                if ((m % k) == 0)
                {
                    /* multiply by (1 - k^{-s}) → subtract contribution */
                    cache[m] -= cache[m / k];
                }
            }
        }

        max_cache = N;
    }

    return cache[n];
}
