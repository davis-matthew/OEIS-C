/*
 * A369465  Arithmetic derivative of the odd part of n: a(n) =
 *          A003415(A000265(n)).
 * https://oeis.org/A369465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(A000265(n)) = arithmetic derivative of the odd
 *            part of n
 * Range:     u8 n <= 242, u16 n <= 9674, u32 n <= 9674, u64 n <= 9674,
 *            u128 n <= 9674, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* provides factor_u64() */

seq_t a(idx_t n)
{
    /* odd_part(n) : remove all factors of 2 */
    uint64_t m = (uint64_t)n;
    while ((m & 1ULL) == 0ULL)
        m >>= 1;

    if (m <= 1ULL)
        return (seq_t)0;                /* derivative of 1 (or 0) is 0 */

    /* factorization of the odd part */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* arithmetic derivative: m * Σ e_i / p_i
       equivalent to Σ (m / p_i) * e_i  (all integers) */
    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t term = (m / primes[i]) * exps[i];
        result += (seq_t)term;
    }

    return result;
}
