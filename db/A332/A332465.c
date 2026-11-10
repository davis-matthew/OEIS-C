/*
 * A332465  Numbers n for which A269174(sigma(n)) is equal to 2*A269174(n).
 * https://oeis.org/A332465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number k with A269174(sigma(k)) = A269174(2k), found
 *            by a search, not a closed formula. Here A269174(x) = (x OR 2x)
 *            AND ((x XOR 2x) OR (x XOR 4x)). Every k <= N is written as k =
 *            M*p^e, where p is its largest prime. The search walks a tree of
 *            such M and tests each one directly. When t...
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 113, u64 n <= 113,
 *            u128 n <= 113, big any n
 * Validated: standard, pass, 59 terms through n = 113 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t A332465_f(uint64_t m)
{
    uint64_t t = m << 1, q = m << 2;
    return (m | t) & ((m ^ t) | (m ^ q));
}

static uint64_t *A332465_res = NULL;
static size_t A332465_rlen = 0, A332465_rcap = 0;
static uint64_t A332465_N = 0;
static uint32_t *A332465_pr = NULL;
static size_t A332465_np = 0;

/* leaf-search parameters */
static uint64_t A332465_M, A332465_S, A332465_pmin, A332465_L;
static int A332465_B;

static void A332465_add(uint64_t v)
{
    if (A332465_rlen == A332465_rcap) {
        A332465_rcap = A332465_rcap ? A332465_rcap * 2 : 256;
        A332465_res = (uint64_t *)realloc(A332465_res, A332465_rcap * sizeof(uint64_t));
    }
    A332465_res[A332465_rlen++] = v;
}

static uint64_t A332465_isqrt(uint64_t x)
{
    uint64_t r = 0;
    uint64_t b = (uint64_t)1 << 32;
    while (b) {
        uint64_t c = r + b;
        if (c <= 0xFFFFFFFFULL && c * c <= x) r = c;
        b >>= 1;
    }
    return r;
}

static void A332465_bitdfs(int j, uint64_t r)
{
    if (r > A332465_L) return;
    if (j > 0) {
        uint64_t mask = (j >= 64) ? ~(uint64_t)0 : (((uint64_t)1 << j) - 1);
        uint64_t ox = A332465_f(A332465_S * (r + 1)) & mask;
        uint64_t oy = A332465_f(2 * A332465_M * r) & mask;
        if (ox != oy) return;
    }
    if (j == A332465_B) {
        if (r >= A332465_pmin &&
            A332465_f(A332465_S * (r + 1)) == A332465_f(2 * A332465_M * r) &&
            is_prime_u64(r))
            A332465_add(A332465_M * r);
        return;
    }
    A332465_bitdfs(j + 1, r);
    A332465_bitdfs(j + 1, r | ((uint64_t)1 << j));
}

static void A332465_node(uint64_t M, uint64_t S, size_t gi)
{
    uint64_t N = A332465_N;
    uint64_t L = N / M;
    size_t i;

    if (A332465_f(S) == A332465_f(2 * M)) A332465_add(M);

    /* leaves: n = M*p, p prime, p > gpf(M), M*p^2 > N */
    {
        uint64_t g = gi ? (uint64_t)A332465_pr[gi - 1] : 1;
        uint64_t pmin = A332465_isqrt(L) + 1;
        if (pmin < g + 1) pmin = g + 1;
        if (pmin <= L) {
            int B = 0;
            uint64_t t = L;
            while (t) { t >>= 1; B++; }
            A332465_M = M; A332465_S = S; A332465_pmin = pmin;
            A332465_L = L; A332465_B = B;
            A332465_bitdfs(0, 0);
        }
    }

    for (i = gi; i < A332465_np; i++) {
        uint64_t p = A332465_pr[i];
        if (p * p > L) break;
        uint64_t pe = p, sp = 1 + p;
        while (pe <= L) {
            A332465_node(M * pe, S * sp, i + 1);
            pe *= p;
            sp += pe;
        }
    }
}

static int A332465_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void A332465_run(uint64_t N)
{
    uint64_t lim = A332465_isqrt(N) + 2, i, j;
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    free(A332465_pr);
    A332465_pr = (uint32_t *)malloc(((size_t)lim + 1) * sizeof(uint32_t));
    A332465_np = 0;
    for (i = 2; i <= lim; i++) {
        if (comp[i]) continue;
        A332465_pr[A332465_np++] = (uint32_t)i;
        for (j = i * i; j <= lim; j += i) comp[j] = 1;
    }
    free(comp);

    A332465_N = N;
    A332465_rlen = 0;
    A332465_node(1, 1, 0);
    qsort(A332465_res, A332465_rlen, sizeof(uint64_t), A332465_cmp);
}

seq_t a(idx_t n)
{
    while (A332465_rlen < (size_t)n) {
        uint64_t N = A332465_N ? A332465_N * 4 : ((uint64_t)1 << 16);
        A332465_run(N);
    }
    return (seq_t)A332465_res[n - 1];
}
