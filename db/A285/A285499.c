/*
 * A285499  Positions of 0 in A285498; complement of A285500.
 * https://oeis.org/A285499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285498, the 1-limiting word of
 *            0->11, 1->0010 (built by iterating the morphism twice from "11")
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9904 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A285499_word = NULL;
static size_t A285499_len = 0;
static size_t *A285499_pos = NULL;
static size_t A285499_cnt = 0;

static void A285499_step(void)
{
    size_t len = 0, i, p = 0;
    unsigned char *buf;
    for (i = 0; i < A285499_len; ++i)
        len += (A285499_word[i] == 0) ? 2 : 4;
    buf = (unsigned char *)malloc(len);
    for (i = 0; i < A285499_len; ++i) {
        if (A285499_word[i] == 0) {
            buf[p++] = 1; buf[p++] = 1;
        } else {
            buf[p++] = 0; buf[p++] = 0; buf[p++] = 1; buf[p++] = 0;
        }
    }
    free(A285499_word);
    A285499_word = buf;
    A285499_len = len;
}

static void A285499_ensure(size_t n)
{
    if (A285499_cnt >= n) return;
    if (!A285499_word) {
        A285499_word = (unsigned char *)malloc(2);
        A285499_word[0] = 1;
        A285499_word[1] = 1;
        A285499_len = 2;
    }
    while (A285499_cnt < n) {
        size_t i, c = 0;
        A285499_step();
        A285499_step();
        for (i = 0; i < A285499_len; ++i)
            if (A285499_word[i] == 0) ++c;
        free(A285499_pos);
        A285499_pos = (size_t *)malloc((c ? c : 1) * sizeof(size_t));
        c = 0;
        for (i = 0; i < A285499_len; ++i)
            if (A285499_word[i] == 0) A285499_pos[c++] = i + 1;
        A285499_cnt = c;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    A285499_ensure((size_t)n);
    return (seq_t)A285499_pos[(size_t)n - 1];
}
