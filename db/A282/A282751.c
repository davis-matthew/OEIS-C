/*
 * A282751  Expansion of phi_{7, 2}(x) where phi_{r, s}(x) = Sum_{n, m>0} m^r
 *          * n^s * x^{m*n}.
 * https://oeis.org/A282751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = p^(2*e) * (p^(5*e+5)-1)/(p^5-1) for each prime factor p^e
 *            of n; multiplicative, a(0)=0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 563, u128 n <= 1000,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Ensure SPF array covers at least up to n. */
static void ensure_spf(idx_t n)
{
    if (n <= spf_limit)
        return;
    if (spf != NULL)
        free(spf);
    build_spf(n, &spf);
    spf_limit = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    ensure_spf(n);

    /* For very narrow seq_t types, use 128‑bit arithmetic
       internally to avoid overflow.  For wider types (including
       arbitrary‑precision integer classes) we operate directly on
       seq_t, which guarantees exactness. */
    if (sizeof(seq_t) <= 8) {
        /* Narrow path: 128‑bit intermediate. */
        idx_t m = n;
        unsigned __int128 result = 1;

        while (m > 1) {
            idx_t p = spf[m];
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            /* p^(2*e) */
            unsigned __int128 p2e = 1;
            for (idx_t i = 0; i < 2 * e; ++i)
                p2e *= (unsigned __int128)p;

            /* Compute ratio = 1 + p^5 + p^10 + ... + p^(5e) */
            unsigned __int128 p5 = 1;
            for (idx_t i = 0; i < 5; ++i)
                p5 *= (unsigned __int128)p;

            unsigned __int128 ratio = 1;
            unsigned __int128 term = 1;
            for (idx_t i = 1; i <= e; ++i) {
                term *= p5;
                ratio += term;
            }

            unsigned __int128 local = p2e * ratio;
            result *= local;
        }
        return (seq_t)result;
    } else {
        /* Wide (or arbitrary‑precision) path: operate directly on seq_t. */
        idx_t m = n;
        seq_t result = (seq_t)1;

        while (m > 1) {
            idx_t p = spf[m];
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            /* p^(2*e) */
            seq_t pval = (seq_t)p;
            seq_t p2e = (seq_t)1;
            for (idx_t i = 0; i < 2 * e; ++i)
                p2e *= pval;

            /* p5 = p^5 */
            seq_t p5 = (seq_t)1;
            for (idx_t i = 0; i < 5; ++i)
                p5 *= pval;

            /* ratio = 1 + p5 + p5^2 + ... + p5^e */
            seq_t ratio = (seq_t)1;
            seq_t term = (seq_t)1;
            for (idx_t i = 1; i <= e; ++i) {
                term *= p5;
                ratio += term;
            }

            seq_t local = p2e * ratio;
            result *= local;
        }
        return result;
    }
}
