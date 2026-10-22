/*
 * A366247  The number of infinitary divisors of n that are "Fermi-Dirac
 *          primes" (A050376) and terms of A366243.
 * https://oeis.org/A366247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p^e‖n} b(e) with b(e) = 1 if e == 2 or e % 3 == 0,
 *            otherwise 0. This implements the additive rule a(p^e) =
 *            A139352(e)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t primes[32];
    uint64_t exps[32];
    size_t   cnt = 0;

    /* factorise n */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if (e == 2 || e % 3 == 0)
            sum += (seq_t)1;
    }
    return sum;
}
