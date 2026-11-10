/*
 * A284937  Positions of 1 in A284935; complement of A284936.
 * https://oeis.org/A284937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A284935, the fixed point of
 *            0->01, 1->1100 (found by generating the word and scanning)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9971 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t need = (size_t)n;

    size_t len = 1;
    uint8_t *buf = (uint8_t *)malloc(1);
    if (!buf) return 0;
    buf[0] = 0;

    for (;;) {
        /* count ones; stop when enough */
        size_t cnt = 0;
        for (size_t i = 0; i < len; ++i) {
            if (buf[i]) {
                if (++cnt == need) {
                    free(buf);
                    return (seq_t)(i + 1);
                }
            }
        }
        /* expand */
        uint8_t *nb = (uint8_t *)malloc(len * 4 + 4);
        if (!nb) { free(buf); return 0; }
        size_t m = 0;
        for (size_t i = 0; i < len; ++i) {
            if (buf[i] == 0) {
                nb[m++] = 0; nb[m++] = 1;
            } else {
                nb[m++] = 1; nb[m++] = 1; nb[m++] = 0; nb[m++] = 0;
            }
        }
        free(buf);
        buf = nb;
        len = m;
    }
}
