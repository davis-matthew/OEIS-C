/*
 * A285164  Positions of 1 in A285162; complement of A285163.
 * https://oeis.org/A285164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A285162, the 1-limiting word of
 *            0->10, 1->0011 (computed by iterating the derived morphism
 *            0->001110, 1->101000110011 from "1" and scanning)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9337 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint8_t *A285164_w = NULL;
static size_t A285164_len = 0;

static void A285164_expand(void)
{
    static const uint8_t g0[6] = {0,0,1,1,1,0};
    static const uint8_t g1[12] = {1,0,1,0,0,0,1,1,0,0,1,1};
    size_t nl = 0, i, pos = 0;
    uint8_t *nw;
    for (i = 0; i < A285164_len; ++i)
        nl += A285164_w[i] ? 12 : 6;
    nw = (uint8_t *)malloc(nl);
    for (i = 0; i < A285164_len; ++i) {
        if (A285164_w[i]) { memcpy(nw + pos, g1, 12); pos += 12; }
        else { memcpy(nw + pos, g0, 6); pos += 6; }
    }
    free(A285164_w);
    A285164_w = nw;
    A285164_len = nl;
}

seq_t a(idx_t n)
{
    size_t i, cnt, target;
    if (n == 0) return (seq_t)0;
    target = (size_t)n;
    if (A285164_w == NULL) {
        A285164_w = (uint8_t *)malloc(1);
        A285164_w[0] = 1;
        A285164_len = 1;
    }
    for (;;) {
        cnt = 0;
        for (i = 0; i < A285164_len; ++i) {
            if (A285164_w[i]) {
                if (++cnt == target) return (seq_t)(i + 1);
            }
        }
        A285164_expand();
    }
}
