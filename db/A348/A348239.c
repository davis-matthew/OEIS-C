/*
 * A348239  Partial sums of A348237.
 * https://oeis.org/A348239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} b(k), where b = A348237: b(0)=1, b(1)=3,
 *            b(2)=6, b(3)=9, b(4)=10, b(5)=13, b(6)=18, b(7)=21, b(8)=22, and
 *            for n>=9: b(n) = b(n-1) - b(n-2) + b(n-3) + b(n-5) - b(n-6) +
 *            b(n-7) - b(n-8)
 * Range:     u8 n <= 12, u16 n <= 215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9046 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* w[0..7] holds b(i-8) .. b(i-1) while computing b(i) */
    static const unsigned init[9] = {1, 3, 6, 9, 10, 13, 18, 21, 22};
    seq_t w[8];
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t v;
        if (i < 9) {
            v = (seq_t)init[i];
        } else {
            v = w[7] + w[5] + w[3] + w[1];
            v -= w[6];
            v -= w[2];
            v -= w[0];
        }
        sum += v;
        if (i >= 9) {
            for (int j = 0; j < 7; ++j) w[j] = w[j + 1];
            w[7] = v;
        } else if (i == 8) {
            /* window now b(1)..b(8) after absorbing b(8) */
            for (int j = 0; j < 8; ++j) w[j] = (seq_t)init[j + 1];
        }
    }
    return sum;
}
