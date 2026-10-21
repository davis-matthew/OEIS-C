/*
 * A256691  From fourth root of Riemann zeta function: form Dirichlet series
 *          Sum b(n)/n^x whose fourth power is zeta function; sequence gives
 *          denominator of b(n).
 * https://oeis.org/A256691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 4095, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A256691: denominator of the coefficient of 1/n^x in the
   Dirichlet series expansion of ζ(x)^(1/4).

   For each prime power p^k || n we have
      b(p^k) = (∏_{i=0}^{k-1} (4i+1)) / (4^k · k!)
   Since the numerator is odd, the denominator is a power of two:
      denom(b(p^k)) = 2^{2k + v2(k!)}.
   Hence
      a(n) = 2^{∑_{p^k||n} (2k + v2(k!))}.
   This function implements that formula.
*/

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    idx_t temp = n;
    idx_t totalExp = 0;

    /* factorization by trial division */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t k = 0;
            while (temp % p == 0) {
                temp /= p;
                ++k;
            }
            /* exponent contributed by prime power p^k */
            idx_t v2k = 0;                    /* v2(k!) */
            for (idx_t m = 2; m <= k; m <<= 1)
                v2k += k / m;
            totalExp += 2 * k + v2k;
        }
    }

    /* remaining prime > sqrt(temp) */
    if (temp > 1) {
        /* exponent for a prime with k=1 : 2*1 + v2(1!) = 2 */
        totalExp += 2;
    }

    /* compute 2^totalExp as seq_t */
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < totalExp; ++i)
        res *= (seq_t)2;

    return res;
}
