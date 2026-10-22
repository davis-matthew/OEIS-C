/*
 * A385079  Consecutive states of the linear congruential pseudo-random number
 *          generator 228*s mod (2^16+1) when started at s=1.
 * https://oeis.org/A385079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 228*a(n-1) mod (2^16+1), a(1) = 1 Equivalent: a(n) =
 *            228^(n-1) mod 65537
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Modulus 2^16+1 = 65537 */
    const seq_t MOD = (seq_t)65537u;

    /* Base value */
    const seq_t BASE = (seq_t)228u;

    /* a(1) = 1 corresponds to exponent 0 */
    if (n <= 1) return (seq_t)1 % MOD;

    /* Compute 228^(n-1) mod MOD using binary exponentiation */
    idx_t exp = n - 1;
    seq_t result = (seq_t)1 % MOD;
    seq_t power = BASE % MOD;

    while (exp) {
        if (exp & 1u) {
            /* multiply with 64‑bit intermediate to avoid overflow */
            result = (seq_t)(((unsigned long long)result * power) % MOD);
        }
        power = (seq_t)(((unsigned long long)power * power) % MOD);
        exp >>= 1;
    }

    return result;
}
