/*
 * A365488  The number of divisors of the smallest number whose cube is
 *          divisible by n.
 * https://oeis.org/A365488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Number of divisors of the smallest integer m whose cube
 *            is divisible by n. The multiplicative definition is a(p^e) =
 *            ceil(e/3) + 1. The implementation factors n, computes ceil(e/3)
 *            for each prime power, and multiplies the resulting divisor
 *            counts
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == (idx_t)0) return 0;          /* defensive, although n>=1 */
    uint64_t x = (uint64_t)n;

    /* Allocate arrays large enough to hold the prime factorisation of any
       64‑bit integer (at most 64 distinct primes). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(x, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        /* ceil(e/3) = (e + 2)/3 for integer division */
        seq_t term = (seq_t)(((e + 2) / 3) + 1);
        result *= term;
    }
    return result;
}
