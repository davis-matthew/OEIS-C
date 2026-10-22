/*
 * A342460  a(n) = 1 if n > 1 and is divisible by the sum of its prime factors
 *          (with repetition), otherwise 0.
 * https://oeis.org/A342460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n > 1 and n is divisible by the sum of its prime
 *            factors (with multiplicity), otherwise 0
 * Range:     u8 n <= 9447, u16 n <= 9447, u32 n <= 9447, u64 n <= 9447,
 *            u128 n <= 9447, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }
    uint64_t m = (uint64_t)n;
    uint64_t sum = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            sum += p * cnt;
        }
    }
    if (m > 1) {
        sum += m;
    }
    return ((uint64_t)n % sum == 0) ? (seq_t)1 : (seq_t)0;
}
