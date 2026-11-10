/*
 * A257694  a(0) = 1; for n >= 1, a(n) = A060130(n) * a(A257684(n)).
 * https://oeis.org/A257694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = A060130(n) * a(A257684(n));
 *            computed iteratively on the factorial-base digits of n
 * Range:     u8 n <= 3472, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 137 terms through n = 9795 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;
    while (n > 0) {
        idx_t m = n;
        idx_t count = 0;
        idx_t next = 0;
        idx_t f = 1;
        if (m % 2 != 0) ++count;
        m /= 2;
        idx_t base = 3;
        while (m > 0) {
            idx_t d = m % base;
            if (d != 0) {
                ++count;
                next += (d - 1) * f;
            }
            m /= base;
            f *= (base - 1);
            ++base;
        }
        res *= (seq_t)count;
        n = next;
    }
    return res;
}
