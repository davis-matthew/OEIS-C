/*
 * A322327  a(n) = A005361(n) * A034444(n).
 * https://oeis.org/A322327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} 2*e (a multiplicative function with a(p^e)=2*e)
 * Range:     u8 n <= 9155, u16 n <= 9155, u32 n <= 9155, u64 n <= 9155,
 *            u128 n <= 9155, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined offset, defensive return */

    uint64_t m = (uint64_t)n;  /* work on uint64_t to avoid overflow in divisions */
    seq_t   result = 1;

    /* trial division; safe against integer overflow using m/p comparison */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t factor = (seq_t)(2 * (seq_t)exp);
            result *= factor;
        }
    }

    /* if a prime factor larger than sqrt(n) remains */
    if (m > 1) {
        result *= (seq_t)2;
    }

    return result;
}
