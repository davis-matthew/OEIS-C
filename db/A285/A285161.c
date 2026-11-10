/*
 * A285161  Positions of 1 in A285159; complement of A285160.
 * https://oeis.org/A285161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A285159, the 0-limiting word of
 *            the morphism 0->10, 1->0011 (built by iterating the morphism and
 *            scanning)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9051 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static uint8_t *A285161_w = NULL;
static size_t A285161_len = 0;
static size_t A285161_ones = 0;

static void A285161_step(void)
{
    size_t nl = 0;
    for (size_t i = 0; i < A285161_len; ++i)
        nl += A285161_w[i] ? 4 : 2;
    uint8_t *nw = (uint8_t *)malloc(nl);
    size_t p = 0, ones = 0;
    for (size_t i = 0; i < A285161_len; ++i) {
        if (A285161_w[i] == 0) {
            nw[p++] = 1; nw[p++] = 0;
            ones += 1;
        } else {
            nw[p++] = 0; nw[p++] = 0; nw[p++] = 1; nw[p++] = 1;
            ones += 2;
        }
    }
    free(A285161_w);
    A285161_w = nw;
    A285161_len = nl;
    A285161_ones = ones;
}

seq_t a(idx_t n)
{
    if (n < 1u)
        return (seq_t)0;
    if (A285161_w == NULL) {
        A285161_w = (uint8_t *)malloc(1);
        A285161_w[0] = 0;
        A285161_len = 1;
        A285161_ones = 0;
    }
    while ((idx_t)A285161_ones < n) {
        A285161_step();
        A285161_step();   /* keep even iterations (prefixes of the limit word) */
    }
    idx_t cnt = 0;
    for (size_t i = 0; i < A285161_len; ++i) {
        if (A285161_w[i]) {
            ++cnt;
            if (cnt == n)
                return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
