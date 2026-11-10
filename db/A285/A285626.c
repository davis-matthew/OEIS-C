/*
 * A285626  Positions of 0 in A285625; complement of A285627.
 * https://oeis.org/A285626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in the fixed point of 0->11,
 *            1->1010 starting from 1 (A285625)
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9514 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A285626_w = NULL;
static uint64_t A285626_len = 0;

static void A285626_expand(void)
{
    uint64_t nl = 0, i, p = 0;
    unsigned char *nw;
    if (!A285626_w) {
        A285626_w = (unsigned char *)malloc(1);
        if (!A285626_w) abort();
        A285626_w[0] = 1;
        A285626_len = 1;
        return;
    }
    for (i = 0; i < A285626_len; i++) nl += A285626_w[i] ? 4 : 2;
    nw = (unsigned char *)malloc((size_t)nl);
    if (!nw) abort();
    for (i = 0; i < A285626_len; i++) {
        if (A285626_w[i]) {
            nw[p++] = 1; nw[p++] = 0; nw[p++] = 1; nw[p++] = 0;
        } else {
            nw[p++] = 1; nw[p++] = 1;
        }
    }
    free(A285626_w);
    A285626_w = nw;
    A285626_len = nl;
}

seq_t a(idx_t n)
{
    if (!A285626_w) A285626_expand();
    for (;;) {
        uint64_t i, cnt = 0;
        for (i = 0; i < A285626_len; i++) {
            if (!A285626_w[i]) {
                cnt++;
                if (cnt == (uint64_t)n) return (seq_t)(i + 1);
            }
        }
        A285626_expand();
    }
}
