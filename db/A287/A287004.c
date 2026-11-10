/*
 * A287004  Positions of 1 in A287002.
 * https://oeis.org/A287004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A287002, the fixed point of
 *            0->01, 1->20, 2->1 starting from 0 (found by generating the
 *            word)
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9229 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static unsigned char *A287004_w = NULL;
static size_t A287004_len = 0;
static size_t *A287004_pos = NULL;
static size_t A287004_cnt = 0;

static void A287004_extend(size_t n)
{
    if (!A287004_w) {
        A287004_w = (unsigned char *)malloc(1);
        A287004_w[0] = 0;
        A287004_len = 1;
    }
    while (A287004_cnt < n) {
        size_t newlen = 0, i, p = 0;
        for (i = 0; i < A287004_len; i++)
            newlen += (A287004_w[i] == 2) ? 1 : 2;
        unsigned char *t = (unsigned char *)malloc(newlen);
        for (i = 0; i < A287004_len; i++) {
            unsigned char v = A287004_w[i];
            if (v == 0) { t[p++] = 0; t[p++] = 1; }
            else if (v == 1) { t[p++] = 2; t[p++] = 0; }
            else t[p++] = 1;
        }
        free(A287004_w);
        A287004_w = t;
        A287004_len = newlen;
        free(A287004_pos);
        A287004_pos = (size_t *)malloc(sizeof(size_t) * (newlen + 1));
        A287004_cnt = 0;
        for (i = 0; i < newlen; i++)
            if (t[i] == 1) A287004_pos[A287004_cnt++] = i + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    A287004_extend((size_t)n);
    return (seq_t)A287004_pos[(size_t)n - 1];
}
