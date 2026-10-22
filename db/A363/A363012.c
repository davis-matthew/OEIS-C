/*
 * A363012  a(n) = Sum_{d|n} d*tau(d)*phi(d)*mu(n/d).
 * https://oeis.org/A363012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} a(p^e), where a(p) = 2·p² − 2·p − 1, and for
 *            e>1 : a(p^e) = ((p²−1)·e + p²) · (p−1) · p^(2e−3)
 *            (multiplicative function)
 * Range:     u8 n <= 12, u16 n <= 134, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* Factor n: primes[] holds prime bases, exps[] their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];
        seq_t part;

        if (e == 1) {
            /* a(p) = 2p² – 2p – 1 */
            part = 2 * p * p - 2 * p - 1;
        } else {
            /* a(p^e) = ((p²−1)·e + p²) · (p−1) · p^(2e−3) */
            seq_t p2 = p * p;                         /* p² */
            seq_t term1 = (p2 - 1) * e + p2;          /* ((p²−1)·e + p²) */
            seq_t term2 = p - 1;                      /* (p−1) */
            seq_t pow   = big_pow(p, 2 * e - 3);      /* p^(2e−3) */
            part = term1 * term2 * pow;
        }
        result *= part;
    }

    return result;
}
