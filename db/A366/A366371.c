/*
 * A366371  Number of divisors of n that are either an odd prime power > 1 or
 *          twice such a number.
 * https://oeis.org/A366371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p odd prime | n} exp_p * (e0 ≥ 1 ? 2 : 1) where n =
 *            2^e0 * ∏ p_i^{exp_i}. This counts all divisors of n that are
 *            either an odd prime power > 1 or twice such a number
 * Range:     u8 n <= 9853, u16 n <= 9853, u32 n <= 9853, u64 n <= 9853,
 *            u128 n <= 9853, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                    /* defined for n ≥ 1, safety guard */
        return 0;

    idx_t temp = n;
    seq_t sum = 0;

    /* count exponent of 2 */
    idx_t e0 = 0;
    while ((temp & 1) == 0) {
        e0++;
        temp >>= 1;
    }

    /* factor the odd part */
    idx_t p = 3;
    while (p <= temp / p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                exp++;
                temp /= p;
            }
            sum += (seq_t)exp;
        }
        p += 2;  /* skip even numbers */
    }
    if (temp > 1)         /* remaining prime > 1 with exponent 1 */
        sum += 1;

    seq_t result = sum;
    if (e0 > 0)
        result *= 2;      /* double the count for even n */

    return result;
}
