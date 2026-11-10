/*
 * A285256  Positions of 0 in A285255; complement of A285257.
 * https://oeis.org/A285256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285255, the 0-limiting word of
 *            0->10, 1->0110 (computed by generating the word; 2n-a(n) in
 *            {0,1})
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9836 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *A285256_w = NULL;
static size_t A285256_len = 0;
static unsigned A285256_iter = 0;
static uint64_t *A285256_z = NULL;
static size_t A285256_zc = 0;

static void A285256_step(void)
{
    size_t nl = 0, i, p = 0;
    for (i = 0; i < A285256_len; ++i) nl += A285256_w[i] ? 4 : 2;
    uint8_t *nw = (uint8_t*)malloc(nl);
    if (!nw) exit(EXIT_FAILURE);
    for (i = 0; i < A285256_len; ++i) {
        if (A285256_w[i]) {
            nw[p++] = 0; nw[p++] = 1; nw[p++] = 1; nw[p++] = 0;
        } else {
            nw[p++] = 1; nw[p++] = 0;
        }
    }
    free(A285256_w);
    A285256_w = nw;
    A285256_len = nl;
    A285256_iter++;
}

static void A285256_rebuild(void)
{
    size_t i, c = 0;
    free(A285256_z);
    for (i = 0; i < A285256_len; ++i) if (!A285256_w[i]) c++;
    A285256_z = (uint64_t*)malloc(sizeof(uint64_t) * (c ? c : 1));
    if (!A285256_z) exit(EXIT_FAILURE);
    c = 0;
    for (i = 0; i < A285256_len; ++i) if (!A285256_w[i]) A285256_z[c++] = (uint64_t)i + 1;
    A285256_zc = c;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A285256_w == NULL) {
        A285256_w = (uint8_t*)malloc(1);
        A285256_w[0] = 0;
        A285256_len = 1;
        A285256_iter = 0;
        A285256_zc = 0;
    }
    if (A285256_zc < (size_t)n || A285256_z == NULL) {
        while (A285256_len < (size_t)(2 * n + 2) || (A285256_iter & 1))
            A285256_step();
        A285256_rebuild();
        while (A285256_zc < (size_t)n) {
            A285256_step();
            A285256_step();
            A285256_rebuild();
        }
    }
    return (seq_t)A285256_z[(size_t)(n - 1)];
}
