/*
 * A329361  a(n) = Sum_{i = 1..n} 2^(n - i) * A000002(i).
 * https://oeis.org/A329361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} 2^(n-i)*A000002(i); a(n+1) = A000002(n+1) +
 *            2*a(n), a(0) = 0 (computed via Kolakoski generation)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t s = 0;
    uint64_t N = (uint64_t)n, i, len, run;
    unsigned char *buf;
    if (N < 1) return s;
    buf = (unsigned char *)malloc((size_t)(N + 8));
    if (!buf) return s;
    buf[1] = 1; buf[2] = 2; buf[3] = 2;
    len = 3; run = 3;
    while (len < N) {
        unsigned char v = (unsigned char)((run & 1) ? 1 : 2);
        unsigned char L = buf[run];
        unsigned char k;
        for (k = 0; k < L; k++) buf[++len] = v;
        run++;
    }
    for (i = 1; i <= N; i++) {
        s = s * 2 + (seq_t)buf[i];
    }
    free(buf);
    return s;
}
