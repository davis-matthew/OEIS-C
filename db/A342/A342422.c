/*
 * A342422  a(n) = Sum_{k=1..n} (n/gcd(k,n))^gcd(k,n).
 * https://oeis.org/A342422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n / gcd(k, n))^(gcd(k, n)) Equivalent: a(n)
 *            = Sum_{d|n} φ(d) * d^(n/d)
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 59, u64 n <= 119,
 *            u128 n <= 242, big any n
 * Helpers:   big_pow, calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utility functions */
#include "../utils/calc_phi.h"   /* seq_t calc_phi(seq_t m); */
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            seq_t d1_t = (seq_t)d1;
            seq_t term1 = calc_phi(d1_t) * big_pow(d1_t, (seq_t)(n / d1));
            sum += term1;

            /* divisor d2 = n / i, if distinct from d1 */
            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t d2_t = (seq_t)d2;
                seq_t term2 = calc_phi(d2_t) * big_pow(d2_t, (seq_t)(n / d2));
                sum += term2;
            }
        }
    }
    return sum;
}
