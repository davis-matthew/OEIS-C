/*
 * A276579  Squarefree numbers larger than one for which A268669(A048675(n))
 *          is a power of two.
 * https://oeis.org/A276579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th squarefree k>1 such that A268669(A048675(k)) is a
 *            power of 2; by sieve/search: prime indices of k must be {s+d : d
 *            submask of j} (m = X^s (X+1)^j over GF(2))
 * Range:     u8 n <= 77, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 88 terms through n = 1813 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A276579_spf = NULL;
static uint32_t *A276579_pix = NULL;
static uint64_t A276579_lim = 0;
static uint64_t *A276579_terms = NULL;
static size_t A276579_cnt = 0, A276579_cap = 0;
static uint64_t A276579_cand = 2;

static void A276579_build(uint64_t L)
{
    free(A276579_spf);
    free(A276579_pix);
    A276579_spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    A276579_pix = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    if (!A276579_spf || !A276579_pix) abort();
    uint32_t k = 0;
    for (uint64_t i = 2; i <= L; i++) {
        if (A276579_spf[i] == 0) {
            A276579_spf[i] = (uint32_t)i;
            A276579_pix[i] = ++k;
            for (uint64_t j = i * i; j <= L; j += i)
                if (A276579_spf[j] == 0) A276579_spf[j] = (uint32_t)i;
        }
    }
    A276579_lim = L;
}

static bool A276579_test(uint64_t c)
{
    uint32_t ids[24];
    int t = 0;
    uint64_t x = c;
    while (x > 1) {
        uint32_t p = A276579_spf[x];
        x /= p;
        if (x % p == 0) return false;
        if (t >= 24) return false;
        ids[t++] = A276579_pix[p];
    }
    if (t == 0) return false;
    uint32_t s = ids[0];
    uint32_t j = ids[t - 1] - s;
    int pc = 0;
    for (uint32_t q = j; q; q &= q - 1) pc++;
    if (pc >= 31 || t != (1 << pc)) return false;
    for (int i = 0; i < t; i++) {
        uint32_t d = ids[i] - s;
        if (d & ~j) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    size_t want = (size_t)n;
    while (A276579_cnt < want) {
        if (A276579_cand > A276579_lim)
            A276579_build(A276579_lim ? A276579_lim * 2 : 4096);
        if (A276579_test(A276579_cand)) {
            if (A276579_cnt == A276579_cap) {
                A276579_cap = A276579_cap ? A276579_cap * 2 : 1024;
                A276579_terms = (uint64_t *)realloc(A276579_terms, A276579_cap * sizeof(uint64_t));
                if (!A276579_terms) abort();
            }
            A276579_terms[A276579_cnt++] = A276579_cand;
        }
        A276579_cand++;
    }
    return (seq_t)A276579_terms[want - 1];
}
