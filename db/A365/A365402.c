/*
 * A365402  The number of divisors of the largest unitary divisor of n that is
 *          an exponentially odd number.
 * https://oeis.org/A365402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of (e_i + 1) over odd exponents e_i in
 *            factorization of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    // n is at least 1
    uint64_t nn = (uint64_t)n;

    // Allocate arrays for primes and exponents; maximum 64 distinct primes for uint64_t
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(nn, pr, ex, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] & 1ULL) {          // odd exponent
            res *= (seq_t)(ex[i] + 1);
        }
    }
    return res;
}
