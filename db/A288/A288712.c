/*
 * A288712  Positions of 0 in A288711; complement of A288713.
 * https://oeis.org/A288712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A288711 (1-limiting word of
 *            00->1000, 10->00, start 00); computed by iterating the morphism
 * Range:     u8 n <= 206, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9942 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *A288712_w = NULL;
static idx_t A288712_len = 0;
static idx_t A288712_zeros = 0;

static void A288712_step(void)
{
    idx_t nl = 0;
    for (idx_t i = 0; i < A288712_len; i += 2)
        nl += (A288712_w[i] == 0) ? 4 : 2;
    uint8_t *nw = (uint8_t *)malloc((size_t)nl);
    idx_t k = 0;
    for (idx_t i = 0; i < A288712_len; i += 2) {
        if (A288712_w[i] == 0) {
            nw[k++] = 1; nw[k++] = 0; nw[k++] = 0; nw[k++] = 0;
        } else {
            nw[k++] = 0; nw[k++] = 0;
        }
    }
    free(A288712_w);
    A288712_w = nw;
    A288712_len = nl;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A288712_w == NULL) {
        A288712_len = 4;
        A288712_w = (uint8_t *)malloc(4);
        A288712_w[0] = 1; A288712_w[1] = 0; A288712_w[2] = 0; A288712_w[3] = 0;
        A288712_zeros = 3;
    }
    while (A288712_zeros < n) {
        /* two steps keep us on the iterates that start with 1 */
        A288712_step();
        A288712_step();
        idx_t z = 0;
        for (idx_t i = 0; i < A288712_len; i++)
            if (A288712_w[i] == 0) z++;
        A288712_zeros = z;
    }
    idx_t c = 0;
    for (idx_t i = 0; i < A288712_len; i++) {
        if (A288712_w[i] == 0) {
            c++;
            if (c == n) return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
