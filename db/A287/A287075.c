/*
 * A287075  Positions of 2 in A287072.
 * https://oeis.org/A287075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 2 in the fixed point of 0->01,
 *            1->21, 2->0 (A287072); computed by iterating the morphism and
 *            scanning
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9845 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static uint8_t *w = NULL;
    static size_t len = 0;
    static size_t twos = 0;

    if (n == 0) return (seq_t)0;

    if (w == NULL) {
        w = (uint8_t *)malloc(1);
        w[0] = 0;
        len = 1;
        twos = 0;
    }

    while (twos < (size_t)n) {
        size_t nl = 0;
        for (size_t i = 0; i < len; ++i)
            nl += (w[i] == 2) ? 1 : 2;
        uint8_t *nw = (uint8_t *)malloc(nl);
        size_t p = 0, t = 0;
        for (size_t i = 0; i < len; ++i) {
            uint8_t c = w[i];
            if (c == 0) {
                nw[p++] = 0; nw[p++] = 1;
            } else if (c == 1) {
                nw[p++] = 2; nw[p++] = 1; t++;
            } else {
                nw[p++] = 0;
            }
        }
        free(w);
        w = nw;
        len = nl;
        twos = t;
    }

    size_t cnt = 0;
    for (size_t i = 0; i < len; ++i) {
        if (w[i] == 2) {
            if (++cnt == (size_t)n)
                return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
