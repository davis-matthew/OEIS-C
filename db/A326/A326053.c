/*
 * A326053  Sum of all other divisors of n except the largest square divisor:
 *          a(n) = sigma(n) - A008833(n).
 * https://oeis.org/A326053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) – largest_square_divisor(n)
 * Range:     u8 n <= 16, u16 n <= 3137, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 99 terms through n = 9698 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"

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
        seq_t p_pow_e1   = ipow_idx(p, (idx_t)(e + 1));
        seq_t sigma_part = (p_pow_e1 - (seq_t)1) / (p - (seq_t)1);
        sigma *= sigma_part;

        /* largest square divisor part: p^(2 * ⌊e/2⌋) */
        uint64_t exp_sq  = 2 * (e / 2);
        seq_t   p_pow_sq = ipow_idx(p, (idx_t)exp_sq);
        square *= p_pow_sq;
    }

    return sigma - square;
}
