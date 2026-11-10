/*
 * A284390  Positions of 1 in A284388; complement of A284389.
 * https://oeis.org/A284390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A284388 (0-limiting word of
 *            0->001, 1->11001 iterated); computed by generating the word
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9951 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A284390_w = NULL;
static size_t A284390_len = 0;
static size_t A284390_ones = 0;

static void A284390_grow(void)
{
    size_t nl = 0, no = 0, i, p = 0;
    unsigned char *nw;
    for (i = 0; i < A284390_len; ++i)
        nl += A284390_w[i] ? 5 : 3;
    nw = (unsigned char *)malloc(nl);
    if (!nw) return;
    for (i = 0; i < A284390_len; ++i) {
        if (A284390_w[i]) {
            nw[p++] = 1; nw[p++] = 1; nw[p++] = 0; nw[p++] = 0; nw[p++] = 1;
            no += 3;
        } else {
            nw[p++] = 0; nw[p++] = 0; nw[p++] = 1;
            no += 1;
        }
    }
    free(A284390_w);
    A284390_w = nw;
    A284390_len = nl;
    A284390_ones = no;
}

seq_t a(idx_t n)
{
    size_t i, cnt = 0;
    if (n == 0) return (seq_t)0;
    if (!A284390_w) {
        A284390_w = (unsigned char *)malloc(1);
        if (!A284390_w) return (seq_t)0;
        A284390_w[0] = 0;
        A284390_len = 1;
        A284390_ones = 0;
    }
    while ((idx_t)A284390_ones < n) {
        size_t before = A284390_len;
        A284390_grow();
        if (A284390_len == before) return (seq_t)0;
    }
    for (i = 0; i < A284390_len; ++i) {
        if (A284390_w[i]) {
            ++cnt;
            if ((idx_t)cnt == n) return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
