/*
 * A384401  Consecutive states of the linear congruential pseudo-random number
 *          generator 40014*s mod (2^31-85) when started at s=1.
 * https://oeis.org/A384401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * 40014 mod (2^31-85), a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1; return 0 */
        return 0;

    /* modulus: 2^31 - 85 = 2147483563 */
    const uint64_t MOD_VAL = 2147483563ULL;

    return mod_pow((seq_t)40014ULL, n - 1, (seq_t)MOD_VAL);
}
