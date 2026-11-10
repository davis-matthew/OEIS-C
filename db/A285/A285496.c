/*
 * A285496  Positions of 0 in A285495; complement of A285497.
 * https://oeis.org/A285496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285495, the fixed point w of
 *            sigma^2 with sigma: 0->11, 1->0010, so 0->00100010 and
 *            1->1111001011. The code builds w by self-generation and scans
 *            for the n-th 0
 * Range:     u8 n <= 138, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9580 (2026-11-08)
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
    static const unsigned char im0[8]  = {0,0,1,0,0,0,1,0};
    static const unsigned char im1[10] = {1,1,1,1,0,0,1,0,1,1};
    size_t target = (size_t)n;
    size_t cap = 64;
    unsigned char *buf = (unsigned char *)malloc(cap);
    size_t len = 0, zeros = 0, rd = 1, i;
    memcpy(buf, im0, 8);
    len = 8;
    zeros = 6;
    while (zeros < target) {
        const unsigned char *im;
        size_t l;
        if (buf[rd++] == 0) { im = im0; l = 8; } else { im = im1; l = 10; }
        if (len + l > cap) {
            cap *= 2;
            buf = (unsigned char *)realloc(buf, cap);
        }
        for (i = 0; i < l; i++) {
            buf[len++] = im[i];
            if (im[i] == 0) zeros++;
        }
    }
    size_t cnt = 0, pos = 0;
    for (i = 0; i < len; i++) {
        if (buf[i] == 0) {
            cnt++;
            if (cnt == target) { pos = i + 1; break; }
        }
    }
    free(buf);
    return (seq_t)pos;
}
