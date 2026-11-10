/*
 * A287081  Positions of 2 in A287002.
 * https://oeis.org/A287081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 2 in the fixed point of the morphism
 *            0->01, 1->20, 2->1 (A287002); computed by iterating the morphism
 *            and scanning
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 8313 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static unsigned char *A287081_w = NULL;
static size_t A287081_len = 0;
static uint64_t *A287081_pos = NULL;
static size_t A287081_cnt = 0;

static void A287081_extend(void)
{
    if (!A287081_w) {
        A287081_w = (unsigned char *)malloc(1);
        A287081_w[0] = 0;
        A287081_len = 1;
    }
    size_t newlen = 0;
    for (size_t i = 0; i < A287081_len; ++i)
        newlen += (A287081_w[i] == 2) ? 1 : 2;
    unsigned char *t = (unsigned char *)malloc(newlen);
    size_t p = 0;
    for (size_t i = 0; i < A287081_len; ++i) {
        unsigned char v = A287081_w[i];
        if (v == 0) { t[p++] = 0; t[p++] = 1; }
        else if (v == 1) { t[p++] = 2; t[p++] = 0; }
        else { t[p++] = 1; }
    }
    free(A287081_w);
    A287081_w = t;
    A287081_len = newlen;

    size_t c = 0;
    for (size_t i = 0; i < A287081_len; ++i)
        if (A287081_w[i] == 2) ++c;
    free(A287081_pos);
    A287081_pos = (uint64_t *)malloc((c ? c : 1) * sizeof(uint64_t));
    size_t k = 0;
    for (size_t i = 0; i < A287081_len; ++i)
        if (A287081_w[i] == 2) A287081_pos[k++] = (uint64_t)i + 1;
    A287081_cnt = c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A287081_cnt < (size_t)n)
        A287081_extend();
    return (seq_t)A287081_pos[n - 1];
}
