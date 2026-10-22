/*
 * A392275  The maximum exponent in the prime factorization of the smallest
 *          number whose cube is divisible by n.
 * https://oeis.org/A392275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling( max exponent among prime factors of n / 3 )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* n = 1 gives a(1) = 0 */
    uint64_t tmp = (uint64_t)n;
    uint64_t maxExp = 0;

    /* trial division up to sqrt(tmp) */
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (e > maxExp) maxExp = e;
        }
    }

    /* remaining prime factor (if any) has exponent 1 */
    if (tmp > 1 && 1 > maxExp) maxExp = 1;

    /* ceil(maxExp / 3) = (maxExp + 2) / 3 */
    return (seq_t)((maxExp + 2) / 3);
}
