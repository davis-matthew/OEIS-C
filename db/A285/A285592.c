/*
 * A285592  {00->0, 11->1}-transform of A285568.
 * https://oeis.org/A285592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th term of A285568 (1-limiting word of 0->11, 1->0110)
 *            read left to right, with each 00 -> 0, 11 -> 1 and a lone symbol
 *            kept as is
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9180 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

static unsigned char *A285592_w = NULL;
static size_t A285592_len = 0;

static unsigned char *A285592_expand(const unsigned char *w, size_t len, size_t *out_len)
{
    size_t i, nl = 0, pos = 0;
    unsigned char *r;
    for (i = 0; i < len; ++i) nl += (w[i] == 0) ? 2 : 4;
    r = (unsigned char *)malloc(nl ? nl : 1);
    if (!r) return NULL;
    for (i = 0; i < len; ++i) {
        if (w[i] == 0) {
            r[pos++] = 1; r[pos++] = 1;
        } else {
            r[pos++] = 0; r[pos++] = 1; r[pos++] = 1; r[pos++] = 0;
        }
    }
    *out_len = nl;
    return r;
}

static int A285592_ensure(size_t need)
{
    if (A285592_w == NULL) {
        A285592_w = (unsigned char *)malloc(2);
        if (!A285592_w) return 0;
        A285592_w[0] = 1; A285592_w[1] = 1;
        A285592_len = 2;
    }
    while (A285592_len < need) {
        size_t l1, l2;
        unsigned char *t = A285592_expand(A285592_w, A285592_len, &l1);
        unsigned char *u;
        if (!t) return 0;
        u = A285592_expand(t, l1, &l2);
        free(t);
        if (!u) return 0;
        free(A285592_w);
        A285592_w = u;
        A285592_len = l2;
    }
    return 1;
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n * 2 + 4;
    size_t i = 0;
    idx_t count = 0;
    if (!A285592_ensure(need)) return 0;
    while (i < A285592_len) {
        unsigned char bit = A285592_w[i];
        if (i + 1 < A285592_len && A285592_w[i + 1] == bit) i += 2;
        else i += 1;
        ++count;
        if (count == n) return (seq_t)bit;
    }
    return 0;
}
