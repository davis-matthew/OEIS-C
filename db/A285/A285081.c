/*
 * A285081  Positions of 0 in A285080; complement of A285082.
 * https://oeis.org/A285081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285080, the 0-limiting word of
 *            the morphism 0->10, 1->011. Computed by generating the word
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9699 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A285082_w = NULL;
static size_t A285082_len = 0;
static size_t A285082_zeros = 0;

static void A285082_step(void)
{
    size_t len1 = 0, i, pos = 0;
    for (i = 0; i < A285082_len; ++i)
        len1 += (A285082_w[i] == 0) ? 2 : 3;
    unsigned char *t = (unsigned char *)malloc(len1);
    if (!t) abort();
    for (i = 0; i < A285082_len; ++i) {
        if (A285082_w[i] == 0) { t[pos++] = 1; t[pos++] = 0; }
        else { t[pos++] = 0; t[pos++] = 1; t[pos++] = 1; }
    }
    free(A285082_w);
    A285082_w = t;
    A285082_len = len1;
}

static void A285082_grow(void)
{
    A285082_step();
    A285082_step();
    size_t z = 0;
    for (size_t i = 0; i < A285082_len; ++i)
        if (A285082_w[i] == 0) ++z;
    A285082_zeros = z;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!A285082_w) {
        A285082_w = (unsigned char *)malloc(1);
        if (!A285082_w) abort();
        A285082_w[0] = 0;
        A285082_len = 1;
        A285082_zeros = 1;
    }
    while ((idx_t)A285082_zeros < n)
        A285082_grow();
    idx_t cnt = 0;
    for (size_t i = 0; i < A285082_len; ++i) {
        if (A285082_w[i] == 0) {
            ++cnt;
            if (cnt == n) return (seq_t)(unsigned long long)(i + 1);
        }
    }
    return (seq_t)0;
}
