/*
 * A309324  Expansion of Sum_{k>=1} psi(k) * x^k/(1 + x^k), where psi =
 *          Dedekind psi function (A001615).
 * https://oeis.org/A309324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1) * psi(d) where psi(d) = d *
 *            Π_{p|d} (1 + 1/p) (Dedekind psi function)
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, psi_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Utility provided by the framework */
#include "../utils/build_spf.h"   /* void build_spf(idx_t limit, idx_t **spf_out); */
#include "../utils/psi_of.h"

/* Trivial integer square‑root (floor) – fast enough for n <= 10 000. */
static idx_t isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) r++;
    return r;
}

/* External linkage function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* no term for n = 0 */

    /* Static cache of smallest prime factors up to the largest n seen so far. */
    static idx_t *spf = NULL;
    static idx_t spf_cap = 0;

    if (n > spf_cap) {                        /* need more data */
        free(spf);
        build_spf(n, &spf);                   /* allocates array of size n+1 */
        spf_cap = n;
    }

    int64_t sum = 0;                          /* signed accumulator */

    idx_t root = isqrt(n);
    for (idx_t i = 1; i <= root; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d = i;
            idx_t nd = n / d;
            int sign = (nd & 1) ? 1 : -1;     /* (-1)^(nd+1) : nd odd -> +1, even -> -1 */
            seq_t psi = psi_of(d, spf);
            sum += sign * (int64_t)psi;

            /* divisor d2 = n / i, if distinct */
            if (d != nd) {
                d = nd;
                nd = n / d;                    /* becomes i */
                sign = (nd & 1) ? 1 : -1;
                psi = psi_of(d, spf);
                sum += sign * (int64_t)psi;
            }
        }
    }
    return (seq_t)sum;
}
