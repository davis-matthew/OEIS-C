/*
 * A292919  Sum of n-th powers of odd divisors of n.
 * https://oeis.org/A292919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   Sum of n-th powers of odd divisors of n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, factor_uint, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/pow_uint.h"
#include "../utils/factor_uint.h"

/* Σ_{d|m} d^n_exp, where n_exp is the index exponent */
static seq_t sigma_for_n(uint64_t m, uint64_t n_exp)
{
    uint64_t primes[64];
    int exps[64];
    size_t cnt;
    factor_uint(m, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base   = big_pow((seq_t)primes[i], (seq_t)n_exp);
        seq_t term   = 1;
        seq_t partial = 1;
        for (int k = 1; k <= exps[i]; ++k) {
            term    = term * base;
            partial = partial + term;
        }
        result = result * partial;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* sequence is defined for n ≥ 1 */
    uint64_t nu = (uint64_t)n;

    seq_t sigma_n = sigma_for_n(nu, nu);          /* Σ_{d|n} d^n  */
    if ((nu & 1ULL) == 1) return sigma_n;        /* n odd → all divisors odd */

    seq_t sigma_half = sigma_for_n(nu / 2, nu);   /* Σ_{d|n/2} d^n   */
    seq_t pow2_n     = pow_uint(2, nu);           /* 2^n             */
    seq_t sub        = pow2_n * sigma_half;       /* contribution of even divisors */
    return sigma_n - sub;
}
