/*
 * A354029  a(n) = 1 if either n or n/2 is a prime power of the form 4m+3,
 *          otherwise 0.
 * https://oeis.org/A354029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n or n/2 is a prime power congruent to 3 (mod 4); 0
 *            otherwise
 * Range:     u8 n <= 9991, u16 n <= 9991, u32 n <= 9991, u64 n <= 9991,
 *            u128 n <= 9991, big any n
 * Helpers:   is_prime_power_4m3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_power_4m3.h"

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;

    if ((n64 & 1U) == 0) {             /* even n: test n/2 */
        uint64_t half = n64 / 2;
        return (seq_t)(is_prime_power_4m3(half) ? 1 : 0);
    } else {                           /* odd n: test n itself */
        return (seq_t)(is_prime_power_4m3(n64) ? 1 : 0);
    }
}
