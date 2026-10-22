/*
 * A384051  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is a unitary divisor of n is a cubefull number.
 * https://oeis.org/A384051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = number of k (1 ≤ k ≤ n) such that the greatest divisor of
 *            k that is a unitary divisor of n is a cubefull number (i.e. each
 *            prime dividing it has exponent ≥ 3)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/factor_u64.h"

static unsigned long long pow_uint64(unsigned long long base, unsigned int exp)
{
    unsigned long long res = 1;
    while (exp--) res *= base;
    return res;
}

seq_t a(idx_t n)
{
    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);

    /* Pre‑compute p^e for each prime factor */
    unsigned long long pow_vals[64];
    for (size_t i = 0; i < pcnt; ++i)
        pow_vals[i] = pow_uint64(primes[i], (unsigned int)exps[i]);

    /* Indices of primes whose exponent is at least 3 (possible to appear
       in a cubefull unitary divisor). */
    size_t bigIdx[64];
    size_t bigCnt = 0;
    for (size_t i = 0; i < pcnt; ++i)
        if (exps[i] >= 3) bigIdx[bigCnt++] = i;

    seq_t total = 0;

    /* Iterate over all subsets of such primes: each subset defines one
       cubefull unitary divisor d = ∏ p_i^e_i. */
    for (uint64_t bm = 0; bm < (1ULL << bigCnt); ++bm)
    {
        unsigned long long L = 1;            /* product for this divisor */
        unsigned char included[64];
        memset(included, 0, sizeof(included));

        for (size_t j = 0; j < bigCnt; ++j)
        {
            if (bm & (1ULL << j))
            {
                size_t idx = bigIdx[j];
                L *= pow_vals[idx];
                included[idx] = 1;
            }
        }

        /* Build list of prime powers for primes NOT in this divisor */
        unsigned long long compPow[64];
        size_t compCnt = 0;
        for (size_t i = 0; i < pcnt; ++i)
            if (!included[i])
                compPow[compCnt++] = pow_vals[i];

        /* Inclusion–exclusion on complements */
        unsigned long long cnt = n / L;          /* multiples of L */
        for (uint64_t cmask = 1; cmask < (1ULL << compCnt); ++cmask)
        {
            unsigned long long mul = 1;
            for (size_t k = 0; k < compCnt; ++k)
                if (cmask & (1ULL << k))
                    mul *= compPow[k];

            unsigned long long t = L * mul;
            if (t == 0) continue;               /* overflow guard, shouldn't happen */
            unsigned long long add = n / t;
            if (__builtin_popcountll(cmask) & 1)
                cnt -= add;
            else
                cnt += add;
        }

        total += (seq_t)cnt;
    }

    return total;
}
