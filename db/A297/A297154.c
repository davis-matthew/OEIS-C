/*
 * A297154  Balanced ternary representation of A083254(n), 2*phi(n)-n.
 * https://oeis.org/A297154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 102 terms through n = 9866 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced ternary representation of 2*phi(n)-n */
#include <stdint.h>

/* encode the balanced ternary representation of m_abs with given sign
   (+1 for non‑negative, -1 for negative).  Result digits (1,0,2) are
   interpreted as a base‑3 integer. */
static seq_t encode_balanced(seq_t m_abs, int sign) {
    if (m_abs == 0) return 0;
    signed char digits[64];
    int cnt = 0;
    while (m_abs != 0) {
        seq_t rem = m_abs % 3;
        m_abs /= 3;
        if (rem == 2) {             /* replace digit 2 by -1 and carry 1 */
            rem = -1;
            m_abs += 1;
        }
        digits[cnt++] = (signed char)rem;
    }
    if (sign < 0) {
        for (int i = 0; i < cnt; ++i) digits[i] = (signed char)(-digits[i]);
    }
    seq_t val = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        unsigned char map = (digits[i] == 1) ? 1
                   : (digits[i] == 0) ? 0
                   : 2;                      /* digits[i] == -1 */
        val = val * 3 + map;
    }
    return val;
}

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
