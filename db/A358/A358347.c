/*
 * A358347  a(n) is the sum of the unitary divisors of n that are squares.
 * https://oeis.org/A358347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e || n, e even} (p^e + 1)
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) is the sum of the unitary divisors of n that are squares. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Use factor_u64 to obtain the prime factorisation of n.
       factor_u64(n, primes, exps, &cnt) writes arrays of primes,
       their exponents, and the count of distinct primes into *cnt. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if ((e & 1) == 0) {                     /* exponent is even */
            seq_t pe = 1;
            for (uint64_t k = 0; k < e; ++k) {
                pe *= (seq_t)primes[i];        /* compute p^e */
            }
            result *= (pe + 1);                /* multiply by (p^e + 1) */
        }
    }
    return result;
}
