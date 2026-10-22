/*
 * A385136  The sum of divisors d of n such that n/d is a cubefull number
 *          (A036966).
 * https://oeis.org/A385136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum of divisors d of n such that n/d is cubefull.
 *            Sequence is multiplicative with a(p) = p and a(p^e) = (p^(e+1) -
 *            p^e + p^(e-2) - 1) / (p-1) for e ≥ 2
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /*  only defined for n ≥ 1 */
    if (n == 1) return 1;         /* a(1) = 1 */

    /* factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        size_t   e  = (size_t)exps[i];
        seq_t    a_p;

        if (e == 1) {
            a_p = (seq_t)p;
        } else {
            /* compute p^e, p^(e+1), p^(e-2) */
            seq_t p_e    = 1;
            for (size_t j = 0; j < e; ++j) p_e *= p;
            seq_t p_e1   = p_e * p;          /* p^(e+1) */
            seq_t p_em2  = 1;
            if (e >= 2) {
                for (size_t j = 0; j < e - 2; ++j) p_em2 *= p;   /* p^(e-2) */
            }
            seq_t numerator   = p_e1 - p_e + p_em2 - (seq_t)1;
            seq_t denominator = p - (seq_t)1;
            a_p = numerator / denominator;
        }

        res *= a_p;
    }

    return res;
}
