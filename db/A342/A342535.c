/*
 * A342535  a(n) = Sum_{k=1..n} phi(gcd(k, n))^3.
 * https://oeis.org/A342535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{k=1..n} phi(gcd(k,n))^3
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9567 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* d is a divisor */
            seq_t phi_d  = totient(d);          /* phi(d) */
            seq_t phi_nd = totient(n / d);      /* phi(n/d) */
            seq_t phi_d_cubed = phi_d * phi_d * phi_d;
            result += phi_nd * phi_d_cubed;

            /* n/d is the complementary divisor, if distinct */
            if (d != n / d) {
                seq_t phi_d2  = totient(n / d); /* phi(n/d) = phi(complement) */
                seq_t phi_d2_cubed = phi_d2 * phi_d2 * phi_d2;
                result += phi_d * phi_d2_cubed;   /* phi(n/d2) = phi(d) */
            }
        }
    }
    return result;
}
