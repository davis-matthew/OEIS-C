/*
 * A267647  a(n) = g_n(4), where g is the weak Goodstein function defined in
 *          A266202.
 * https://oeis.org/A267647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, full, fini, easy
 * Formula:   a(n) = g_n(4), where g_n is the weak Goodstein function. The
 *            weak Goodstein iteration starts with m=4 in base 2. At each step
 *            the current number is written in base k, the same digit string
 *            is interpreted in base k+1, one is subtracted, and k is
 *            increased. Recurrence: a(0) = 4 a(n) = reinterpret(a...
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)4;

    seq_t x = (seq_t)4;
    idx_t base = 2;

    for (idx_t i = 1; i <= n; ++i) {
        /* extract digits of x in current base */
        int digits[64];
        int len = 0;
        seq_t tmp = x;
        do {
            digits[len++] = (int)(tmp % (seq_t)base);
            tmp /= (seq_t)base;
        } while (tmp > 0);

        /* interpret same digits in base (base + 1) */
        seq_t y = 0;
        seq_t pow = 1;
        for (int k = 0; k < len; ++k) {
            y += (seq_t)digits[k] * pow;
            pow *= (seq_t)(base + 1);
        }

        /* subtract 1 and prepare for next step */
        x = y - 1;
        ++base;
    }
    return x;
}
