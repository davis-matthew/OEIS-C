/*
 * A319444  Total number of factors in a factorization of n into Eisenstein
 *          primes.
 * https://oeis.org/A319444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p^e∥n} e * a_p where a(p) = 2 if p = 3 or p ≡ 1 (mod
 *            3), and a(p) = 1 if p ≡ 2 (mod 3). a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function uses the provided primality and factorisation utilities. */
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0 */

    uint64_t m = (uint64_t)n;
    /* Largest possible number of distinct prime factors for n<2^64 is < 64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64 fills primes[], exps[] and sets cnt. */
    factor_u64(m, primes, exps, &cnt);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t weight = (p == 3ULL || p % 3ULL == 1ULL) ? 2ULL : 1ULL;
        result += (seq_t)(e * weight);
    }
    return result;
}
