/*
 * A278908  Multiplicative with a(p^e) = 2^omega(e), where omega = A001221.
 * https://oeis.org/A278908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e||n of 2^ω(e) where ω(e) =
 *            number of distinct prime factors of e
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   omega_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/omega_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but guard */
    seq_t result = 1;
    idx_t temp = n;

    /* factor 2 separately for speed */
    if ((temp & 1) == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        unsigned k = omega_small(e);
        result *= ((seq_t)1) << k;   /* 2^k */
    }

    /* odd factors */
    for (idx_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            idx_t e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }
            unsigned k = omega_small(e);
            result *= ((seq_t)1) << k;
        }
    }

    /* remaining prime factor, if any, has exponent 1 → ω(1)=0 → factor 1 */
    return result;
}
