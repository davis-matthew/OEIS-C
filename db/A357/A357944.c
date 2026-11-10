/*
 * A357944  If n appears in A357943, a(n) is the smallest k such that
 *          A357943(k) = n, otherwise a(n) = -1.
 * https://oeis.org/A357944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k : A357943(k) = n}, or -1 if no such k; search:
 *            generate A357943 (a(0)=0, a(1)=1, a(2)=2, a(n) = number of
 *            earlier occurrences of a(n-1-a(n-1))) until n first appears
 * Range:     u8 n <= 20, u16 n <= 424, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9657 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint32_t *A357944_s = NULL;      /* A357943 terms (native, they are counts <= index) */
static size_t A357944_len = 0, A357944_scap = 0;
static uint32_t *A357944_freq = NULL;   /* occurrences of each value so far */
static uint32_t *A357944_first = NULL;  /* first index of each value, UINT32_MAX if none */
static size_t A357944_vcap = 0;
static const size_t A357944_LIMIT = (size_t)1 << 30;

static void A357944_ensure_s(size_t need)
{
    if (need <= A357944_scap) return;
    size_t c = A357944_scap ? A357944_scap : 4096;
    while (c < need) c <<= 1;
    uint32_t *t = (uint32_t *)realloc(A357944_s, c * sizeof(uint32_t));
    if (!t) abort();
    A357944_s = t;
    A357944_scap = c;
}

static void A357944_ensure_v(size_t idx)
{
    if (idx < A357944_vcap) return;
    size_t c = A357944_vcap ? A357944_vcap : 4096;
    while (c <= idx) c <<= 1;
    uint32_t *f = (uint32_t *)realloc(A357944_freq, c * sizeof(uint32_t));
    if (!f) abort();
    A357944_freq = f;
    uint32_t *g = (uint32_t *)realloc(A357944_first, c * sizeof(uint32_t));
    if (!g) abort();
    A357944_first = g;
    for (size_t i = A357944_vcap; i < c; ++i) {
        A357944_freq[i] = 0;
        A357944_first[i] = UINT32_MAX;
    }
    A357944_vcap = c;
}

static void A357944_append(uint32_t v)
{
    size_t pos = A357944_len;
    A357944_ensure_s(pos + 1);
    A357944_s[pos] = v;
    ++A357944_len;
    A357944_ensure_v(v);
    ++A357944_freq[v];
    if (A357944_first[v] == UINT32_MAX) A357944_first[v] = (uint32_t)pos;
}

static void A357944_step(void)
{
    size_t m = A357944_len;
    if (m == 0) { A357944_append(0); return; }
    if (m == 1) { A357944_append(1); return; }
    if (m == 2) { A357944_append(2); return; }
    size_t prev = A357944_s[m - 1];
    size_t km = m - 1;
    size_t t = (prev <= km) ? km - prev : 0;
    uint32_t tv = A357944_s[t];
    A357944_ensure_v(tv);
    A357944_append(A357944_freq[tv]);
}

seq_t a(idx_t n)
{
    if (n >= (idx_t)A357944_LIMIT)
        return (seq_t)0 - (seq_t)1;
    size_t v = (size_t)n;
    for (;;) {
        if (v < A357944_vcap && A357944_first[v] != UINT32_MAX)
            return (seq_t)A357944_first[v];
        if (A357944_len >= A357944_LIMIT)
            return (seq_t)0 - (seq_t)1;
        A357944_step();
    }
}
