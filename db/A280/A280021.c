/*
 * A280021  Expansion of phi_{11, 2}(x) where phi_{r, s}(x) = Sum_{n, m>0} m^r
 *          * n^s * x^{m*n}.
 * https://oeis.org/A280021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 0 for n = 0 * a(n) = Π_{p^e||n} [ p^(2e) * (1 + p^9 +
 *            p^18 + … + p^(9e)) ] * that is, a(p^e) = p^(2e) * (p^(9(e+1)) -
 *            1) / (p^9-1) * which implements the multiplicative definition
 *            given in the OEIS entry
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 56, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"          /* seq_t big_pow(seq_t base, seq_t exp)      */
#include "../utils/factor_u64.h"       /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset 0 */
        return (seq_t)0;

    /* n == 1 goes through factorisation and returns 1 automatically */
    uint64_t primes[64], exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;          /* multiplicative identity */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p      = (seq_t)primes[i];
        idx_t  e     = (idx_t)exps[i];

        /* p^(2e) */
        seq_t p_pow_2e   = big_pow(p, (seq_t)(2 * e));

        /* geometric series 1 + p^9 + p^18 + … + p^(9e) */
        seq_t sum   = 1;          /* first term */
        seq_t term  = 1;
        seq_t p_pow_9 = big_pow(p, (seq_t)9);

        for (idx_t k = 1; k <= e; ++k) {
            term = term * p_pow_9;
            sum += term;
        }

        /* multiplier for this prime power */
        seq_t mult = p_pow_2e * sum;

        result *= mult;
    }

    return result;
}
