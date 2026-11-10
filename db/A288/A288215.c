/*
 * A288215  Positions of 1 in A288213; complement of A288215.
 * https://oeis.org/A288215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A288213 (fixed point of
 *            00->0010, 1->011 from 00); computed by generating the word
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8520 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static unsigned char *A288215_cur = NULL;
static size_t A288215_len = 0;
static size_t A288215_ones = 0;

static void A288215_expand(void)
{
    size_t i, nl = 0, j = 0, ones = 0;
    unsigned char *nx;
    for (i = 0; i < A288215_len; ++i) {
        if (A288215_cur[i] == 0) {
            if (i + 1 < A288215_len && A288215_cur[i + 1] == 0) { nl += 4; ++i; }
            else nl += 1;
        } else nl += 3;
    }
    nx = (unsigned char *)malloc(nl);
    for (i = 0; i < A288215_len; ++i) {
        if (A288215_cur[i] == 0) {
            if (i + 1 < A288215_len && A288215_cur[i + 1] == 0) {
                nx[j++] = 0; nx[j++] = 0; nx[j++] = 1; nx[j++] = 0;
                ++i;
            } else nx[j++] = 0;
        } else {
            nx[j++] = 0; nx[j++] = 1; nx[j++] = 1;
        }
    }
    for (i = 0; i < nl; ++i) ones += nx[i];
    free(A288215_cur);
    A288215_cur = nx;
    A288215_len = nl;
    A288215_ones = ones;
}

seq_t a(idx_t n)
{
    size_t i, cnt = 0;
    if (A288215_cur == NULL) {
        A288215_len = 2;
        A288215_cur = (unsigned char *)malloc(2);
        A288215_cur[0] = 0;
        A288215_cur[1] = 0;
        A288215_ones = 0;
    }
    if (n == 0) return (seq_t)0;
    while ((idx_t)A288215_ones < n) A288215_expand();
    for (i = 0; i < A288215_len; ++i) {
        if (A288215_cur[i]) {
            ++cnt;
            if ((idx_t)cnt == n) return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
