/*
 * A342004  a(n) = 1 if the maximal exponent in the prime factorization of the
 *          arithmetic derivative of n (A003415) is less than the maximal
 *          exponent in n, otherwise 0. a(1) = 1 by convention.
 * https://oeis.org/A342004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the maximal exponent in the prime factorization * of
 *            the arithmetic derivative of n is less than the maximal exponent
 *            * in n, otherwise 0 (a(1)=1)
 * Range:     u8 n <= 778, u16 n <= 9428, u32 n <= 9428, u64 n <= 9428,
 *            u128 n <= 9428, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Special case for n = 1 */
    if (n == (idx_t)1)
        return (seq_t)1;

    uint64_t nix = (uint64_t)n;
    uint64_t primes[64], exps[64];
    size_t cnt;

    /* Factor n to find its prime exponents */
    factor_u64(nix, primes, exps, &cnt);

    /* Determine the maximal exponent in n */
    unsigned max_exp_n = 0;
    for (size_t i = (size_t)0; i < cnt; ++i)
        if (exps[i] > max_exp_n)
            max_exp_n = exps[i];

    /* Compute the arithmetic derivative n' = sum_i e_i * n / p_i */
    seq_t deriv = (seq_t)0;
    for (size_t i = (size_t)0; i < cnt; ++i) {
        seq_t term = ((seq_t)nix / primes[i]) * (seq_t)exps[i];
        deriv += term;
    }

    /* Determine the maximal exponent in the derivative */
    unsigned max_exp_deriv = 0;
    if (deriv != (seq_t)1) {
        uint64_t deriv64 = (uint64_t)deriv;   /* fits for n <= 10⁴ */
        factor_u64(deriv64, primes, exps, &cnt);
        for (size_t i = (size_t)0; i < cnt; ++i)
            if (exps[i] > max_exp_deriv)
                max_exp_deriv = exps[i];
    }
    /* For deriv == 1, max_exp_deriv stays 0 */

    /* Return 1 if max exponent in derivative < max exponent in n, else 0 */
    return (max_exp_deriv < max_exp_n) ? (seq_t)1 : (seq_t)0;
}
