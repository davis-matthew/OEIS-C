/*
 * A350388  a(n) is the largest unitary divisor of n that is a square.
 * https://oeis.org/A350388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of p^e for all prime powers p^e dividing n with e
 *            even (i.e., the largest unitary square divisor of n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: integer power for indices (p^e), e is even. */
static seq_t pow_uint64(uint64_t base, uint64_t exp)
{
    seq_t r = 1;
    for (uint64_t i = 0; i < exp; ++i)
        r *= (seq_t)base;
    return r;
}

/* Main function: largest unitary divisor that is a square. */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined in OEIS but guard anyway */
        return 0;

    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e % 2 == 0) {
                result *= pow_uint64((uint64_t)p, e);
            }
        }
    }

    /* If a prime larger than sqrt(n) remains, its exponent is 1 (odd),
       so it contributes nothing to the square divisor. */
    return result;
}
