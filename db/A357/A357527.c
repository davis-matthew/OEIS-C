/*
 * A357527  Reverse run lengths in binary expansions of terms of A044813: for
 *          n > 0, a(n) is the unique k such that A044813(k) =
 *          A056539(A044813(n)); a(0) = 0.
 * https://oeis.org/A357527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = k such that A044813(k) = A056539(A044813(n)); computed by
 *            enumerating numbers of each bit length with distinct run lengths
 *            (compositions into distinct parts), sorting, and ranking. a(0) =
 *            0
 * Range:     u8 n <= 205, u16 n <= 11666, u32 n <= 11666, u64 n <= 11666,
 *            u128 n <= 11666, big any n
 * Validated: standard, pass, 84 terms through n = 9914 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A357527_grp[64];
static size_t A357527_cnt[64];
static size_t A357527_cap[64];
static bool A357527_built[64];

static void A357527_push(int L, uint64_t v)
{
    if (A357527_cnt[L] == A357527_cap[L]) {
        size_t nc = A357527_cap[L] ? A357527_cap[L] * 2 : 16;
        A357527_grp[L] = (uint64_t *)realloc(A357527_grp[L], nc * sizeof(uint64_t));
        A357527_cap[L] = nc;
    }
    A357527_grp[L][A357527_cnt[L]++] = v;
}

static void A357527_dfs(int L, int rem, uint64_t mask, uint64_t val, int bit)
{
    if (rem == 0) {
        A357527_push(L, val);
        return;
    }
    for (int r = 1; r <= rem; r++) {
        if ((mask >> r) & 1)
            continue;
        uint64_t nv = (val << r) | (bit ? ((((uint64_t)1) << r) - 1) : 0);
        A357527_dfs(L, rem - r, mask | (((uint64_t)1) << r), nv, !bit);
    }
}

static int A357527_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a < b) ? -1 : (a > b);
}

static void A357527_build(int L)
{
    if (A357527_built[L])
        return;
    A357527_dfs(L, L, 0, 0, 1);
    if (A357527_cnt[L] > 1)
        qsort(A357527_grp[L], A357527_cnt[L], sizeof(uint64_t), A357527_cmp);
    A357527_built[L] = true;
}

static uint64_t A357527_A056539(uint64_t m)
{
    if (m == 0)
        return 0;
    int bitlen = 64 - __builtin_clzll(m);
    int runs = 1;
    int prev = (int)((m >> (bitlen - 1)) & 1);
    for (int i = bitlen - 2; i >= 0; --i) {
        int b = (int)((m >> i) & 1);
        if (b != prev) {
            runs++;
            prev = b;
        }
    }
    uint64_t rev = 0;
    for (int i = 0; i < bitlen; ++i)
        rev = (rev << 1) | ((m >> i) & 1);
    if ((runs & 1) == 0) {
        uint64_t mask = (bitlen == 64) ? ~(uint64_t)0 : (((uint64_t)1 << bitlen) - 1);
        rev ^= mask;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    uint64_t off = (uint64_t)n - 1;
    int L = 1;
    uint64_t base = 0;
    for (;; L++) {
        A357527_build(L);
        if (off < A357527_cnt[L])
            break;
        off -= A357527_cnt[L];
        base += A357527_cnt[L];
    }
    uint64_t v = A357527_grp[L][off];
    uint64_t w = A357527_A056539(v);
    size_t lo = 0, hi = A357527_cnt[L];
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (A357527_grp[L][mid] < w)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)(base + lo + 1);
}
