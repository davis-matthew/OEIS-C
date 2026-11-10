/*
 * A284681  Positions of 0 in A284680; complement of A284682.
 * https://oeis.org/A284681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A284680, the fixed point of the
 *            morphism 0->1, 1->1000 starting from 1 (computed by expanding
 *            the word)
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 8521 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A284681_w = NULL;
static size_t A284681_len = 0;

static void A284681_expand(void)
{
    size_t nl = 0, i, p = 0;
    for (i = 0; i < A284681_len; ++i) nl += A284681_w[i] ? 4 : 1;
    unsigned char *nw = (unsigned char *)malloc(nl);
    for (i = 0; i < A284681_len; ++i) {
        if (A284681_w[i]) {
            nw[p++] = 1; nw[p++] = 0; nw[p++] = 0; nw[p++] = 0;
        } else {
            nw[p++] = 1;
        }
    }
    free(A284681_w);
    A284681_w = nw;
    A284681_len = nl;
}

seq_t a(idx_t n)
{
    if (!A284681_w) {
        A284681_w = (unsigned char *)malloc(1);
        A284681_w[0] = 1;
        A284681_len = 1;
    }
    for (;;) {
        idx_t cnt = 0;
        for (size_t i = 0; i < A284681_len; ++i) {
            if (!A284681_w[i]) {
                if (++cnt == n) return (seq_t)(i + 1);
            }
        }
        A284681_expand();
    }
}
