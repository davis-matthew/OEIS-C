/*
 * A343250  a(n) is the least k0 <= n such that v_3(n), the 3-adic order of n,
 *          can be obtained by the formula: v_3(n) = log_3(n / L_3(k0, n)),
 *          where L_3(k0, n) is the lowest common denominator of the elements
 *          of the set S_3(k0, n) = {(1/n)*binomial(n, k), with 0 < k <= k0
 *          such that k is not divisible by 3} o...
 * https://oeis.org/A343250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max { p^e : p^e | n, p != 3 } (greatest prime power
 *            divisor of n excluding 3)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is at least 1 by the OEIS offset; tolerate n==0 defensively */
    if (n == 0) return (seq_t)1;

    uint64_t rem = (uint64_t)n;
    seq_t max_power = (seq_t)1;

    /* Handle factor 2 */
    if (rem % 2 == 0) {
        uint64_t power = 1;
        while (rem % 2 == 0) {
            rem /= 2;
            power *= 2;
        }
        if (power > (uint64_t)max_power) max_power = (seq_t)power;
    }

    /* Handle odd factors up to sqrt(rem) */
    for (uint64_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            uint64_t power = 1;
            while (rem % p == 0) {
                rem /= p;
                power *= p;
            }
            /* Skip prime 3 */
            if (p != 3 && power > (uint64_t)max_power) {
                max_power = (seq_t)power;
            }
        }
    }

    /* Remaining rem is a prime > sqrt(original n) */
    if (rem > 1 && rem != 3) {
        if (rem > (uint64_t)max_power) max_power = (seq_t)rem;
    }

    return max_power;
}
