/*
 * A289240  Positions of 0 in A289239; complement of A289241.
 * https://oeis.org/A289240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A289239 (fixed point of
 *            00->0010, 01->100, 10->010 from 00); computed by expanding the
 *            word
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9239 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned char *A289240_cur = NULL;
static size_t A289240_len = 0;
static size_t A289240_zeros = 0;

static void A289240_expand(void)
{
    size_t pairs = A289240_len / 2;
    unsigned char *nx = (unsigned char *)malloc(pairs * 4 + 4);
    size_t w = 0, z = 0, i;
    for (i = 0; i + 1 < A289240_len; i += 2) {
        unsigned char x = A289240_cur[i], y = A289240_cur[i + 1];
        if (x == 0 && y == 0) {
            nx[w++] = 0; nx[w++] = 0; nx[w++] = 1; nx[w++] = 0;
        } else if (x == 0 && y == 1) {
            nx[w++] = 1; nx[w++] = 0; nx[w++] = 0;
        } else {
            nx[w++] = 0; nx[w++] = 1; nx[w++] = 0;
        }
    }
    for (i = 0; i < w; i++) if (nx[i] == 0) z++;
    free(A289240_cur);
    A289240_cur = nx;
    A289240_len = w;
    A289240_zeros = z;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A289240_len == 0) {
        A289240_cur = (unsigned char *)malloc(2);
        A289240_cur[0] = 0; A289240_cur[1] = 0;
        A289240_len = 2;
        A289240_zeros = 2;
    }
    while (A289240_zeros < (size_t)n) A289240_expand();
    size_t cnt = 0, i;
    for (i = 0; i < A289240_len; i++) {
        if (A289240_cur[i] == 0) {
            cnt++;
            if (cnt == (size_t)n) return (seq_t)(i + 1);
        }
    }
    return 0;
}
