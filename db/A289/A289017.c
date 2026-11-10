/*
 * A289017  Positions of 0 in A289016; complement of A289018.
 * https://oeis.org/A289017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A289016, the fixed point of
 *            00->0010, 01->010, 10->000 starting from 00; computed by
 *            generating the word
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9669 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A289017_gen(idx_t need, idx_t *out_len)
{
    idx_t src_len = 2;
    unsigned char *src = (unsigned char *)malloc(2);
    if (!src) return NULL;
    src[0] = 0; src[1] = 0;
    while (src_len < need) {
        idx_t cap = src_len * 2 + 8;
        unsigned char *dest = (unsigned char *)malloc((size_t)cap);
        if (!dest) { free(src); return NULL; }
        idx_t dl = 0;
        for (idx_t i = 0; i + 1 < src_len; i += 2) {
            unsigned char a0 = src[i], a1 = src[i + 1];
            if (a0 == 0 && a1 == 0) {
                dest[dl++] = 0; dest[dl++] = 0; dest[dl++] = 1; dest[dl++] = 0;
            } else if (a0 == 0 && a1 == 1) {
                dest[dl++] = 0; dest[dl++] = 1; dest[dl++] = 0;
            } else if (a0 == 1 && a1 == 0) {
                dest[dl++] = 0; dest[dl++] = 0; dest[dl++] = 0;
            }
        }
        free(src);
        src = dest;
        src_len = dl;
    }
    *out_len = src_len;
    return src;
}

seq_t a(idx_t n)
{
    idx_t need = 2 * n + 16;
    for (;;) {
        idx_t len = 0;
        unsigned char *bits = A289017_gen(need, &len);
        if (!bits) return (seq_t)0;
        idx_t cnt = 0;
        for (idx_t i = 0; i < len; i++) {
            if (bits[i] == 0) {
                cnt++;
                if (cnt == n) {
                    free(bits);
                    return (seq_t)(i + 1);
                }
            }
        }
        free(bits);
        need *= 2;
    }
}
