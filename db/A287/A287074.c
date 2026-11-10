/*
 * A287074  Positions of 1 in A287072.
 * https://oeis.org/A287074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A287072, the fixed point of
 *            0->01, 1->21, 2->0 (built by iterating the morphism)
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9679 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *w_word = NULL;
static size_t w_len = 0;
static size_t *w_pos = NULL;
static size_t w_cnt = 0;

static void w_rebuild_positions(void)
{
    free(w_pos);
    w_pos = (size_t *)malloc((w_len + 1) * sizeof(size_t));
    w_cnt = 0;
    for (size_t i = 0; i < w_len; ++i)
        if (w_word[i] == 1)
            w_pos[w_cnt++] = i + 1;
}

static void w_grow(void)
{
    size_t nl = 0;
    for (size_t i = 0; i < w_len; ++i)
        nl += (w_word[i] == 2) ? 1 : 2;
    uint8_t *nw = (uint8_t *)malloc(nl);
    size_t p = 0;
    for (size_t i = 0; i < w_len; ++i) {
        uint8_t c = w_word[i];
        if (c == 0) { nw[p++] = 0; nw[p++] = 1; }
        else if (c == 1) { nw[p++] = 2; nw[p++] = 1; }
        else { nw[p++] = 0; }
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
        w_word[0] = 0;
        w_len = 1;
        w_rebuild_positions();
    }
    while ((idx_t)w_cnt < n) {
        w_grow();
        w_rebuild_positions();
    }
    return (seq_t)w_pos[n - 1];
}
