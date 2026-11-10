/*
 * A357491  Distinct values in A356784, in order of appearance.
 * https://oeis.org/A357491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th distinct value of A356784 in order of first
 *            appearance; computed by generating the inventory table rows
 *            (doubling the flattened length) and scanning for first
 *            occurrences
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 84 terms through n = 7986 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static size_t *A357491_dist = NULL;
static size_t A357491_cnt = 0;

static void A357491_build(size_t L)
{
    size_t *flat = (size_t *)calloc(L, sizeof(size_t));
    size_t len = 1;
    if (!flat) abort();
    flat[0] = 0;
    while (len < L) {
        size_t *cnt = (size_t *)calloc(len, sizeof(size_t));
        size_t *off = (size_t *)calloc(len, sizeof(size_t));
        size_t i, v;
        if (!cnt || !off) abort();
        for (i = 0; i < len; ++i) cnt[flat[i]]++;
        for (v = 1; v < len; ++v) off[v] = off[v - 1] + cnt[v - 1];
        for (i = 0; i < len; ++i) {
            size_t val = flat[i];
            flat[len + off[val]] = i;
            off[val]++;
        }
        len *= 2;
        free(cnt);
        free(off);
    }
    {
        unsigned char *seen = (unsigned char *)calloc(L, 1);
        size_t *d = (size_t *)malloc(L * sizeof(size_t));
        size_t c = 0, i;
        if (!seen || !d) abort();
        for (i = 0; i < L; ++i) {
            size_t val = flat[i];
            if (!seen[val]) {
                seen[val] = 1;
                d[c++] = val;
            }
        }
        free(seen);
        free(A357491_dist);
        A357491_dist = d;
        A357491_cnt = c;
    }
    free(flat);
}

seq_t a(idx_t n)
{
    size_t L = 2;
    if ((size_t)n >= A357491_cnt) {
        for (;;) {
            A357491_build(L);
            if ((size_t)n < A357491_cnt) break;
            L *= 2;
        }
    }
    return (seq_t)A357491_dist[(size_t)n];
}
