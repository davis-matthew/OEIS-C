/*
 * A393082  Fourth column of A392806.
 * https://oeis.org/A393082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(7^(e+1)*n + 7^e*k) = a(7^(e+1)*n) + k*max(0,
 *            2*7^e*Sum_{i=1..n} A373217(i) - A392138(e)), e >= 0, 0 < k < 7,
 *            a(0) = 0; A373217(i) = v_7(i)+1, A392138(e) = 2*e*7^e -
 *            (7^e-1)/6. Applied once per nonzero base-7 digit k of n (e = its
 *            position, n = the digits above it)
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9536 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t m = n;
    seq_t pw = 1;   /* 7^e */
    seq_t e = 0;
    while (m > 0) {
        idx_t k = m % 7;
        m /= 7;
        if (k != 0) {
            /* S(m) = Sum_{i=1..m} (v_7(i)+1) = m + v_7(m!) */
            seq_t S = (seq_t)m;
            idx_t q = m / 7;
            while (q > 0) {
                S += (seq_t)q;
                q /= 7;
            }
            seq_t B = (seq_t)2 * pw * S;
            seq_t A = (seq_t)2 * e * pw - (pw - (seq_t)1) / (seq_t)6;
            if (B > A) res += (seq_t)k * (B - A);
        }
        pw *= 7;
        e++;
    }
    return res;
}
