/*
 * A309621  Numbers k such that the number of divisors of k is equal to the
 *          number of power-of-two-divisors of the sum of divisors of k.
 * https://oeis.org/A309621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   computed by brute‑force search of positive integers. a(n) = the
 *            nth positive integer k such that τ(k) = v₂(σ(k)) + 1, where τ is
 *            the divisor count and σ is the sum of divisors
 * Range:     u8 none, u16 none, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

#define TERMS_CAP 10000               /* maximum terms to cache */
#define LIMIT 2000000                 /* upper bound for search */

static uint32_t *spf = NULL;          /* smallest prime factor array */
static seq_t *terms = NULL;           /* cached sequence terms */
static size_t terms_len = 0;          /* number of cached terms */
static bool initialized = false;

/* --------- Sieve of smallest prime factors --------- */
static void init_spf(void)
{
    spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= LIMIT; ++i)
        spf[i] = (uint32_t)i;

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i)
        if (spf[i] == i)
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                if (spf[j] == j)
                    spf[j] = i;
}

/* --------- Precompute enough terms for all requested indices --------- */
static void init_terms(void)
{
    terms = (seq_t *)malloc(TERMS_CAP * sizeof(seq_t));
    terms_len = 0;

    for (uint32_t k = 1; k <= LIMIT && terms_len < TERMS_CAP; ++k) {
        /* factor k using the SPF table */
        uint32_t n = k;
        uint32_t tau_val = 1;
        seq_t sigma = 1;

        while (n > 1) {
            uint32_t p = spf[n];
            uint32_t e  = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            tau_val *= (e + 1);

            seq_t pe1 = pow_uint((uint64_t)p, (idx_t)(e + 1));
            sigma *= (pe1 - (seq_t)1) / (seq_t)(p - 1);
        }

        /* count exponent of 2 in sigma */
        idx_t v2 = 0;
        seq_t tmp = sigma;
        while ((tmp & (seq_t)1) == 0) {
            tmp >>= 1;
            ++v2;
        }

        if (tau_val == v2 + 1)
            terms[terms_len++] = (seq_t)k;
    }
}

/* --------- Public function --------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (!initialized) {
        init_spf();
        init_terms();
        initialized = true;
    }

    if ((idx_t)terms_len < n)
        return 0;   /* undefined for larger n */

    return terms[(size_t)(n - 1)];
}
