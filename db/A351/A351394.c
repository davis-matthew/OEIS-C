/*
 * A351394  Number of divisors of n that are either squarefree, prime powers,
 *          or both.
 * https://oeis.org/A351394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^ω(n) + Σ e(p) – ω(n) where ω(n) is the number of
 *            distinct prime factors of n and e(p) is the exponent of each
 *            prime p in n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, twopow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides factor_u64() */
#include "../utils/twopow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined only for n≥1 */

    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;                     /* number of distinct primes */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* ω(n) = cnt,  Σ e(p) */
    unsigned omega = (unsigned)cnt;
    seq_t sum_exp = 0;
    for (size_t i = 0; i < cnt; ++i) sum_exp += (seq_t)exps[i];

    /* a(n) = 2^ω(n) + Σ e(p) – ω(n) */
    seq_t result = twopow(omega) + sum_exp - (seq_t)omega;
    return result;
}
