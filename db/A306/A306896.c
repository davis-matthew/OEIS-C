/*
 * A306896  a(n) = Sum_{d|n} (2^d + 2*(-1)^d)*phi(n/d).
 * https://oeis.org/A306896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A306896: a(n) = Sum_{d|n} (2^d + 2*(-1)^d) * phi(n/d)   */
/*   phi(m) is Euler's totient function.                           */

#include <stdint.h>
#include "../utils/pow2.h"

static seq_t my_phi(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t res = n;
    uint64_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            res -= res / p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) res -= res / tmp;
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1; guard for safety */

    seq_t sum = (seq_t)0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* divisor d1 */
            seq_t phi_n_over_d1 = my_phi(d2); /* phi(n/d1) = phi(d2) */
            seq_t term1 = (d1 & 1ULL) ? (pow2(d1) - (seq_t)2)
                                       : (pow2(d1) + (seq_t)2);
            sum += term1 * phi_n_over_d1;

            if (d1 != d2) {
                /* divisor d2 */
                seq_t phi_n_over_d2 = my_phi(d1); /* phi(n/d2) = phi(d1) */
                seq_t term2 = (d2 & 1ULL) ? (pow2(d2) - (seq_t)2)
                                           : (pow2(d2) + (seq_t)2);
                sum += term2 * phi_n_over_d2;
            }
        }
    }
    return sum;
}
