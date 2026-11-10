/*
 * A288598  Positions of 1 in A288596; complement of A288597.
 * https://oeis.org/A288598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A288596, the fixed point of
 *            00->0101, 10->1000 starting with 00 (found by generating the
 *            word)
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9137 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint8_t *A288598_w = NULL;
static size_t A288598_len = 0;
static idx_t *A288598_pos = NULL;
static size_t A288598_cnt = 0;

static void A288598_step(void)
{
    size_t L = A288598_len;
    uint8_t *nx = (uint8_t *)malloc(L * 4 + 4);
    size_t j = 0, i;
    for (i = 0; i + 1 < L; ++i) {
        if (A288598_w[i] == 0 && A288598_w[i + 1] == 0) {
            nx[j++] = 0; nx[j++] = 1; nx[j++] = 0; nx[j++] = 1;
            ++i;
        } else if (A288598_w[i] == 1 && A288598_w[i + 1] == 0) {
            nx[j++] = 1; nx[j++] = 0; nx[j++] = 0; nx[j++] = 0;
            ++i;
        } else {
            nx[j++] = A288598_w[i];
        }
    }
    if (!(L >= 2 &&
          ((A288598_w[L - 2] == 0 && A288598_w[L - 1] == 0) ||
           (A288598_w[L - 2] == 1 && A288598_w[L - 1] == 0)))) {
        nx[j++] = A288598_w[L - 1];
    }
    free(A288598_w);
    A288598_w = nx;
    A288598_len = j;
}

static void A288598_build(idx_t n)
{
    size_t i, c;
    if (A288598_w == NULL) {
        A288598_w = (uint8_t *)malloc(2);
        A288598_w[0] = 0; A288598_w[1] = 0;
        A288598_len = 2;
    }
    for (;;) {
        c = 0;
        for (i = 0; i + 1 < A288598_len; ++i) c += A288598_w[i];
        if ((idx_t)c >= n) break;
        A288598_step();
    }
    free(A288598_pos);
    A288598_pos = (idx_t *)malloc((c + 1) * sizeof(idx_t));
    A288598_cnt = 0;
    for (i = 0; i + 1 < A288598_len; ++i)
        if (A288598_w[i]) A288598_pos[A288598_cnt++] = (idx_t)(i + 1);
}

seq_t a(idx_t n)
{
    if (n > (idx_t)A288598_cnt) A288598_build(n);
    return (seq_t)A288598_pos[n - 1];
}
