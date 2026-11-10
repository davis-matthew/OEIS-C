/*
 * A285412  Positions of 0 in A285411; complement of A285413.
 * https://oeis.org/A285412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285411, the fixed point of g:
 *            0->010010, 1->1101011 (g is the square of 0->11, 1->010)
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8908 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *A285412_w = NULL;
static size_t A285412_len = 0;
static size_t *A285412_pos = NULL;
static size_t A285412_nz = 0;

static void A285412_extend(void)
{
    if (A285412_w == NULL) {
        A285412_w = (uint8_t *)malloc(1);
        A285412_w[0] = 0;
        A285412_len = 1;
    } else {
        size_t newlen = 0;
        for (size_t i = 0; i < A285412_len; ++i)
            newlen += A285412_w[i] ? 7 : 6;
        uint8_t *nw = (uint8_t *)malloc(newlen);
        static const uint8_t g0[6] = {0, 1, 0, 0, 1, 0};
        static const uint8_t g1[7] = {1, 1, 0, 1, 0, 1, 1};
        size_t p = 0;
        for (size_t i = 0; i < A285412_len; ++i) {
            if (A285412_w[i]) {
                for (int k = 0; k < 7; ++k) nw[p++] = g1[k];
            } else {
                for (int k = 0; k < 6; ++k) nw[p++] = g0[k];
            }
        }
        free(A285412_w);
        A285412_w = nw;
        A285412_len = newlen;
    }
    free(A285412_pos);
    size_t cnt = 0;
    for (size_t i = 0; i < A285412_len; ++i)
        if (!A285412_w[i]) cnt++;
    A285412_pos = (size_t *)malloc((cnt ? cnt : 1) * sizeof(size_t));
    size_t j = 0;
    for (size_t i = 0; i < A285412_len; ++i)
        if (!A285412_w[i]) A285412_pos[j++] = i + 1;
    A285412_nz = cnt;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    size_t k = (size_t)(n - 1);
    while (A285412_w == NULL || A285412_nz <= k)
        A285412_extend();
    return (seq_t)A285412_pos[k];
}
