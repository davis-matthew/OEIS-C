/*
 * A326053  Sum of all other divisors of n except the largest square divisor:
 *          a(n) = sigma(n) - A008833(n).
 * https://oeis.org/A326053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) – largest_square_divisor(n)
 * Range:     u8 n <= 16, u16 n <= 3137, u32 n <= 9698, u64 n <= 9698,
 *            u128 n <= 9698, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* n is 1-based; for all n >= 1. */
    if (n == 0) /* safety for out-of-range calls */
        return (seq_t)0;

    /* Buffers for prime factorization – enough for 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* sigma(n) and largest square divisor initialize to 1. */
    seq_t sigma  = (seq_t)1;
    seq_t square = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* σ part: (p^(e+1) – 1) / (p – 1) */
        seq_t p_pow_e1   = seq_pow(p, (idx_t)(e + 1));
        seq_t sigma_part = (p_pow_e1 - (seq_t)1) / (p - (seq_t)1);
        sigma *= sigma_part;

        /* largest square divisor part: p^(2 * ⌊e/2⌋) */
        uint64_t exp_sq  = 2 * (e / 2);
        seq_t   p_pow_sq = seq_pow(p, (idx_t)exp_sq);
        square *= p_pow_sq;
    }

    return sigma - square;
}
