/*
 * A366126  The largest unitary divisor of n that is a cube.
 * https://oeis.org/A366126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of p^e for each prime p|n with exponent e
 *            divisible by 3; i.e. the largest unitary divisor of n that is a
 *            perfect cube
 * Range:     u8 n <= 342, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_smallest_prime_factor.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; 0 is undefined */
    seq_t result = 1;
    idx_t temp = n;
    while (temp > 1) {
        uint64_t p = _smallest_prime_factor(temp);
        size_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        if (e % 3 == 0) {
            result *= pow_uint(p, (idx_t)e);
        }
    }
    return result;
}
