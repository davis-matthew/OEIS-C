/*
 * A328827  a(n) is the largest prime factor of n + n*(n+1)/2 = n*(n+3)/2.
 * https://oeis.org/A328827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of n*(n+3)/2
 * Range:     u8 n <= 253, u16 n <= 9242, u32 n <= 9242, u64 n <= 9242,
 *            u128 n <= 9242, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The largest prime factor of n*(n+3)/2 is the maximum of the
 * largest prime factors of n and of n+3 (except for the factor 2
 * which is removed by the division by 2). */

#include <stdint.h>
#include "../utils/largest_prime_divisor.h"   /* declares uint64_t largest_prime_divisor(uint64_t) */

seq_t a(idx_t n)
{
    /* Convert the index to a 64‑bit value for the helper.  The
     * largest_prime_divisor helper works with 64‑bit inputs.  We
     * assume that n is small enough that n+3 does not overflow a
     * 64‑bit unsigned integer – the test suite does not exercise
     * values close to the 64‑bit limit. */
    uint64_t v = (uint64_t)n;
    uint64_t v1 = v;        /* n */
    uint64_t v2 = v + 3;    /* n+3 */

    /* Compute largest prime factor of v1 and v2.  The helper
     * returns 1 for the input value 1, which is fine for the
     * computation because 1 has no prime factors. */
    uint64_t lpf1 = (v1 > 1) ? largest_prime_divisor(v1) : 1;
    uint64_t lpf2 = (v2 > 1) ? largest_prime_divisor(v2) : 1;

    /* Return the larger of the two.  The result fits in seq_t
     * because the largest prime factor of a 64‑bit number
     * certainly fits in a 64‑bit value, and seq_t is at least
     * 64 bits in all supported builds. */
    return (lpf1 > lpf2) ? (seq_t)lpf1 : (seq_t)lpf2;
}
