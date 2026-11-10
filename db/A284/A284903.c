/*
 * A284903  Positions of 1 in A284901; complement of A284902.
 * https://oeis.org/A284903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in the fixed point of 0->01,
 *            1->1000 (A284901); computed by expanding the morphism and
 *            scanning
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9568 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A284903_w = NULL;
static idx_t A284903_len = 0;
static idx_t A284903_ones = 0;
static idx_t *A284903_pos = NULL;

seq_t a(idx_t n)
{
    int grown = 0;
    if (A284903_w == NULL) {
        A284903_w = (unsigned char *)malloc(1);
        A284903_w[0] = 0;
        A284903_len = 1;
        A284903_ones = 0;
        grown = 1;
    }
    while (A284903_ones < n) {
        idx_t nl = 0;
        for (idx_t i = 0; i < A284903_len; ++i)
            nl += A284903_w[i] ? 4 : 2;
        unsigned char *nw = (unsigned char *)malloc((size_t)nl);
        idx_t p = 0, ones = 0;
        for (idx_t i = 0; i < A284903_len; ++i) {
            if (A284903_w[i] == 0) {
                nw[p++] = 0;
                nw[p++] = 1;
            } else {
                nw[p++] = 1;
                nw[p++] = 0;
                nw[p++] = 0;
                nw[p++] = 0;
            }
        }
        for (idx_t i = 0; i < nl; ++i)
            ones += nw[i];
        free(A284903_w);
        A284903_w = nw;
        A284903_len = nl;
        A284903_ones = ones;
        grown = 1;
    }
    if (grown) {
        free(A284903_pos);
        A284903_pos = (idx_t *)malloc((size_t)(A284903_ones + 1) * sizeof(idx_t));
        idx_t k = 0;
        for (idx_t i = 0; i < A284903_len; ++i) {
            if (A284903_w[i])
                A284903_pos[k++] = i + 1;
        }
    }
    return (seq_t)A284903_pos[n - 1];
}
