/*
 * A287088  Positions of 1 in A287086.
 * https://oeis.org/A287088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A287086, the fixed point of the
 *            morphism 0->01, 1->22, 2->0 starting from 0 (computed by
 *            iterating the substitution)
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9786 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A287088_s = NULL;
static size_t A287088_len = 0;
static idx_t *A287088_pos = NULL;
static size_t A287088_cnt = 0;

static void A287088_expand(void)
{
    if (A287088_len == 0) {
        A287088_s = (unsigned char *)malloc(1);
        A287088_s[0] = 0;
        A287088_len = 1;
    } else {
        size_t nl = 0;
        for (size_t i = 0; i < A287088_len; ++i)
            nl += (A287088_s[i] == 2) ? 1 : 2;
        unsigned char *nx = (unsigned char *)malloc(nl);
        size_t p = 0;
        for (size_t i = 0; i < A287088_len; ++i) {
            unsigned char c = A287088_s[i];
            if (c == 0) { nx[p++] = 0; nx[p++] = 1; }
            else if (c == 1) { nx[p++] = 2; nx[p++] = 2; }
            else { nx[p++] = 0; }
        }
        free(A287088_s);
        A287088_s = nx;
        A287088_len = nl;
    }
    size_t c = 0;
    for (size_t i = 0; i < A287088_len; ++i)
        if (A287088_s[i] == 1) ++c;
    free(A287088_pos);
    A287088_pos = (idx_t *)malloc((c + 1) * sizeof(idx_t));
    size_t k = 0;
    for (size_t i = 0; i < A287088_len; ++i)
        if (A287088_s[i] == 1) A287088_pos[k++] = (idx_t)(i + 1);
    A287088_cnt = k;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while ((idx_t)A287088_cnt < n)
        A287088_expand();
    return (seq_t)A287088_pos[n - 1];
}
