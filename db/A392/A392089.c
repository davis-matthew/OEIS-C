/*
 * A392089  The unitary totient of the smallest powerful number that is
 *          divisible by n.
 * https://oeis.org/A392089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product_{p^e||n} (p^max(e,2) - 1) (unitary totient of the
 *            smallest powerful multiple of n)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety guard – sequence starts at n=1 */
    seq_t result = 1;
    /* Factor n.  The helper allocates primes[64] and exps[64] where the
       prime factors and their exponents are stored.  It automatically
       uses an internal list of small primes for trial division. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (e < 2) e = 2;                     /* ensure exponent >= 2 */
        seq_t term = big_pow((seq_t)p, (seq_t)e) - 1;
        result *= term;                       /* multiplicative property */
    }
    return result;
}
