/*
 * A276326  Numbers expressed in greedy A001563-base.
 * https://oeis.org/A276326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   write n greedily in base weights A001563(i)=i*i! (i=1,2,...),
 *            digit d_i = floor(rem/w_i); a(n) = Sum d_i*10^(i-1)
 * Range:     u8 n <= 53, u16 n <= 4175, u32 n <= 4320, u64 n <= 4320,
 *            u128 n <= 4320, big any n
 * Validated: standard, pass, 89 terms through n = 4003 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t w[64];
    int k = 0;
    seq_t f = 1; /* i! */
    for (int i = 1; i < 63; i++) {
        f = f * (seq_t)i;
        /* need i*f <= m */
        if (f > m / (seq_t)i) break;
        w[++k] = f * (seq_t)i;
        if (f > m / (seq_t)(i + 1)) break;
    }
    seq_t res = 0;
    for (int i = k; i >= 1; i--) {
        seq_t d = m / w[i];
        m = m % w[i];
        res = res * 10 + d;
    }
    return res;
}
