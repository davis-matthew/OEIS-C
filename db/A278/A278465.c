/*
 * A278465  Numbers that are the sum of one or two terms of A035928.
 * https://oeis.org/A278465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest number of the form t or t+u with t,u in
 *            A035928 (search: generate terms by bit length, form all pair
 *            sums, sort, dedupe)
 * Range:     u8 n <= 19, u16 n <= 3063, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9925 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static int A278465_cmp(const void *x, const void *y)
{
    seq_t p = *(const seq_t *)x;
    seq_t q = *(const seq_t *)y;
    return (p == q) ? 0 : (p < q ? -1 : 1);
}

static seq_t *A278465_res = NULL;
static size_t A278465_cnt = 0;
static unsigned A278465_L = 0;

static void A278465_build(unsigned L)
{
    size_t nt = 0;
    unsigned len;
    for (len = 2; len <= L; len += 2)
        nt += (size_t)1 << (len / 2 - 1);

    seq_t *t = mem_new(seq_t, nt);
    size_t pos = 0;
    for (len = 2; len <= L; len += 2) {
        unsigned k = len / 2;
        size_t count = (size_t)1 << (k - 1);
        for (size_t mask = 0; mask < count; ++mask) {
            seq_t val = (seq_t)1 << (len - 1);
            for (unsigned i = 1; i < k; ++i) {
                if ((mask >> (i - 1)) & 1)
                    val |= (seq_t)1 << i;
                else
                    val |= (seq_t)1 << (len - 1 - i);
            }
            t[pos++] = val;
        }
    }

    seq_t lim = (seq_t)1 << L;
    size_t cap = 1024, m = 0;
    seq_t *s = mem_new(seq_t, cap);
    for (size_t i = 0; i < nt; ++i) {
        if (t[i] < lim) {
            if (m == cap) { cap <<= 1; s = mem_resize(seq_t, s, cap); }
            s[m++] = t[i];
        }
        for (size_t j = i; j < nt; ++j) {
            seq_t sum = t[i] + t[j];
            if (sum < lim && sum >= t[i]) {
                if (m == cap) { cap <<= 1; s = mem_resize(seq_t, s, cap); }
                s[m++] = sum;
            }
        }
    }
    mem_free(t);
    mem_qsort(s, m, sizeof(seq_t), A278465_cmp);
    size_t u = 0;
    for (size_t i = 0; i < m; ++i) {
        if (u == 0 || s[i] != s[u - 1])
            s[u++] = s[i];
    }
    if (A278465_res) mem_free(A278465_res);
    A278465_res = s;
    A278465_cnt = u;
    A278465_L = L;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    unsigned L = A278465_L ? A278465_L : 6;
    if (A278465_L == 0) A278465_build(L);
    while ((idx_t)A278465_cnt < n) {
        L += 2;
        A278465_build(L);
    }
    return A278465_res[(size_t)(n - 1)];
}
