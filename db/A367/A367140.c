/*
 * A367140  a(n) = Sum_{prime p|n} p^A001222(n).
 * https://oeis.org/A367140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{prime p|n} p^Ω(n), where Ω(n) is the sum of *
 *            exponents in the prime factorization of n
 * Range:     u8 n <= 27, u16 n <= 135, u32 n <= 1311, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9720 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    /* Factor n.  The arrays hold at most 64 factors for 64‑bit numbers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    /* Compute Ω(n) = Σ e_i. */
    unsigned int omega = 0;
    for (size_t i = 0; i < cnt; ++i)
        omega += (unsigned int)exps[i];

    /* Sum p^Ω(n) over distinct primes p. */
    seq_t sum = (seq_t)0;
    seq_t exp = (seq_t)omega;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        sum += ipow(p, exp);
    }

    return sum;
}
