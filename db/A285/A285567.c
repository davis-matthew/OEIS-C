/*
 * A285567  Positions of 1 in A285565; complement of A285566.
 * https://oeis.org/A285567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A285565, the fixed point of
 *            0->01100110, 1->110110011011 (square of morphism 0->11, 1->0110)
 * Range:     u8 n <= 158, u16 n <= 6178, u32 n <= 6178, u64 n <= 6178,
 *            u128 n <= 6178, big any n
 * Validated: standard, pass, 81 terms through n = 6088 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned char *A285567_word = NULL;
static size_t A285567_len = 0;
static size_t A285567_ones = 0;

static void A285567_grow(void)
{
    static const unsigned char b0[8] = {0,1,1,0,0,1,1,0};
    static const unsigned char b1[12] = {1,1,0,1,1,0,0,1,1,0,1,1};
    if (A285567_len == 0) {
        A285567_word = (unsigned char *)malloc(1);
        A285567_word[0] = 0;
        A285567_len = 1;
        A285567_ones = 0;
        return;
    }
    size_t zeros = A285567_len - A285567_ones;
    size_t newLen = zeros * 8 + A285567_ones * 12;
    unsigned char *nw = (unsigned char *)malloc(newLen);
    size_t pos = 0, ones = 0;
    for (size_t i = 0; i < A285567_len; ++i) {
        if (A285567_word[i] == 0) {
            memcpy(nw + pos, b0, 8);
            pos += 8;
            ones += 4;
        } else {
            memcpy(nw + pos, b1, 12);
            pos += 12;
            ones += 8;
        }
    }
    free(A285567_word);
    A285567_word = nw;
    A285567_len = newLen;
    A285567_ones = ones;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A285567_len == 0) A285567_grow();
    while (A285567_ones < (size_t)n) A285567_grow();
    size_t cnt = 0;
    for (size_t i = 0; i < A285567_len; ++i) {
        if (A285567_word[i]) {
            ++cnt;
            if (cnt == (size_t)n) return (seq_t)(i + 1);
        }
    }
    return 0;
}
