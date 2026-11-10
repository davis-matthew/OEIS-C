/*
 * A284365  Positions of 0 in A284364; complement of A284366.
 * https://oeis.org/A284365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A284364, the fixed point of
 *            0->1, 1->101010 (computed by expanding the morphism and scanning
 *            for zeros)
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9736 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static unsigned char *word = NULL;
    static size_t wlen = 0;
    static uint64_t *zpos = NULL;
    static size_t zcnt = 0;

    if (n == 0) return (seq_t)0;

    if (wlen == 0) {
        word = (unsigned char *)malloc(1);
        if (!word) return (seq_t)0;
        word[0] = 1;
        wlen = 1;
        zcnt = 0;
    }

    while ((idx_t)zcnt < n) {
        size_t i, p = 0, ones = 0, zeros = 0;
        for (i = 0; i < wlen; ++i) {
            if (word[i]) ++ones; else ++zeros;
        }
        size_t nl = zeros + ones * 6;
        unsigned char *nw = (unsigned char *)malloc(nl);
        if (!nw) return (seq_t)0;
        for (i = 0; i < wlen; ++i) {
            if (word[i] == 0) {
                nw[p++] = 1;
            } else {
                nw[p++] = 1; nw[p++] = 0;
                nw[p++] = 1; nw[p++] = 0;
                nw[p++] = 1; nw[p++] = 0;
            }
        }
        free(word);
        word = nw;
        wlen = nl;

        size_t zc = 0;
        for (i = 0; i < wlen; ++i) if (word[i] == 0) ++zc;
        uint64_t *nz = (uint64_t *)malloc((zc ? zc : 1) * sizeof(uint64_t));
        if (!nz) return (seq_t)0;
        size_t k = 0;
        for (i = 0; i < wlen; ++i) if (word[i] == 0) nz[k++] = (uint64_t)(i + 1);
        free(zpos);
        zpos = nz;
        zcnt = zc;
    }

    return (seq_t)zpos[n - 1];
}
