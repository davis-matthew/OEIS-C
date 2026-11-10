/*
 * A340636  Primes of the form k + A037276(k) in more than one way.
 * https://oeis.org/A340636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th prime p (in increasing order) with p = k +
 *            A037276(k) for at least two distinct k; found by sieving
 *            smallest prime factors for k <= L/2 (since A037276(k) >= k),
 *            collecting primes k + A037276(k) <= L, and keeping repeated
 *            values, with L doubled until enough terms are found
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 7500, u64 n <= 7500,
 *            u128 n <= 7500, big any n
 * Validated: standard, pass, 55 terms through n = 7219 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static int A340636_cmp(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static uint64_t *A340636_terms = 0;
static size_t A340636_cnt = 0;
static uint64_t A340636_limit = 0;

static void A340636_build(uint64_t limit) {
    uint64_t kmax = limit / 2;
    uint32_t *spf = (uint32_t *)calloc((size_t)kmax + 1, sizeof(uint32_t));
    size_t cap = 1024, hc = 0;
    uint64_t *hits = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t i, j, k;
    for (i = 2; i <= kmax; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= kmax)
                for (j = i * i; j <= kmax; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (k = 1; k <= kmax; k++) {
        uint64_t res = 0, m = k;
        bool ok = true;
        if (k == 1) res = 1;
        while (m > 1) {
            uint64_t p = spf[m];
            uint64_t mul = 10, t = p;
            m /= p;
            while (t >= 10) { t /= 10; mul *= 10; }
            if (res > limit / mul) { ok = false; break; }
            res = res * mul + p;
            if (res > limit) { ok = false; break; }
        }
        if (!ok) continue;
        {
            uint64_t s = k + res;
            if (s > limit) continue;
            if ((s & 1) == 0 && s != 2) continue;
            if (!is_prime_u64(s)) continue;
            if (hc == cap) {
                cap *= 2;
                hits = (uint64_t *)realloc(hits, cap * sizeof(uint64_t));
            }
            hits[hc++] = s;
        }
    }
    free(spf);
    qsort(hits, hc, sizeof(uint64_t), A340636_cmp);
    free(A340636_terms);
    A340636_terms = (uint64_t *)malloc((hc + 1) * sizeof(uint64_t));
    A340636_cnt = 0;
    {
        size_t t;
        for (t = 0; t + 1 < hc; t++) {
            if (hits[t] == hits[t + 1]) {
                if (A340636_cnt == 0 || A340636_terms[A340636_cnt - 1] != hits[t])
                    A340636_terms[A340636_cnt++] = hits[t];
            }
        }
    }
    free(hits);
    A340636_limit = limit;
}

seq_t a(idx_t n)
{
    if (A340636_limit == 0) A340636_build((uint64_t)1 << 20);
    while ((uint64_t)n > A340636_cnt && A340636_limit < ((uint64_t)1 << 33))
        A340636_build(A340636_limit * 2);
    if ((uint64_t)n > A340636_cnt) return (seq_t)0;
    return (seq_t)A340636_terms[n - 1];
}
