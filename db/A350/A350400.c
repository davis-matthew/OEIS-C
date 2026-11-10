/*
 * A350400  a(n) is the least k with A350399(k) = n, or 0 if there is no such
 *          k.
 * https://oeis.org/A350400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k >= 1 : A350399(k) = n}, found by searching k = 1,
 *            2, 3, ... where A350399(k) = #{primes p <= q, p+q = 2k, (p*q mod
 *            2k) prime}. Since q = 2k - p, the residue p*q mod 2k equals
 *            (-p^2) mod 2k. The counts are cached between calls. The search
 *            returns 0 once k is far past the last k whose...
 * Range:     u8 n <= 18, u16 n <= 721, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 991 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static unsigned char *A350400_comp = 0;   /* 1 = composite or < 2 */
static uint64_t A350400_lim = 0;
static uint32_t *A350400_pr = 0;          /* primes <= lim */
static uint64_t A350400_np = 0;
static uint32_t *A350400_cnt = 0;         /* cnt[k] = A350399(k) */
static uint64_t A350400_have = 0;
static uint64_t A350400_cap = 0;

static void A350400_sieve(uint64_t L)
{
    if (A350400_comp) free(A350400_comp);
    if (A350400_pr) free(A350400_pr);
    A350400_comp = (unsigned char *)calloc((size_t)(L + 1), 1);
    A350400_lim = L;
    A350400_comp[0] = 1;
    if (L >= 1) A350400_comp[1] = 1;
    for (uint64_t i = 2; i * i <= L; i++)
        if (!A350400_comp[i])
            for (uint64_t j = i * i; j <= L; j += i)
                A350400_comp[j] = 1;
    uint64_t c = 0;
    for (uint64_t i = 2; i <= L; i++) if (!A350400_comp[i]) c++;
    A350400_pr = (uint32_t *)malloc((size_t)(c + 1) * sizeof(uint32_t));
    A350400_np = 0;
    for (uint64_t i = 2; i <= L; i++)
        if (!A350400_comp[i]) A350400_pr[A350400_np++] = (uint32_t)i;
}

static void A350400_extend(void)
{
    uint64_t k = A350400_have + 1;
    uint64_t m = 2 * k;
    if (m > A350400_lim) {
        uint64_t L = A350400_lim ? A350400_lim * 2 : 8192;
        while (L < m) L *= 2;
        A350400_sieve(L);
    }
    if (k >= A350400_cap) {
        uint64_t nc = A350400_cap ? A350400_cap * 2 : 1024;
        while (nc <= k) nc *= 2;
        A350400_cnt = (uint32_t *)realloc(A350400_cnt, (size_t)nc * sizeof(uint32_t));
        A350400_cap = nc;
    }
    const unsigned char *comp = A350400_comp;
    const uint32_t *pr = A350400_pr;
    uint64_t c = 0;
    /* p = 2 never contributes (only k = 2, residue 0), so start at p = 3 */
    for (uint64_t i = 1; i < A350400_np; i++) {
        uint64_t p = pr[i];
        if (p > k) break;
        if (comp[m - p]) continue;
        uint64_t r = (p * p) % m;
        r = r ? m - r : 0;
        if (!comp[r]) c++;
    }
    A350400_cnt[k] = (uint32_t)c;
    A350400_have = k;
}

seq_t a(idx_t n)
{
    uint64_t lastle = 0;   /* last k with A350399(k) <= n */
    for (uint64_t k = 1;; k++) {
        while (A350400_have < k) A350400_extend();
        uint64_t c = A350400_cnt[k];
        if (c == (uint64_t)n) return (seq_t)k;
        if (c < (uint64_t)n) lastle = k;
        if (k > 4 * lastle + 20000) return (seq_t)0;
    }
}
