/*
 * A378441  Fixed points of A378226, where A378226 is XOR-Moebius transform of
 *          A318457, and A318457(n) = n XOR (sigma(n)-n).
 * https://oeis.org/A378441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that the XOR‑Möbius transform of
 *            A318457 at k equals k, where A318457(m) = m XOR (σ(m) – m)
 * Range:     u8 n <= 10, u16 n <= 65, u32 n <= 6010, u64 n <= 9916,
 *            u128 n <= 9916, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

/* Test whether k is a fixed point of the XOR‑Möbius transform A378226. */
static bool is_fixed(uint64_t k)
{
    uint64_t primes[64], exps[64];
    size_t np = 0;
    factor_u64(k, primes, exps, &np);

    seq_t result = 0;
    uint64_t mask_limit = np ? (1ULL << np) : 1ULL;

    for (uint64_t mask = 0; mask < mask_limit; ++mask) {
        seq_t m     = 1;
        seq_t sigma = 1;
        for (size_t i = 0; i < np; ++i) {
            uint64_t bit   = (mask >> i) & 1U;
            uint64_t exp   = exps[i] - bit;
            seq_t pow_p    = seq_pow((seq_t)primes[i], exp);
            seq_t pow_p1   = seq_pow((seq_t)primes[i], exp + 1);
            seq_t sigma_p  = (pow_p1 - 1) / (primes[i] - 1);
            m     *= pow_p;
            sigma *= sigma_p;
        }
        seq_t term = m ^ (sigma - m);
        result ^= term;
    }
    return result == (seq_t)k;
}

/* Persistent cache of the fixed points found so far. */
static seq_t  *fixed   = NULL;
static size_t  fp_len  = 0;
static size_t  fp_cap  = 0;
static idx_t   checked = 0;

/* Sequence function. */
seq_t a(idx_t n)
{
    if (!fixed) {
        fp_cap = 128;
        fixed  = (seq_t *)malloc(fp_cap * sizeof(seq_t));
    }

    while (fp_len < (size_t)n) {
        ++checked;
        if (is_fixed((uint64_t)checked)) {
            if (fp_len == fp_cap) {
                fp_cap <<= 1;
                fixed = (seq_t *)realloc(fixed, fp_cap * sizeof(seq_t));
            }
            fixed[fp_len++] = (seq_t)checked;
        }
    }
    return fixed[(size_t)n - 1];
}
