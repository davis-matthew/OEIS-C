/*
 * A365404  The sum of the unitary divisors of the square root of the largest
 *          square dividing n.
 * https://oeis.org/A365404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e‖n, e≥2} (1 + p^{⌊e/2⌋}) (sum of unitary
 *            divisors of √(largest square‖n)) Computes the sum of the unitary
 *            divisors of the square root of the largest square dividing n.
 *            Uses the supplied utilities factor_u64 and pow_int
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9949 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* factorize n (n fits in 64‑bit) */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        /* floor(e/2) is the exponent of the prime in √(largest square) */
        uint64_t t = exps[i] / 2;
        if (t == 0) continue;           /* skip primes with exponent < 2 */
        seq_t term = 1 + ipow_idx((seq_t)primes[i], (idx_t)t);
        result *= term;
    }
    return result;
}
