/*
 * A284902  Positions of 0 in A284901; complement of A284903.
 * https://oeis.org/A284902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A284901, the fixed point of
 *            0->01, 1->1000 (found by generating the word by iterating the
 *            morphism)
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9803 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A284902_w = NULL;
static idx_t A284902_len = 0;
static idx_t *A284902_pos = NULL;
static idx_t A284902_zc = 0;

static void A284902_expand(void)
{
    idx_t nl = 0;
    for (idx_t i = 0; i < A284902_len; ++i) nl += A284902_w[i] ? 4 : 2;
    unsigned char *nw = (unsigned char *)malloc((size_t)nl);
    idx_t p = 0;
    for (idx_t i = 0; i < A284902_len; ++i) {
        if (A284902_w[i] == 0) { nw[p++] = 0; nw[p++] = 1; }
        else { nw[p++] = 1; nw[p++] = 0; nw[p++] = 0; nw[p++] = 0; }
    }
    free(A284902_w);
    A284902_w = nw;
    A284902_len = nl;
}

static void A284902_index(void)
{
    free(A284902_pos);
    idx_t z = 0;
    for (idx_t i = 0; i < A284902_len; ++i) if (A284902_w[i] == 0) ++z;
    A284902_pos = (idx_t *)malloc((size_t)z * sizeof(idx_t));
    idx_t k = 0;
    for (idx_t i = 0; i < A284902_len; ++i) if (A284902_w[i] == 0) A284902_pos[k++] = i;
    A284902_zc = z;
}

seq_t a(idx_t n)
{
    if (A284902_w == NULL) {
        A284902_w = (unsigned char *)malloc(1);
        A284902_w[0] = 0;
        A284902_len = 1;
        A284902_index();
    }
    if (n > A284902_zc) {
        while (A284902_zc < n) {
            A284902_expand();
            idx_t z = 0;
            for (idx_t i = 0; i < A284902_len; ++i) if (A284902_w[i] == 0) ++z;
            A284902_zc = z;
        }
        A284902_index();
    }
    return (seq_t)(A284902_pos[n - 1] + 1);
}
