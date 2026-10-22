/*
 * A351248  a(n) = n^8 * Sum_{p|n, p prime} 1/p^8.
 * https://oeis.org/A351248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^8 * Sum_{p|n} 1/p^8 = Sum_{p|n} (n/p)^8
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 31, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow_uint, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/smallest_prime_factor.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    uint64_t n0 = (uint64_t)n;
    uint64_t temp = n0;
    while (temp > 1) {
        uint64_t p = smallest_prime_factor(temp);
        sum += pow_uint(n0 / p, (idx_t)8);
        while (temp % p == 0) temp /= p;
    }
    return sum;
}
