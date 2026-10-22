/*
 * A369893  a(n) = Sum_{p|n, p prime} n^tau(n/p).
 * https://oeis.org/A369893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{p|n, p prime} n^{τ(n/p)} (ω(1)=0)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 35, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
/* big_pow: exponentiation for seq_t */
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)0;          /* a(1)=0, a(0) is out of range */

    /* Factorize n: primes[] & exps[] hold distinct primes and their exponents */
    uint64_t primes[64];
    uint32_t exps[64];
    size_t pcnt = 0;
    idx_t t = n;

    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint32_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (t > 1) {               /* remaining prime factor */
        primes[pcnt] = t;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Compute τ(n) = ∏ (e_i + 1) */
    uint64_t tauN = 1;
    for (size_t i = 0; i < pcnt; ++i)
        tauN *= (uint64_t)(exps[i] + 1);

    seq_t sum = (seq_t)0;
    seq_t base = (seq_t)n;

    for (size_t i = 0; i < pcnt; ++i) {
        uint32_t e = exps[i];
        /* τ(n/p) = τ(n) * e / (e+1) */
        uint64_t tauByP = (tauN / (uint64_t)(e + 1)) * (uint64_t)e;
        seq_t term = big_pow(base, (seq_t)tauByP);
        sum += term;
    }
    return sum;
}
