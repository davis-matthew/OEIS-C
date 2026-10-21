/*
 * A297154  Balanced ternary representation of A083254(n), 2*phi(n)-n.
 * https://oeis.org/A297154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 9866, u32 n <= 9866, u64 n <= 9866,
 *            u128 n <= 9866, big any n
 * Helpers:   encode_balanced
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced ternary representation of 2*phi(n)-n */
#include <stdint.h>
#include "../utils/encode_balanced.h"

/* compute Euler's totient phi(n) for n >= 1 */
static seq_t phi_calc(idx_t n) {
    if (n <= 1) return (seq_t)n;            /* phi(1) = 1, phi(0) = 0 */
    seq_t result = (seq_t)n;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            result = result / p * (p - 1);
        }
    }
    if (t > 1) result = result / t * (t - 1);
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* Safety for unused offset */
    seq_t phi_n = phi_calc(n);
    long long B = 2LL * (long long)phi_n - (long long)n;
    if (B == 0) return 0;
    int sign = (B > 0) ? 1 : -1;
    seq_t m_abs = B > 0 ? (seq_t)B : (seq_t)(-B);
    return encode_balanced(m_abs, sign);
}
