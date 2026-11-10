/*
 * A273872  Subword complexity (number of distinct blocks of length n) of
 *          sequence A189576.
 * https://oeis.org/A273872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct length-n factors of the fixed point of
 *            0->01, 1->110 (A189576); computed by hashing all windows of a
 *            long prefix and counting distinct ones
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static int A273872_cmp(const void *x, const void *y)
{
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return (p > q) - (p < q);
}

static uint64_t A273872_mulm(uint64_t a, uint64_t b)
{
    const uint64_t M = (((uint64_t)1) << 61) - 1;
    unsigned __int128 t = (unsigned __int128)a * b;
    uint64_t lo = (uint64_t)(t & M);
    uint64_t hi = (uint64_t)(t >> 61);
    uint64_t r = lo + hi;
    if (r >= M) r -= M;
    return r;
}

seq_t a(idx_t n)
{
    const uint64_t M = (((uint64_t)1) << 61) - 1;
    const uint64_t B = 1000003123456789ULL % M;
    size_t N = (size_t)n;
    size_t L = 100 * N + 1000;

    uint8_t *s = (uint8_t *)malloc(L * 3 + 8);
    uint8_t *t = (uint8_t *)malloc(L * 3 + 8);
    if (!s || !t) { free(s); free(t); return 0; }
    size_t len = 1;
    s[0] = 0;
    while (len < L) {
        size_t p = 0;
        for (size_t i = 0; i < len && p < L + 3; ++i) {
            if (s[i] == 0) { t[p++] = 0; t[p++] = 1; }
            else { t[p++] = 1; t[p++] = 1; t[p++] = 0; }
        }
        uint8_t *tmp = s; s = t; t = tmp;
        len = p;
        if (len > L) len = L;
        if (len >= L) break;
    }
    free(t);
    if (len > L) len = L;

    size_t W = len - N + 1;
    uint64_t *h = (uint64_t *)malloc(W * sizeof(uint64_t));
    if (!h) { free(s); return 0; }

    uint64_t bn = 1;
    for (size_t i = 0; i < N; ++i) bn = A273872_mulm(bn, B);

    uint64_t cur = 0;
    for (size_t i = 0; i < N; ++i) {
        cur = A273872_mulm(cur, B) + (uint64_t)s[i] + 1;
        if (cur >= M) cur -= M;
    }
    h[0] = cur;
    for (size_t i = 1; i < W; ++i) {
        cur = A273872_mulm(cur, B) + (uint64_t)s[i + N - 1] + 1;
        if (cur >= M) cur -= M;
        uint64_t sub = A273872_mulm(bn, (uint64_t)s[i - 1] + 1);
        cur = (cur + M - sub) % M;
        h[i] = cur;
    }
    free(s);
    qsort(h, W, sizeof(uint64_t), A273872_cmp);
    uint64_t cnt = 1;
    for (size_t i = 1; i < W; ++i)
        if (h[i] != h[i - 1]) ++cnt;
    free(h);
    return (seq_t)cnt;
}
