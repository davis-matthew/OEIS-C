/*
 * A346558  a(n) = Sum_{d|n} phi(n/d) * (2^d - 1).
 * https://oeis.org/A346558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * (2^d - 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Euler's totient for an integer m (1 ≤ m ≤ n).  Returns a seq_t. */
static seq_t phi_uint(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t temp = m;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / (seq_t)p;
        }
    }
    if (temp > 1) result -= result / (seq_t)temp;
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* divisor d1 */
            seq_t phi1 = phi_uint(d2);               /* phi(n/d1) = phi(d2) */
            seq_t term1 = phi1 * (pow2(d1) - 1);
            sum += term1;

            /* divisor d2 (if different) */
            if (d1 != d2) {
                seq_t phi2 = phi_uint(d1);               /* phi(n/d2) = phi(d1) */
                seq_t term2 = phi2 * (pow2(d2) - 1);
                sum += term2;
            }
        }
    }
    return sum;
}
