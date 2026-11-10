/*
 * A285359  Positions of 0 in A285358; complement of A285360.
 * https://oeis.org/A285359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285358, the fixed point of
 *            0->10, 1->1101 starting from 1 (found by expanding the word and
 *            scanning for zeros)
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9861 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned char *A285359_w = NULL;
static size_t A285359_len = 0;
static idx_t *A285359_z = NULL;
static size_t A285359_zc = 0;

static void A285359_rebuild(void)
{
    size_t c = 0;
    for (size_t i = 0; i < A285359_len; ++i)
        if (A285359_w[i] == 0) ++c;
    free(A285359_z);
    A285359_z = (idx_t *)malloc((c + 1) * sizeof(idx_t));
    if (!A285359_z) exit(EXIT_FAILURE);
    size_t k = 0;
    for (size_t i = 0; i < A285359_len; ++i)
        if (A285359_w[i] == 0) A285359_z[k++] = (idx_t)(i + 1);
    A285359_zc = k;
}

static void A285359_expand(void)
{
    size_t nl = 0;
    for (size_t i = 0; i < A285359_len; ++i)
        nl += (A285359_w[i] == 0) ? 2 : 4;
    unsigned char *nw = (unsigned char *)malloc(nl);
    if (!nw) exit(EXIT_FAILURE);
    size_t p = 0;
    for (size_t i = 0; i < A285359_len; ++i) {
        if (A285359_w[i] == 0) {
            nw[p++] = 1; nw[p++] = 0;
        } else {
            nw[p++] = 1; nw[p++] = 1; nw[p++] = 0; nw[p++] = 1;
        }
    }
    free(A285359_w);
    A285359_w = nw;
    A285359_len = nl;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A285359_len == 0) {
        A285359_w = (unsigned char *)malloc(1);
        if (!A285359_w) exit(EXIT_FAILURE);
        A285359_w[0] = 1;
        A285359_len = 1;
        A285359_rebuild();
    }
    if (A285359_zc < (size_t)n) {
        while (A285359_zc < (size_t)n) {
            A285359_expand();
            A285359_rebuild();
        }
    }
    return (seq_t)A285359_z[n - 1];
}
