/*
 * A287183  Positions of 0 in A288106; complement of A287184.
 * https://oeis.org/A287183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A288106 (fixed point of
 *            00->0010, 1->00 starting with 00); computed by generating the
 *            word
 * Range:     u8 n <= 208, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9316 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A287183_w = NULL;
static size_t A287183_len = 0;

static void A287183_expand(void)
{
    if (A287183_len == 0) {
        A287183_w = (unsigned char *)malloc(2);
        if (!A287183_w) abort();
        A287183_w[0] = 0;
        A287183_w[1] = 0;
        A287183_len = 2;
        return;
    }
    size_t cap = A287183_len * 2 + 10;
    unsigned char *nb = (unsigned char *)malloc(cap);
    if (!nb) abort();
    size_t nl = 0, i = 0;
    while (i < A287183_len) {
        if (A287183_w[i] == 0) {
            if (i + 1 < A287183_len && A287183_w[i + 1] == 0) {
                nb[nl++] = 0; nb[nl++] = 0; nb[nl++] = 1; nb[nl++] = 0;
                i += 2;
            } else {
                nb[nl++] = 0;
                i += 1;
            }
        } else {
            nb[nl++] = 0; nb[nl++] = 0;
            i += 1;
        }
    }
    free(A287183_w);
    A287183_w = nb;
    A287183_len = nl;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    for (;;) {
        if (A287183_len == 0) A287183_expand();
        size_t cnt = 0;
        for (size_t i = 0; i < A287183_len; i++) {
            if (A287183_w[i] == 0) {
                cnt++;
                if ((idx_t)cnt == n) return (seq_t)(i + 1);
            }
        }
        A287183_expand();
    }
}
