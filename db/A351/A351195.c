/*
 * A351195  Sum of the 7th powers of the primes dividing n.
 * https://oeis.org/A351195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p^7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 28, u64 n <= 568,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* factor the integer into distinct primes */
    factor_u64(m, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        seq_t pow7 = (seq_t)p;
        for (int k = 1; k < 7; ++k) {
            pow7 *= (seq_t)p;
        }
        sum += pow7;
    }

    return sum;
}
