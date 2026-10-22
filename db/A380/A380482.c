/*
 * A380482  a(n) is the multiplicative order of -3 modulo prime(n); a(2) = 0
 *          for completion.
 * https://oeis.org/A380482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = multiplicative order of -3 modulo the n-th prime (a(2)=0)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize, get_prime, mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/factorize.h"
#include "../utils/mul_mod64.h"

static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* safety, although n>=1 by spec */

    uint32_t p = get_prime(n);           /* n-th prime */
    if (p == 3)                          /* -3 ≡ 0 (mod 3) → undefined order */
        return (seq_t)0;

    uint64_t base;
    if (p == 2)
        base = 1;                        /* -3 ≡ 1 (mod 2) */
    else
        base = (uint64_t)p - 3;          /* positive residue of -3 */

    uint64_t ord = (uint64_t)p - 1;      /* exponent divides p-1 */

    /* factorize p-1 */
    uint64_t primes[64];
    int exps[64];
    size_t cnt = factorize(ord, primes, exps);

    /* reduce order by checking prime factors */
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t q = primes[i];
        while ((ord % q == 0) &&
               (powmod_uint64(base, ord / q, (uint64_t)p) == 1))
        {
            ord /= q;
        }
    }

    return (seq_t)ord;
}
