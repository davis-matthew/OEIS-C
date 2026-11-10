/*
 * A285465  Positions of 0 in A285464; complement of A285466.
 * https://oeis.org/A285465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285464, the fixed point of
 *            0->00010001, 1->1111110001 (the square of the morphism 0->11,
 *            1->0001); computed by generating the word
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9648 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static uint8_t *w = NULL;
    static size_t len = 0;
    static size_t zeros = 0;

    if (n == 0) return (seq_t)0;
    size_t need = (size_t)n;

    if (w == NULL) {
        w = (uint8_t *)malloc(1);
        w[0] = 0;
        len = 1;
        zeros = 1;
    }

    while (zeros < need) {
        size_t nz = 0, no = 0;
        for (size_t i = 0; i < len; ++i) {
            if (w[i] == 0) { nz += 6; no += 2; }
            else           { nz += 3; no += 7; }
        }
        size_t nl = nz + no;
        uint8_t *nw = (uint8_t *)malloc(nl);
        size_t p = 0;
        static const uint8_t t0[8]  = {0,0,0,1,0,0,0,1};
        static const uint8_t t1[10] = {1,1,1,1,1,1,0,0,0,1};
        for (size_t i = 0; i < len; ++i) {
            if (w[i] == 0) {
                for (int k = 0; k < 8; ++k) nw[p++] = t0[k];
            } else {
                for (int k = 0; k < 10; ++k) nw[p++] = t1[k];
            }
        }
        free(w);
        w = nw;
        len = nl;
        zeros = nz;
    }

    size_t cnt = 0;
    for (size_t i = 0; i < len; ++i) {
        if (w[i] == 0) {
            if (++cnt == need) return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
