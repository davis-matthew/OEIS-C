/*
 * A328411  Largest m such that (Z/mZ)* = C_2 X C_(2n), or 0 if no such m
 *          exists, where (Z/mZ)* is the multiplicative group of integers
 *          modulo m.
 * https://oeis.org/A328411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest m such that (Z/mZ)* is isomorphic to C_2 × C_(2n)
 * Range:     u8 n <= 17, u16 n <= 4373, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"

seq_t a(idx_t n)
{
    /* maximum n we precompute */
    static const idx_t MAX_N = 20000;
    /* precomputation flag */
    static bool done = false;
    static seq_t *res = NULL;

    if (!done) {
        /* allocate result array */
        res = (seq_t*)calloc((size_t)MAX_N + 1, sizeof(seq_t));

        /* parameters for sieve */
        const uint32_t MAX_K = 4 * MAX_N;      /* maximum totient value needed */
        const uint32_t MMAX   = 1000000;       /* upper bound for m */

        /* allocate arrays for phi and smallest prime factor */
        uint32_t *phi = (uint32_t*)malloc((MMAX + 1) * sizeof(uint32_t));
        uint32_t *spf = (uint32_t*)malloc((MMAX + 1) * sizeof(uint32_t));

        for (uint32_t i = 0; i <= MMAX; ++i) {
            phi[i] = i;
            spf[i] = i;
        }

        /* sieve for phi and smallest prime factor */
        for (uint32_t p = 2; p <= MMAX; ++p) {
            if (spf[p] == p) {                     /* p is prime */
                for (uint32_t j = p; j <= MMAX; j += p) {
                    if (spf[j] == j) spf[j] = p;    /* record smallest prime */
                    phi[j] -= phi[j] / p;
                }
            }
        }

        /* process all m */
        for (uint32_t m = 1; m <= MMAX; ++m) {
            uint32_t k = phi[m];
            if (k % 4 != 0) continue;                 /* we need phi(m) = 4n */
            uint32_t n_val = k / 4;
            if (n_val == 0 || n_val > MAX_N) continue;

            /* compute Carmichael lambda of m */
            uint32_t mm = m;
            uint64_t lambda = 1;
            while (mm > 1) {
                uint32_t p = spf[mm];
                uint32_t e = 0;
                while (mm % p == 0) {
                    mm /= p;
                    ++e;
                }
                uint64_t lam;
                if (p == 2) {
                    if (e == 1) lam = 1;
                    else if (e == 2) lam = 2;
                    else lam = ((uint64_t)1) << (e - 2);
                } else {
                    uint64_t pe_minus1 = 1;
                    for (uint32_t i = 1; i < e; ++i) pe_minus1 *= p;
                    lam = (uint64_t)(p - 1) * pe_minus1;
                }
                lambda = lambda / gcd_uint64(lambda, lam) * lam;
            }

            if ((uint32_t)lambda == k / 2) {            /* λ(m) = 2n */
                if (m > (uint32_t)res[n_val]) res[n_val] = (seq_t)m;
            }
        }

        /* free temporary arrays */
        free(phi);
        free(spf);

        done = true;
    }

    if (n == 0 || n > MAX_N) return 0;
    return res[n];
}
