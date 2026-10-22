/*
 * A385048  The sum of the unitary divisors of n that are cubefull numbers
 *          (A036966).
 * https://oeis.org/A385048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is multiplicative with
 *   a(p^e) = 1          if e ≤ 2
 *   a(p^e) = p^e + 1    if e ≥ 3
 * (a(1) = 1)
 */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* safety guard – not needed for OEIS but harmless */
    if (n == (idx_t)0) return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64: factorises n into primes[] and exps[] */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        unsigned e = (unsigned)exps[i];
        if (e <= 2) continue;          /* contributes 1 */

        seq_t p   = (seq_t)primes[i];
        seq_t pow = (seq_t)1;
        for (unsigned k = 0; k < e; ++k) pow *= p;     /* compute p^e */

        result *= (pow + (seq_t)1);    /* multiply by (p^e + 1) */
    }

    return result;
}
