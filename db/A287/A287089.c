/*
 * A287089  Positions of 2 in A287086.
 * https://oeis.org/A287089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 2 in A287086, the fixed point of
 *            0->01, 1->22, 2->0 starting from 0 (computed by iterating the
 *            morphism)
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9619 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A287089_s = NULL;
static size_t A287089_len = 0;
static idx_t *A287089_pos = NULL;
static idx_t A287089_cnt = 0;

static void A287089_extend(idx_t n)
{
    if (A287089_len == 0) {
        A287089_s = (unsigned char *)malloc(1);
        A287089_s[0] = 0;
        A287089_len = 1;
    }
    while (1) {
        idx_t c = 0;
        for (size_t i = 0; i < A287089_len; ++i)
            if (A287089_s[i] == 2) ++c;
        if (c >= n) {
            free(A287089_pos);
            A287089_pos = (idx_t *)malloc(sizeof(idx_t) * (size_t)(c ? c : 1));
            idx_t k = 0;
            for (size_t i = 0; i < A287089_len; ++i)
                if (A287089_s[i] == 2) A287089_pos[k++] = (idx_t)(i + 1);
            A287089_cnt = c;
            return;
        }
        size_t newlen = 0;
        for (size_t i = 0; i < A287089_len; ++i)
            newlen += (A287089_s[i] == 2) ? 1 : 2;
        unsigned char *nx = (unsigned char *)malloc(newlen);
        size_t p = 0;
        for (size_t i = 0; i < A287089_len; ++i) {
            unsigned char ch = A287089_s[i];
            if (ch == 0) { nx[p++] = 0; nx[p++] = 1; }
            else if (ch == 1) { nx[p++] = 2; nx[p++] = 2; }
            else { nx[p++] = 0; }
        }
        free(A287089_s);
        A287089_s = nx;
        A287089_len = newlen;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A287089_cnt < n) A287089_extend(n);
    return (seq_t)A287089_pos[n - 1];
}
