/*
 * A284472  Positions of 0 in A284471; complement of A284473.
 * https://oeis.org/A284472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A284471, the limit word of
 *            0->1, 1->0100 (via 0->0100, 1->1010011 from '1'); computed by
 *            expanding the word and scanning
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8460 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A284472_w = NULL;
static size_t A284472_len = 0;
static idx_t *A284472_zp = NULL;
static size_t A284472_zc = 0;

static void A284472_expand(void)
{
    size_t nl = 0, i, p = 0;
    for (i = 0; i < A284472_len; ++i) nl += A284472_w[i] ? 7 : 4;
    unsigned char *nw = (unsigned char *)malloc(nl);
    static const unsigned char s0[4] = {0, 1, 0, 0};
    static const unsigned char s1[7] = {1, 0, 1, 0, 0, 1, 1};
    for (i = 0; i < A284472_len; ++i) {
        if (A284472_w[i]) {
            for (int k = 0; k < 7; ++k) nw[p++] = s1[k];
        } else {
            for (int k = 0; k < 4; ++k) nw[p++] = s0[k];
        }
    }
    free(A284472_w);
    A284472_w = nw;
    A284472_len = nl;

    size_t zc = 0;
    for (i = 0; i < nl; ++i) if (!nw[i]) ++zc;
    free(A284472_zp);
    A284472_zp = (idx_t *)malloc(zc * sizeof(idx_t));
    zc = 0;
    for (i = 0; i < nl; ++i) if (!nw[i]) A284472_zp[zc++] = (idx_t)(i + 1);
    A284472_zc = zc;
}

seq_t a(idx_t n)
{
    if (A284472_w == NULL) {
        A284472_w = (unsigned char *)malloc(1);
        A284472_w[0] = 1;
        A284472_len = 1;
        A284472_zc = 0;
    }
    while (A284472_zc < (size_t)n) A284472_expand();
    return (seq_t)A284472_zp[(size_t)(n - 1)];
}
