/*
 * A355825  a(n) = 1 if all exponents in prime factorization of n have an odd
 *          binary weight, otherwise 0.
 * https://oeis.org/A355825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 if all exponents in the prime factorization of n have
 *            an odd binary weight, else 0
 * Range:     u8 n <= 9719, u16 n <= 9719, u32 n <= 9719, u64 n <= 9719,
 *            u128 n <= 9719, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Special case n = 1: no prime factors, vacuously true */
    if (n == 1)
        return (seq_t)1;

    /* Prime factorization: at most 64 distinct primes for uint64_t */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        /* Compute binary weight (popcount) */
        unsigned pc = 0;
        while (e) {
            pc++;
            e &= e - 1;          /* clear lowest set bit */
        }
        if ((pc & 1) == 0)      /* even popcount → return 0 */
            return (seq_t)0;
    }

    return (seq_t)1;
}
