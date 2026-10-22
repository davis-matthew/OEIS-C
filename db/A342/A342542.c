/*
 * A342542  a(n) = Sum_{k=1..n} phi(gcd(k, n))^(n/gcd(k, n) - 1).
 * https://oeis.org/A342542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} phi(gcd(k, n))^(n/gcd(k, n) - 1)
 * Range:     u8 n <= 20, u16 n <= 39, u32 n <= 79, u64 n <= 154,
 *            u128 n <= 314, big any n
 * Helpers:   phi, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n is always >=1 for this sequence */

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;

            /* term for divisor d: phi(d2) * phi(d)^(d2-1) */
            idx_t exp1 = d2 - 1;
            seq_t phi_d  = phi(d);
            seq_t phi_d2 = phi(d2);
            seq_t pow1   = pow_int(phi_d, exp1);
            sum += phi_d2 * pow1;

            if (d != d2) {
                /* term for divisor d2: phi(d) * phi(d2)^(d-1) */
                idx_t exp2 = d - 1;
                seq_t pow2 = pow_int(phi_d2, exp2);
                sum += phi_d * pow2;
            }
        }
    }

    return sum;
}
