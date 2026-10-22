/*
 * A359592  Parity (and also absolute values) of Dirichlet inverse of A035263,
 *          where A035263(n) is parity of 2-adic valuation of 2n.
 * https://oeis.org/A359592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if the exponent of 2 in n is 0 or 2 and each odd prime
 *            * factor occurs to the first power; otherwise a(n) = 0. *
 *            Multiplicative with a(2^e) = 1 if e = 2 else 0, * a(p^e) = 1 if
 *            e = 1 else 0
 * Range:     u8 n <= 9716, u16 n <= 9716, u32 n <= 9716, u64 n <= 9716,
 *            u128 n <= 9716, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;            /* a(1) = 1 */

    uint64_t uint_n = (uint64_t)n;

    /* Maximum distinct prime factors for 64‑bit n is < 12,
       but we reserve 64 slots for safety. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    /* factor_u64 returns primes, exponents and count. */
    factor_u64(uint_n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        if (primes[i] == 2ULL) {
            /* for 2 the exponent must be exactly 2 */
            if (exps[i] != 2ULL) return (seq_t)0;
        } else {
            /* for odd primes the exponent must be exactly 1 */
            if (exps[i] != 1ULL) return (seq_t)0;
        }
    }
    return (seq_t)1;
}
