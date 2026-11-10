/*
 * A381434  Numbers appearing only once in A381431 (section-sum partition of
 *          prime indices).
 * https://oeis.org/A381434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over v = 1,2,3,...: v is a term iff exactly one multiset
 *            y has section-sum partition equal to the prime indices of v,
 *            i.e. exactly one chain of strict sets D_1 ⊇ D_2 ⊇ ... ⊇ D_E with
 *            sum(D_k) = S_k, where S_1 >= S_2 >= ... >= S_E are the prime
 *            indices of v (counted, stop at 2)
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-10)
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

static uint64_t A381434_S[80];
static int A381434_E;
static int A381434_cnt;
static unsigned char *A381434_used = NULL;
static size_t A381434_usedcap = 0;

static void A381434_level(int k);

static void A381434_pick(int k, uint64_t need, uint64_t maxp)
{
    if (A381434_cnt >= 2) return;
    if (need == 0) { A381434_level(k - 1); return; }
    if (maxp * (maxp + 1) / 2 < need) return;
    uint64_t p = maxp < need ? maxp : need;
    for (; p >= 1; --p) {
        if (A381434_used[p]) continue;
        A381434_used[p] = 1;
        A381434_pick(k, need - p, p - 1);
        A381434_used[p] = 0;
        if (A381434_cnt >= 2) return;
    }
}

/* Build innermost set D_E first, then add new distinct parts outward. */
static void A381434_level(int k)
{
    if (A381434_cnt >= 2) return;
    if (k < 0) { A381434_cnt++; return; }
    uint64_t prev = (k + 1 < A381434_E) ? A381434_S[k + 1] : 0;
    uint64_t need = A381434_S[k] - prev;
    A381434_pick(k, need, A381434_S[0]);
}

static int A381434_cmp_desc(const void *x, const void *y)
{
    uint64_t a1 = *(const uint64_t *)x, b1 = *(const uint64_t *)y;
    return (a1 < b1) - (a1 > b1);
}

static bool A381434_unique(uint64_t v)
{
    if (v == 1) return true;
    uint64_t pr[20], ex[20];
    size_t cnt = 0;
    factor_u64(v, pr, ex, &cnt);
    int E = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t idx = (uint64_t)prime_pi((idx_t)pr[i]);
        for (uint64_t j = 0; j < ex[i]; ++j) A381434_S[E++] = idx;
    }
    qsort(A381434_S, (size_t)E, sizeof(uint64_t), A381434_cmp_desc);
    A381434_E = E;
    size_t need = (size_t)A381434_S[0] + 2;
    if (need > A381434_usedcap) {
        A381434_used = (unsigned char *)realloc(A381434_used, need);
        if (!A381434_used) abort();
        A381434_usedcap = need;
    }
    for (size_t i = 0; i < need; ++i) A381434_used[i] = 0;
    A381434_cnt = 0;
    A381434_level(E - 1);
    return A381434_cnt == 1;
}

seq_t a(idx_t n)
{
    static uint64_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t next = 1;
    while (len < (size_t)n) {
        if (A381434_unique(next)) {
            if (len == cap) {
                cap = cap ? cap * 2 : 64;
                cache = (uint64_t *)realloc(cache, cap * sizeof(uint64_t));
                if (!cache) abort();
            }
            cache[len++] = next;
        }
        ++next;
    }
    return (seq_t)cache[(size_t)n - 1];
}
