/*
 * A384049  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is a unitary divisor of n is cubefree.
 * https://oeis.org/A384049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p^e if e ≤ 2 * p^e – 1 if e ≥ 3)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t result = 1;
    idx_t temp = n;

    /* factor 2 */
    idx_t exp = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++exp;
    }
    if (exp) {
        seq_t pe = 1;
        for (idx_t i = 0; i < exp; ++i) pe *= 2;
        result *= (exp <= 2) ? pe : (pe - (seq_t)1);
    }

    /* factor odd primes */
    for (idx_t d = 3; d * d <= temp; d += 2) {
        if (temp % d == 0) {
            exp = 0;
            while (temp % d == 0) {
                temp /= d;
                ++exp;
            }
            seq_t pe = 1;
            for (idx_t i = 0; i < exp; ++i) pe *= d;
            result *= (exp <= 2) ? pe : (pe - (seq_t)1);
        }
    }

    /* remaining prime factor (exponent 1) */
    if (temp > 1) {
        result *= (seq_t)temp;   /* exp = 1 ≤ 2 */
    }

    return result;
}
