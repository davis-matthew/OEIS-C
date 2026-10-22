/*
 * A341772  a(n) = Sum_{d|n} phi(d) * J_2(n/d).
 * https://oeis.org/A341772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Σ_{d|n} φ(d) · J₂(n/d) For n ≥ 1. The function computes
 *            the sequence values exactly
 * Range:     u8 n <= 14, u16 n <= 224, u32 n <= 660, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0, return 0 */

    /* ---------- φ array ----------
       φ(i) = i initially, then subtract multiples. */
    seq_t *phi = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!phi) return 0;
    for (idx_t i = 1; i <= n; ++i) phi[i] = (seq_t)i;

    /* ---------- Smallest prime factor (spf) array ----------
       spf[i] = smallest prime factor of i (0 means unset). */
    uint32_t *spf = (uint32_t *)calloc((size_t)n + 1, sizeof(uint32_t));
    if (!spf) { free(phi); return 0; }

    for (idx_t i = 2; i <= n; ++i)
    {
        if (spf[i] == 0)              /* i is prime */
        {
            spf[i] = (uint32_t)i;
            if (i * i <= n)
            {
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    /* Update φ for each prime p: φ[m] -= φ[m]/p for all multiples m of p. */
    for (idx_t i = 2; i <= n; ++i)
    {
        if (spf[i] == i)            /* i is prime */
        {
            for (idx_t j = i; j <= n; j += i)
                phi[j] -= phi[j] / (seq_t)i;
        }
    }

    /* ---------- Jordan totient J₂ array ----------
       J₂(k) = k² · Π_{p|k} (1 - 1/p²).  Compute multiplicatively. */
    seq_t *j2 = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!j2) { free(phi); free(spf); return 0; }
    j2[0] = 0;
    j2[1] = 1;

    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t val = (seq_t)k * (seq_t)k;      /* initial k² */
        idx_t tmp = k;
        while (tmp > 1)
        {
            uint32_t p = spf[tmp];
            while (tmp % p == 0) tmp /= p;
            seq_t p2 = (seq_t)p * (seq_t)p;
            val = val * (p2 - (seq_t)1) / p2; /* multiply by (p²‑1)/p² */
        }
        j2[k] = val;
    }

    /* ---------- Sum over divisors ----------
       For each divisor d of n add φ(d) * J₂(n/d). */
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t d2 = n / d;
            result += phi[d] * j2[d2];
            if (d2 != d)
                result += phi[d2] * j2[d];
        }
    }

    /* ---------- Clean up ----------
       All temporary allocations are freed. */
    free(phi);
    free(j2);
    free(spf);
    return result;
}
