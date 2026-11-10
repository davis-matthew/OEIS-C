/*
 * A287243  Positions of 2 in A287240.
 * https://oeis.org/A287243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 2 in A287240 (1-limiting word of
 *            0->01, 1->20, 2->1 from 1); computed by iterating the morphism
 *            and scanning
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9014 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    unsigned char *w = (unsigned char *)malloc(1);
    if (!w) return (seq_t)0;
    w[0] = 1;
    size_t len = 1;
    size_t iter = 0;

    while (1) {
        unsigned char *nw = (unsigned char *)malloc(len * 2 + 1);
        if (!nw) { free(w); return (seq_t)0; }
        size_t nl = 0;
        for (size_t i = 0; i < len; ++i) {
            unsigned char s = w[i];
            if (s == 0) { nw[nl++] = 0; nw[nl++] = 1; }
            else if (s == 1) { nw[nl++] = 2; nw[nl++] = 0; }
            else { nw[nl++] = 1; }
        }
        free(w);
        w = nw;
        len = nl;
        ++iter;

        if (iter & 1) {
            idx_t cnt = 0;
            for (size_t i = 0; i < len; ++i) {
                if (w[i] == 2) {
                    ++cnt;
                    if (cnt == n) {
                        free(w);
                        return (seq_t)(i + 1);
                    }
                }
            }
        }
    }
}
