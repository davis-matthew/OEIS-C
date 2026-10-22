/*
 * A307000  Number of unitary rings with additive group (Z/nZ)^2.
 *          Equivalently, number of unitary commutative rings with additive
 *          group (Z/nZ)^2.
 * https://oeis.org/A307000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative with * a(p^e) = 2*e + 1 for odd primes p,
 *            * a(2^1) = 3, * a(2^e) = 4*e - 2 for e >= 2. * a(1) = 1
 * Range:     u8 n <= 839, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    if (n == 0)               /* below offset – undefined, return 0 */
        return 0;
    if (n == 1)               /* a(1) = 1 */
        return 1;

    /* Factor n; factor_u64 fills arrays of primes and exponents. */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  c = 0;
    factor_u64(nn, primes, exps, &c);

    seq_t result = 1;
    for (size_t i = 0; i < c; ++i) {
        seq_t term;
        if (primes[i] == 2) {
            if (exps[i] == 1)
                term = 3;                     /* a(2) */
            else
                term = (seq_t)(4 * exps[i] - 2); /* a(2^e), e >= 2 */
        } else {
            term = (seq_t)(2 * exps[i] + 1);   /* a(p^e) for odd p */
        }
        result *= term;
    }
    return result;
}
