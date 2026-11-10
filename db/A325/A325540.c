/*
 * A325540  Complement of A325539.
 * https://oeis.org/A325540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th number that is 2x+1 or 3x+2 for some x in A325539
 *            (sieve: greedy build of A325539, complement = marked numbers)
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9003 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static size_t *A325540_list = NULL;
static size_t A325540_cnt = 0;

static void A325540_build(size_t L)
{
    unsigned char *mark = (unsigned char *)calloc(L + 1, 1);
    if (!mark) abort();
    size_t cnt = 0;
    for (size_t x = 1; x <= L; ++x) {
        if (!mark[x]) {
            size_t f1 = 2 * x + 1, f2 = 3 * x + 2;
            if (f1 <= L) mark[f1] = 1;
            if (f2 <= L) mark[f2] = 1;
        } else {
            ++cnt;
        }
    }
    size_t *lst = (size_t *)malloc((cnt + 1) * sizeof(size_t));
    if (!lst) abort();
    size_t k = 0;
    for (size_t x = 1; x <= L; ++x)
        if (mark[x]) lst[k++] = x;
    free(mark);
    free(A325540_list);
    A325540_list = lst;
    A325540_cnt = cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t L = 1024;
    if (A325540_cnt < (size_t)n) {
        while (1) {
            A325540_build(L);
            if (A325540_cnt >= (size_t)n) break;
            L *= 2;
        }
    }
    return (seq_t)A325540_list[(size_t)n - 1];
}
