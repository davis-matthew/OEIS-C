/*
 * A397645  Multiplicative with a(p^e) = (e+1)*(e+2)*(5*e+3)/6.
 * https://oeis.org/A397645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative with a(p^e) = (e+1)*(e+2)*(5*e+3)/6 * *
 *            This implementation factors n and multiplies the local
 *            contributions * for each prime power according to the
 *            definition. The helper * function factor_u64 is used from the
 *            utilities to obtain the prime * factorisation. The result is
 *            com...
 * Range:     u8 n <= 23, u16 n <= 2519, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case (n = 1) automatically */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps[i];               /* exponent as seq_t */
        seq_t term = (e + 1) * (e + 2) * (5 * e + 3) / 6;
        result *= term;
    }

    return result;
}
