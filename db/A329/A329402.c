/*
 * A329402  Number of rectangles (w X h, w <= h) with integer side lengths w
 *          and h having area = n * perimeter.
 * https://oeis.org/A329402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (d(4*n*n) + 1) / 2, where d(x) is the divisor count of x.
 *            Derived from (w-2n)(h-2n) = 4n^2, counting unordered factor
 *            pairs of 4n^2
 * Range:     u8 n <= 4619, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0, but return 0 for safety */
    idx_t nn = n;                    /* work on a copy of n for factorisation */
    idx_t e2 = 0;                    /* exponent of prime 2 in n */
    while ((nn & 1) == 0) {          /* remove factors of 2 */
        e2++;
        nn >>= 1;
    }

    /* divisor count of 4*n^2:
       2's exponent → 2*e2 + 2, multiplier (2*e2 + 3)
       other primes p with exponent e → exponent in 4n^2 is 2*e, multiplier (2*e + 1)    */
    seq_t d = (seq_t)(2 * e2 + 3);   /* factor from 2^2 * (2^e2)^2 */

    idx_t temp = nn;                 /* remaining part of n after removing 2s */
    for (idx_t p = 3; p <= temp / p; p += 2) {   /* trial division by odd primes */
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            d *= (seq_t)(2 * e + 1);
        }
    }
    /* if temp > 1, it is a remaining prime factor with exponent 1 */
    if (temp > 1) {
        d *= (seq_t)(2 * 1 + 1);     /* multiplier for prime^1  → (2*1 + 1) = 3 */
    }

    /* Number of unordered factor pairs = (d + 1) / 2 */
    return (d + 1) >> 1;
}
