/*
 * A285163  Positions of 0 in A285162; complement of A285164.
 * https://oeis.org/A285163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285162, the fixed point of
 *            0->001110, 1->101000110011 started from 1 (generated directly,
 *            then scanned)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8796 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static uint8_t *w_word = NULL;
static size_t w_len = 0;
static size_t *w_zpos = NULL;   /* 1-based positions of zeros */
static size_t w_zcnt = 0;

static void w_expand(void)
{
    static const uint8_t g0[6]  = {0,0,1,1,1,0};
    static const uint8_t g1[12] = {1,0,1,0,0,0,1,1,0,0,1,1};
    size_t nl = 0, i, pos = 0;
    uint8_t *nw;
    for (i = 0; i < w_len; ++i) nl += (w_word[i] == 0) ? 6 : 12;
    nw = (uint8_t *)malloc(nl);
    for (i = 0; i < w_len; ++i) {
        int k;
        if (w_word[i] == 0) { for (k = 0; k < 6; ++k) nw[pos++] = g0[k]; }
        else { for (k = 0; k < 12; ++k) nw[pos++] = g1[k]; }
    }
    free(w_word);
    w_word = nw;
    w_len = nl;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (w_word == NULL) {
        w_word = (uint8_t *)malloc(1);
        w_word[0] = 1;
        w_len = 1;
    }
    if ((size_t)n > w_zcnt) {
        size_t i, c = 0;
        while (1) {
            c = 0;
            for (i = 0; i < w_len; ++i) if (w_word[i] == 0) ++c;
            if (c >= (size_t)n) break;
            w_expand();
        }
        free(w_zpos);
        w_zpos = (size_t *)malloc(c * sizeof(size_t));
        c = 0;
        for (i = 0; i < w_len; ++i) if (w_word[i] == 0) w_zpos[c++] = i + 1;
        w_zcnt = c;
    }
    return (seq_t)w_zpos[(size_t)n - 1];
}
