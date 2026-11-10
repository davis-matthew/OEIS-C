/*
 * A289018  Positions of 1 in A289016; complement of A289017.
 * https://oeis.org/A289018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A289016 (fixed point of
 *            00->0010, 01->010, 10->000 from 00); computed by expanding the
 *            word until it holds n ones
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t src_len = 2;
    unsigned char *src = (unsigned char *)malloc(src_len);
    if (!src) return (seq_t)0;
    src[0] = 0; src[1] = 0;

    for (;;) {
        idx_t cnt = 0;
        for (idx_t i = 0; i < src_len; i++) {
            if (src[i]) {
                cnt++;
                if (cnt == n) {
                    free(src);
                    return (seq_t)(i + 1);
                }
            }
        }
        idx_t dest_cap = src_len * 2 + 8;
        unsigned char *dest = (unsigned char *)malloc(dest_cap);
        if (!dest) { free(src); return (seq_t)0; }
        idx_t dl = 0;
        for (idx_t i = 0; i + 1 < src_len; i += 2) {
            unsigned char p = src[i], q = src[i + 1];
            if (p == 0 && q == 0) {
                dest[dl++] = 0; dest[dl++] = 0; dest[dl++] = 1; dest[dl++] = 0;
            } else if (p == 0 && q == 1) {
                dest[dl++] = 0; dest[dl++] = 1; dest[dl++] = 0;
            } else if (p == 1 && q == 0) {
                dest[dl++] = 0; dest[dl++] = 0; dest[dl++] = 0;
            }
        }
        free(src);
        src = dest;
        src_len = dl;
    }
}
