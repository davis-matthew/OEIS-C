/*
 * A285564  Positions of 1 in A286939; complement of A285432.
 * https://oeis.org/A285564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A286939 (fixed point of
 *            00->001, 1->000, starting from 00); computed by generating the
 *            word and scanning
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9778 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static unsigned char *A285564_w = NULL;
static size_t A285564_len = 0;

static void A285564_step(void) {
    size_t cap = A285564_len * 3 + 10;
    unsigned char *nb = (unsigned char *)malloc(cap);
    size_t nl = 0, i;
    for (i = 0; i < A285564_len; ++i) {
        if (A285564_w[i] == 0 && i + 1 < A285564_len && A285564_w[i + 1] == 0) {
            nb[nl++] = 0; nb[nl++] = 0; nb[nl++] = 1;
            ++i;
        } else if (A285564_w[i] == 1) {
            nb[nl++] = 0; nb[nl++] = 0; nb[nl++] = 0;
        } else {
            nb[nl++] = 0;
        }
    }
    free(A285564_w);
    A285564_w = nb;
    A285564_len = nl;
}

static size_t A285564_ones(void) {
    size_t c = 0, i;
    for (i = 0; i < A285564_len; ++i) c += A285564_w[i];
    return c;
}

seq_t a(idx_t n) {
    size_t i, c = 0;
    if (n == 0) return (seq_t)0;
    if (A285564_w == NULL) {
        A285564_w = (unsigned char *)malloc(4);
        A285564_w[0] = 0;
        A285564_w[1] = 0;
        A285564_len = 2;
    }
    while (A285564_ones() < (size_t)n) A285564_step();
    for (i = 0; i < A285564_len; ++i) {
        if (A285564_w[i]) {
            ++c;
            if (c == (size_t)n) return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
