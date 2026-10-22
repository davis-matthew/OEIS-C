/*
 * A345938  a(n) = uphi(n) / gcd(n-1, uphi(n)), where uphi is unitary totient
 *          (or unitary phi) function, A047994.
 * https://oeis.org/A345938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = uphi(n) / gcd(n-1, uphi(n)), where * uphi(n) =
 *            product_{p^a || n} (p^a - 1) (p^a prime power divisor)
 * Range:     u8 n <= 327, u16 n <= 8617, u32 n <= 8617, u64 n <= 8617,
 *            u128 n <= 8617, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this implementation uses trial division, suitable for the
 * Small < 20000 size of the sequence in the test harness. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Helper: exponentiation by squaring for seq_t. */
static seq_t seq_pow_uint(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1, but return 0 */

    seq_t uphi = 1;
    uint64_t temp = n;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t p_pow = seq_pow_uint((seq_t)p, exp);
            seq_t term = p_pow - 1;
            uphi *= term;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        seq_t term = (seq_t)temp - 1;
        uphi *= term;
    }

    seq_t g = gcd(uphi, (seq_t)(n - 1));
    return uphi / g;
}
