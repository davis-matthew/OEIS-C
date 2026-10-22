/*
 * A326126  Sum of all other divisors of n except the squarefree part of n:
 *          a(n) = sigma(n) - A007913(n).
 * https://oeis.org/A326126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - squarefree_part(n) * where squarefree_part(n)
 *            = n / (largest square dividing n) * (product of primes dividing
 *            n to an odd exponent)
 * Range:     u8 n <= 107, u16 n <= 8879, u32 n <= 8879, u64 n <= 8879,
 *            u128 n <= 8879, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, return 0 */

    uint64_t nn = (uint64_t)n;
    uint64_t pr[64];               /* prime factors */
    uint64_t ex[64];               /* exponents of the primes */
    size_t   cnt = 0;              /* number of distinct primes */

    factor_u64(nn, pr, ex, &cnt);  /* factorize nn */

    seq_t sigma = 1;               /* product of sigma(p^e) terms */
    seq_t sqf   = 1;               /* squarefree part (product of primes with odd exponent) */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t term  = 1;           /* sigma term for p^e: 1 + p + p^2 + ... + p^e */
        seq_t p_pow = 1;
        uint64_t e = ex[i];
        for (uint64_t k = 0; k < e; ++k) {
            p_pow = p_pow * (seq_t)pr[i];
            term  = term + p_pow;
        }
        sigma = sigma * term;

        if (e & 1U) {             /* exponent odd: include prime in squarefree part */
            sqf = sqf * (seq_t)pr[i];
        }
    }

    return sigma - sqf;
}
