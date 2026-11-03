/*
 * A342423  a(n) = Sum_{k=1..n} gcd(k,n)^gcd(k,n).
 * https://oeis.org/A342423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * d^d
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 367 (2026-11-01)
 * Helpers:   ipow, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helper utilities */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 for safety */

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;

            /* d is a divisor, e = n/d is the complementary divisor */
            seq_t d_pow = ipow((seq_t)d, (seq_t)d);      /* d^d */
            seq_t phi_e  = totient(e);                        /* phi(n/d) */
            sum = sum + phi_e * d_pow;

            if (d != e) {
                seq_t e_pow = ipow((seq_t)e, (seq_t)e);   /* e^e */
                seq_t phi_d  = totient(d);                       /* phi(d) */
                sum = sum + phi_d * e_pow;
            }
        }
    }

    return sum;
}
