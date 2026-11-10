/*
 * A289027  Positions of 1 in A289025; complement of A289026.
 * https://oeis.org/A289027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A289025, the fixed point of
 *            00->0010, 01->010, 10->001 starting from 00. The code builds
 *            iterates and reads only the prefix shared by two consecutive
 *            iterates, which can no longer change
 * Range:     u8 n <= 74, u16 n <= 10343, u32 n <= 10343, u64 n <= 10343,
 *            u128 n <= 10343, big any n
 * Validated: standard, pass, 74 terms through n = 8594 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static unsigned char *A289027_cur = NULL;
static size_t A289027_cur_len = 0;
static unsigned char *A289027_prev = NULL;
static size_t A289027_prev_len = 0;

static void A289027_step(void)
{
    unsigned char *next = (unsigned char *)malloc(A289027_cur_len * 2 + 4);
    size_t m = 0, i;
    for (i = 0; i + 1 < A289027_cur_len; i += 2) {
        unsigned char x = A289027_cur[i], y = A289027_cur[i + 1];
        if (x == 0 && y == 0) {
            next[m++] = 0; next[m++] = 0; next[m++] = 1; next[m++] = 0;
        } else if (x == 0 && y == 1) {
            next[m++] = 0; next[m++] = 1; next[m++] = 0;
        } else if (x == 1 && y == 0) {
            next[m++] = 0; next[m++] = 0; next[m++] = 1;
        } else {
            next[m++] = x; next[m++] = y;
        }
    }
    if (A289027_cur_len & 1)
        next[m++] = A289027_cur[A289027_cur_len - 1];
    if (A289027_prev) free(A289027_prev);
    A289027_prev = A289027_cur;
    A289027_prev_len = A289027_cur_len;
    A289027_cur = next;
    A289027_cur_len = m;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (A289027_cur == NULL) {
        A289027_cur = (unsigned char *)malloc(2);
        A289027_cur[0] = 0;
        A289027_cur[1] = 0;
        A289027_cur_len = 2;
        A289027_step();
    }
    for (;;) {
        /* Common prefix of prev and cur, rounded down to even, is permanent:
           its image is a longer common prefix of cur and next. */
        size_t lim = A289027_prev_len < A289027_cur_len ? A289027_prev_len : A289027_cur_len;
        size_t s = 0;
        while (s < lim && A289027_prev[s] == A289027_cur[s])
            ++s;
        s &= ~(size_t)1;
        idx_t cnt = 0;
        for (size_t i = 0; i < s; ++i) {
            if (A289027_cur[i]) {
                ++cnt;
                if (cnt == n)
                    return (seq_t)(idx_t)(i + 1);
            }
        }
        A289027_step();
    }
}
