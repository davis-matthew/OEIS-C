/*
 * A387529  Numbers k such that A387527(k) = 2.
 * https://oeis.org/A387529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: k is a term iff (k,k+1) and (k,k+2-1,k+2) pairs have
 *            disjoint prime-exponent sets but (k+2,k+3) does not, i.e.
 *            A387527(k) = 2. One number of each DPE pair must be powerful, so
 *            only k with k or k+1 powerful are tested
 * Range:     u8 n <= 20, u16 n <= 342, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8081 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint32_t *A387529_pr = NULL;
static size_t A387529_npr = 0;
static uint64_t *A387529_terms = NULL;
static size_t A387529_cnt = 0;

static void A387529_sieve(void)
{
    const uint32_t LIM = 1u << 17;
    unsigned char *c = (unsigned char *)calloc(LIM + 1, 1);
    A387529_pr = (uint32_t *)malloc(sizeof(uint32_t) * 20000);
    A387529_npr = 0;
    for (uint32_t i = 2; i <= LIM; i++) {
        if (!c[i]) {
            A387529_pr[A387529_npr++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= LIM; j += i) c[j] = 1;
        }
    }
    free(c);
}

static uint64_t A387529_isqrt(uint64_t x)
{
    if (x < 2) return x;
    uint64_t r = x, y = (r + 1) / 2;
    while (y < r) { r = y; y = (r + x / r) / 2; }
    return r;
}

/* bitmask of the set of prime exponents of m */
static uint64_t A387529_emask(uint64_t m)
{
    uint64_t mk = 0;
    if (m <= 1) return 0;
    for (size_t i = 0; i < A387529_npr; i++) {
        uint64_t p = A387529_pr[i];
        if (p * p * p > m) break;
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            mk |= (uint64_t)1 << (e & 63);
        }
    }
    if (m > 1) {
        uint64_t s = A387529_isqrt(m);
        mk |= (s * s == m) ? ((uint64_t)1 << 2) : ((uint64_t)1 << 1);
    }
    return mk;
}

static bool A387529_good(uint64_t x)
{
    return (A387529_emask(x) & A387529_emask(x + 1)) == 0;
}

static int A387529_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static bool A387529_sqfree(uint64_t b)
{
    for (uint64_t p = 2; p * p <= b; p++)
        if (b % (p * p) == 0) return false;
    return true;
}

static void A387529_build(size_t need)
{
    if (!A387529_pr) A387529_sieve();
    uint64_t L = 1024;
    for (;;) {
        /* enumerate powerful numbers a^2 b^3 <= L, b squarefree */
        size_t cap = 1024, np = 0;
        uint64_t *pw = (uint64_t *)malloc(sizeof(uint64_t) * cap);
        for (uint64_t b = 1; b * b * b <= L; b++) {
            if (!A387529_sqfree(b)) continue;
            uint64_t b3 = b * b * b, q = L / b3;
            for (uint64_t a = 1; a * a <= q; a++) {
                if (np == cap) { cap *= 2; pw = (uint64_t *)realloc(pw, sizeof(uint64_t) * cap); }
                pw[np++] = a * a * b3;
            }
        }
        /* candidates k in {P-1, P} */
        uint64_t *cand = (uint64_t *)malloc(sizeof(uint64_t) * (2 * np + 1));
        size_t nc = 0;
        for (size_t i = 0; i < np; i++) {
            if (pw[i] >= 2) cand[nc++] = pw[i] - 1;
            cand[nc++] = pw[i];
        }
        free(pw);
        qsort(cand, nc, sizeof(uint64_t), A387529_cmp);
        uint64_t *terms = (uint64_t *)malloc(sizeof(uint64_t) * (nc + 1));
        size_t cnt = 0;
        uint64_t prev = 0;
        for (size_t i = 0; i < nc; i++) {
            uint64_t k = cand[i];
            if (k == prev || k == 0 || k > L - 1) continue;
            prev = k;
            if (A387529_good(k) && A387529_good(k + 1) && !A387529_good(k + 2))
                terms[cnt++] = k;
        }
        free(cand);
        free(A387529_terms);
        A387529_terms = terms;
        A387529_cnt = cnt;
        if (cnt >= need) return;
        L *= 4;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    size_t idx = (size_t)n;
    if (idx > A387529_cnt)
        A387529_build(idx);
    if (idx > A387529_cnt) return 0;
    return (seq_t)A387529_terms[idx - 1];
}
