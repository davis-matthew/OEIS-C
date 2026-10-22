/*
 * A332845  a(n) = (-1)^omega(n) * Sum_{k=1..n} (-1)^omega(n/gcd(n, k)), where
 *          omega = A001221.
 * https://oeis.org/A332845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e││n} (p^e - 2) (a(1)=1)
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t temp = n;
    seq_t result = (seq_t)1;
    /* handle factor 2 separately for speed */
    if ((temp & 1) == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        seq_t pwr = (seq_t)1;
        for (idx_t i = 0; i < e; ++i)
            pwr *= (seq_t)2;
        result *= (pwr - (seq_t)2);
    }
    /* odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t pwr = (seq_t)1;
            for (idx_t i = 0; i < e; ++i)
                pwr *= (seq_t)p;
            result *= (pwr - (seq_t)2);
        }
    }
    if (temp > 1) { /* remaining prime factor */
        seq_t pwr = (seq_t)temp;
        result *= (pwr - (seq_t)2);
    }
    return result;
}
