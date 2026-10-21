/*
 * A284900  a(n) = Sum_{d|n} (-1)^(n/d+1)*d^4.
 * https://oeis.org/A284900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1) * d^4 a(1) = 1 (multiplicative
 *            with a(2^e) = (7*2^(4e+1)+1)/15, a(p^e) = p^4 + p^8 + ... +
 *            p^(4e) for odd prime p)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;   /* meaningless, but safe */
    if (n == 1u) return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;

    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t res = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        size_t  e = (size_t)exps[i];
        seq_t   term;

        if (p == 2u) {
            /* term = (7*2^(4e+1)+1)/15 */
            seq_t pow_val = pow_uint(2, 4 * (idx_t)e + 1);
            term = pow_val * 7 + 1;
            term = term / 15;
        } else {
            /* term = 1 + p^4 + p^8 + ... + p^(4e) */
            seq_t x   = pow_uint(p, 4);
            seq_t sum = 1;
            seq_t pow_val = 1;
            for (size_t j = 1; j <= e; ++j) {
                pow_val = pow_val * x;
                sum += pow_val;
            }
            term = sum;
        }

        res = res * term;
    }

    return res;
}
