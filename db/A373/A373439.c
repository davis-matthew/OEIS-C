/*
 * A373439  Numerator of sum of reciprocals of square divisors of n.
 * https://oeis.org/A373439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{d^2 | n} 1/d^2
 * Range:     u8 n <= 24, u16 n <= 3467, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* offset starts at 1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);   /* factor n */

    seq_t num = 1;
    seq_t den = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t  p  = primes[i];
        uint64_t  e  = exps[i];
        uint64_t  M  = e / 2;                    /* floor(e/2) */

        seq_t seq_p      = (seq_t)p;
        seq_t pow_upper  = big_pow(seq_p, (seq_t)(2 * (M + 1)));   /* p^{2(M+1)} */
        seq_t numeratorA = pow_upper - (seq_t)1;                    /* p^{2(M+1)} - 1 */
        seq_t denomA     = seq_p * seq_p - (seq_t)1;               /* p^2 - 1 */
        seq_t A          = numeratorA / denomA;                    /* exact division */

        num = num * A;

        seq_t powM2 = big_pow(seq_p, (seq_t)(2 * M));               /* p^{2M} */
        den = den * powM2;
    }

    seq_t g   = gcd(num, den);
    num = num / g;                                               /* reduce */
    return num;
}
