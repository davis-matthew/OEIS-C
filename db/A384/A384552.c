/*
 * A384552  Consecutive states of the linear congruential pseudo-random number
 *          generator 3934873077*s mod 2^32 when started at s=1.
 * https://oeis.org/A384552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3934873077 * a(n-1) mod 2^32, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul_32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_mul_32.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* out of range, but handled safely */

    uint32_t res  = 1;            /* multiplier^{n-1} */
    uint32_t base = 3934873077u;  /* 2^32 modulus multiplier */

    idx_t e = n - 1;
    while (e) {
        if (e & 1)
            res = mod_mul_32(res, base);   /* res = res * base mod 2^32 */
        base = mod_mul_32(base, base);     /* base = base^2 mod 2^32 */
        e >>= 1;
    }
    return (seq_t)res;           /* finally, a(n) = multiplier^{n-1} mod 2^32 */
}
