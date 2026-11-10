/*
 * A284589  Positions of 0 in A284588; complement of A284590.
 * https://oeis.org/A284589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A284588, the 1-limiting word of
 *            0->1, 1->0101 (computed by expanding the word)
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9535 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static uint8_t *A284589_step(const uint8_t *w, size_t len, size_t *outlen)
{
    size_t ones = 0, i;
    for (i = 0; i < len; ++i) ones += w[i];
    size_t nl = (len - ones) + 4 * ones;
    uint8_t *r = (uint8_t *)malloc(nl ? nl : 1);
    size_t k = 0;
    for (i = 0; i < len; ++i) {
        if (w[i] == 0) {
            r[k++] = 1;
        } else {
            r[k++] = 0; r[k++] = 1; r[k++] = 0; r[k++] = 1;
        }
    }
    *outlen = nl;
    return r;
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n;
    size_t len = 1;
    uint8_t *w = (uint8_t *)malloc(1);
    w[0] = 1;
    for (;;) {
        size_t i, zeros = 0;
        for (i = 0; i < len; ++i) if (w[i] == 0) ++zeros;
        if (zeros >= need) {
            size_t c = 0;
            for (i = 0; i < len; ++i) {
                if (w[i] == 0) {
                    ++c;
                    if (c == need) {
                        free(w);
                        return (seq_t)(i + 1);
                    }
                }
            }
        }
        /* two steps keep us at an even depth (prefix-stable) */
        size_t l1, l2;
        uint8_t *t = A284589_step(w, len, &l1);
        free(w);
        w = A284589_step(t, l1, &l2);
        free(t);
        len = l2;
    }
}
