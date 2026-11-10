/*
 * A348946  a(n) = gcd(sigma(n), A348944(n)), where A348944 is the arithmetic
 *          mean of A003959 and A034448, and sigma is the sum of divisors
 *          function.
 * https://oeis.org/A348946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), (A003959(n) + A034448(n))/2), with A003959
 *            = Product (p+1)^e and A034448 = Product (p^e+1) over p^e || n
 * Range:     u8 n <= 71, u16 n <= 19999, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 98 terms through n = 9459 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sig = 1, f = 1, g = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
                f *= (seq_t)(p + 1);
            }
            sig *= term;
            g *= (pw + 1);
        }
    }
    if (m > 1) {
        sig *= ((seq_t)m + 1);
        f *= ((seq_t)m + 1);
        g *= ((seq_t)m + 1);
    }
    seq_t mean = (f + g) / 2;
    return gcd(sig, mean);
}
