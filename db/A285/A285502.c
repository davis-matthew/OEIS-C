/*
 * A285502  Positions of 0 in A285501; complement of A285503.
 * https://oeis.org/A285502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285501, the fixed point of
 *            0->00110011, 1->111100110011 (the square of 0->11, 1->0011);
 *            computed by expanding the word and scanning
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9427 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned char *a285502_w = NULL;
static uint64_t a285502_len = 0;
static uint64_t a285502_zeros = 0;

static void a285502_expand(void)
{
    uint64_t newlen = 0, i, p = 0, z = 0;
    unsigned char *nw;
    for (i = 0; i < a285502_len; i++)
        newlen += a285502_w[i] ? 12 : 8;
    nw = (unsigned char *)malloc((size_t)newlen);
    if (!nw) return;
    for (i = 0; i < a285502_len; i++) {
        const char *s = a285502_w[i] ? "111100110011" : "00110011";
        size_t k, m = a285502_w[i] ? 12 : 8;
        for (k = 0; k < m; k++) {
            unsigned char c = (unsigned char)(s[k] - '0');
            nw[p++] = c;
            if (!c) z++;
        }
    }
    free(a285502_w);
    a285502_w = nw;
    a285502_len = newlen;
    a285502_zeros = z;
}

seq_t a(idx_t n)
{
    uint64_t i, cnt = 0;
    if (n < 1) return (seq_t)0;
    if (a285502_len == 0) {
        a285502_w = (unsigned char *)malloc(1);
        if (!a285502_w) return (seq_t)0;
        a285502_w[0] = 0;
        a285502_len = 1;
        a285502_zeros = 1;
    }
    while (a285502_zeros < (uint64_t)n) {
        uint64_t before = a285502_len;
        a285502_expand();
        if (a285502_len == before) return (seq_t)0;
    }
    for (i = 0; i < a285502_len; i++) {
        if (!a285502_w[i]) {
            cnt++;
            if (cnt == (uint64_t)n) return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
