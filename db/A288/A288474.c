/*
 * A288474  Positions of 0 in A288473; complement of A288475.
 * https://oeis.org/A288474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A288473, the 1-limiting word of
 *            00->0101, 1->011 (found by generating odd iterates and scanning)
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9710 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static void A288474_map(const unsigned char *in, size_t in_len, unsigned char **out, size_t *out_len)
{
    size_t len = 0;
    for (size_t i = 0; i < in_len; ++i) {
        if (in[i] == 0) {
            if (i + 1 < in_len && in[i + 1] == 0) { len += 4; ++i; }
            else len += 1;
        } else len += 3;
    }
    unsigned char *buf = (unsigned char *)malloc(len ? len : 1);
    size_t j = 0;
    for (size_t i = 0; i < in_len; ++i) {
        if (in[i] == 0) {
            if (i + 1 < in_len && in[i + 1] == 0) {
                buf[j++] = 0; buf[j++] = 1; buf[j++] = 0; buf[j++] = 1;
                ++i;
            } else buf[j++] = 0;
        } else {
            buf[j++] = 0; buf[j++] = 1; buf[j++] = 1;
        }
    }
    *out = buf;
    *out_len = len;
}

static size_t A288474_zeros(const unsigned char *w, size_t len)
{
    size_t c = 0;
    for (size_t i = 0; i < len; ++i) if (w[i] == 0) ++c;
    return c;
}

seq_t a(idx_t n)
{
    static unsigned char *word = NULL;
    static size_t wlen = 0;
    static size_t zc = 0;

    if (n == 0) return (seq_t)0;

    if (word == NULL) {
        word = (unsigned char *)malloc(4);
        word[0] = 0; word[1] = 1; word[2] = 0; word[3] = 1;
        wlen = 4;
        zc = 2;
    }

    while ((idx_t)zc < n) {
        unsigned char *ev = NULL, *nw = NULL;
        size_t el = 0, nl = 0;
        A288474_map(word, wlen, &ev, &el);
        A288474_map(ev, el, &nw, &nl);
        free(ev);
        free(word);
        word = nw;
        wlen = nl;
        zc = A288474_zeros(word, wlen);
    }

    idx_t cnt = 0;
    for (size_t i = 0; i < wlen; ++i) {
        if (word[i] == 0) {
            ++cnt;
            if (cnt == n) return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
