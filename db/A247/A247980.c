/*
 * A247980  Least prime whose square contributes to A234429.
 * https://oeis.org/A247980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest prime p such that no smaller prime q has
 *            digitsum(q^2) = digitsum(p^2), i.e. the least primes for each
 *            digit sum in A234429, listed in increasing order. Computed by a
 *            search over x = A*10^5 + b: x^2 = Q*10^5 + (b^2 mod 10^5), with
 *            Q = A^2*10^5 + 2*A*b + floor(b^2/10^5). For e...
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 51, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

#define A247980_B     100000u
#define A247980_NDL   46
#define A247980_MAXS  400
#define A247980_MAXT  1024

static bool     A247980_inited = false;
static uint8_t  A247980_ds4[10000];
static uint8_t  A247980_lowds[A247980_B];
static uint32_t A247980_hb[A247980_B];
static uint32_t A247980_bk[A247980_B];
static uint32_t A247980_bstart[3 * A247980_NDL + 1];

static bool     A247980_wanted[A247980_MAXS];
static int      A247980_pre[A247980_MAXS + 1];

static uint64_t A247980_terms[A247980_MAXT];
static size_t   A247980_cnt = 0;
static uint64_t A247980_A = 0;

static uint64_t A247980_cx[A247980_B];
static unsigned A247980_cs[A247980_B];

static unsigned A247980_dsum(uint64_t v)
{
    unsigned s = 0;
    while (v) {
        s += A247980_ds4[v % 10000u];
        v /= 10000u;
    }
    return s;
}

static void A247980_rebuild_pre(void)
{
    int i;
    A247980_pre[0] = 0;
    for (i = 0; i < A247980_MAXS; i++)
        A247980_pre[i + 1] = A247980_pre[i] + (A247980_wanted[i] ? 1 : 0);
}

static bool A247980_any(unsigned lo, unsigned hi)
{
    if (lo >= A247980_MAXS) return false;
    if (hi >= A247980_MAXS) hi = A247980_MAXS - 1;
    if (hi < lo) return false;
    return A247980_pre[hi + 1] - A247980_pre[lo] > 0;
}

static void A247980_init(void)
{
    uint32_t i, b;
    uint32_t cnt[3 * A247980_NDL];
    uint32_t pos[3 * A247980_NDL];

    A247980_ds4[0] = 0;
    for (i = 1; i < 10000; i++)
        A247980_ds4[i] = (uint8_t)(A247980_ds4[i / 10] + i % 10);

    for (i = 0; i < 3 * A247980_NDL; i++) cnt[i] = 0;
    for (b = 0; b < A247980_B; b++) {
        uint64_t sq = (uint64_t)b * b;
        A247980_lowds[b] = (uint8_t)A247980_dsum(sq % A247980_B);
        A247980_hb[b] = (uint32_t)(sq / A247980_B);
        {
            uint32_t u = b % 10;
            if (u == 1 || u == 3 || u == 7 || u == 9)
                cnt[(b % 3) * A247980_NDL + A247980_lowds[b]]++;
        }
    }
    A247980_bstart[0] = 0;
    for (i = 0; i < 3 * A247980_NDL; i++) {
        A247980_bstart[i + 1] = A247980_bstart[i] + cnt[i];
        pos[i] = A247980_bstart[i];
    }
    for (b = 0; b < A247980_B; b++) {
        uint32_t u = b % 10;
        if (u == 1 || u == 3 || u == 7 || u == 9) {
            uint32_t g = (b % 3) * A247980_NDL + A247980_lowds[b];
            A247980_bk[pos[g]++] = b;
        }
    }

    for (i = 0; i < A247980_MAXS; i++)
        A247980_wanted[i] = (i >= 4 && i % 3 == 1) || i == 9;
    A247980_rebuild_pre();
    A247980_inited = true;
}

static unsigned A247980_maxds(uint64_t lo, uint64_t hi)
{
    unsigned best = A247980_dsum(hi);
    uint64_t p = 1;
    while (p <= hi) {
        uint64_t d = (hi / p) % 10;
        if (d > 0) {
            uint64_t c = (hi / (p * 10)) * (p * 10) + (d - 1) * p + (p - 1);
            if (c >= lo) {
                unsigned t = A247980_dsum(c);
                if (t > best) best = t;
            }
        }
        if (p > hi / 10) break;
        p *= 10;
    }
    return best;
}

static unsigned A247980_minds(uint64_t lo, uint64_t hi)
{
    unsigned best = A247980_dsum(lo);
    uint64_t p = 10;
    while (p <= hi) {
        uint64_t c = (lo / p + 1) * p;
        if (c <= hi) {
            unsigned t = A247980_dsum(c);
            if (t < best) best = t;
        }
        if (p > hi / 10) break;
        p *= 10;
    }
    return best;
}

static void A247980_record(uint64_t x, unsigned s)
{
    A247980_wanted[s] = false;
    if (A247980_cnt < A247980_MAXT) A247980_terms[A247980_cnt++] = x;
    A247980_rebuild_pre();
}

static void A247980_block0(void)
{
    uint64_t x;
    for (x = 2; x < A247980_B; x++) {
        unsigned s = A247980_dsum(x * x);
        if (s < A247980_MAXS && A247980_wanted[s] && is_prime_u64(x))
            A247980_record(x, s);
    }
}

static void A247980_block(uint64_t A)
{
    uint64_t qlo = A * A * A247980_B;
    uint64_t qhi = (A + 1) * (A + 1) * A247980_B - 1;
    unsigned mx = A247980_maxds(qlo, qhi);
    unsigned mn = A247980_minds(qlo, qhi);
    unsigned a3 = (unsigned)(A % 3);
    uint64_t twoA = 2 * A;
    uint64_t base = A * A247980_B;
    size_t nc = 0, i, j;
    unsigned c, d;

    if (!A247980_any(mn, mx + (A247980_NDL - 1))) return;

    for (c = 0; c < 3; c++) {
        if ((a3 + c) % 3 == 0) continue;
        for (d = 0; d < A247980_NDL; d++) {
            uint32_t g = c * A247980_NDL + d;
            uint32_t k, e;
            if (A247980_bstart[g] == A247980_bstart[g + 1]) continue;
            if (!A247980_any(mn + d, mx + d)) continue;
            e = A247980_bstart[g + 1];
            for (k = A247980_bstart[g]; k < e; k++) {
                uint32_t b = A247980_bk[k];
                uint64_t q = qlo + twoA * b + A247980_hb[b];
                unsigned s = A247980_dsum(q) + d;
                if (s < A247980_MAXS && A247980_wanted[s]) {
                    uint64_t x = base + b;
                    if (is_prime_u64(x)) {
                        A247980_cx[nc] = x;
                        A247980_cs[nc] = s;
                        nc++;
                    }
                }
            }
        }
    }
    /* insertion sort candidates by x */
    for (i = 1; i < nc; i++) {
        uint64_t kx = A247980_cx[i];
        unsigned ks = A247980_cs[i];
        j = i;
        while (j > 0 && A247980_cx[j - 1] > kx) {
            A247980_cx[j] = A247980_cx[j - 1];
            A247980_cs[j] = A247980_cs[j - 1];
            j--;
        }
        A247980_cx[j] = kx;
        A247980_cs[j] = ks;
    }
    for (i = 0; i < nc; i++)
        if (A247980_wanted[A247980_cs[i]])
            A247980_record(A247980_cx[i], A247980_cs[i]);
}

seq_t a(idx_t n)
{
    if (n < 1 || n > A247980_MAXT) return (seq_t)0;
    if (!A247980_inited) A247980_init();
    while (A247980_cnt < n) {
        if (A247980_A == 0) A247980_block0();
        else A247980_block(A247980_A);
        A247980_A++;
    }
    return (seq_t)A247980_terms[n - 1];
}
