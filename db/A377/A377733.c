/*
 * A377733  Numbers k such that k and k+1 are both terms in A377732.
 * https://oeis.org/A377733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   no closed form; a(n) is found by search. If k is in A377732, it
 *            can be written k = d*(s^2-d), where d = max{d|k, d<=sqrt(k)} and
 *            s^2 = d + k/d. Every value d*(s^2-d) with s^2 >= 2d is
 *            enumerated into windowed bitmaps. For each adjacent pair k, k+1
 *            found this way, both are checked exactly against...
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 3510, u64 n <= 5242,
 *            u128 n <= 5242, big any n
 * Validated: standard, pass, 58 terms through n = 4535 (2026-11-10)
 * Helpers:   ctz_u64, factor_u64, is_square_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/isqrt_u64.h"
#include "../utils/is_square_u64.h"
#include "../utils/ctz_u64.h"

#define A377733_WBITS ((uint64_t)1 << 24)
#define A377733_WWORDS (A377733_WBITS / 64)

static uint64_t *A377733_terms = NULL;
static size_t A377733_len = 0, A377733_cap = 0;
static uint64_t A377733_L = 1;          /* start of next window */
static uint64_t *A377733_s = NULL;      /* next s for each d (index d) */
static size_t A377733_nd = 1;           /* d values 1..nd-1 active */
static size_t A377733_scap = 0;
static uint64_t *A377733_bits = NULL;
static bool A377733_prevbit = false;    /* was L-1 a candidate */

/* Largest divisor of factorised number not exceeding limit. */
static void A377733_best_rec(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                             size_t idx, uint64_t cur, uint64_t limit,
                             uint64_t *best)
{
    if (idx == cnt) {
        if (cur <= limit && cur > *best) *best = cur;
        return;
    }
    uint64_t p = pr[idx], e = ex[idx], val = cur;
    for (uint64_t i = 0; i <= e; ++i) {
        A377733_best_rec(pr, ex, cnt, idx + 1, val, limit, best);
        if (i == e) break;
        if (val > limit / p) break;
        val *= p;
    }
}

/* Exact A377732 membership test. */
static bool A377733_good(uint64_t k)
{
    if (k == 0) return false;
    uint64_t r = isqrt_u64(k);
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(k, pr, ex, &cnt);
    uint64_t best = 1;
    A377733_best_rec(pr, ex, cnt, 0, 1, r, &best);
    return is_square_u64(best + k / best);
}

static void A377733_push(uint64_t v)
{
    if (A377733_len == A377733_cap) {
        size_t nc = A377733_cap ? A377733_cap * 2 : 256;
        A377733_terms = (uint64_t *)realloc(A377733_terms, nc * sizeof(uint64_t));
        A377733_cap = nc;
    }
    A377733_terms[A377733_len++] = v;
}

static void A377733_check_pair(uint64_t k)
{
    if (A377733_good(k) && A377733_good(k + 1)) A377733_push(k);
}

static void A377733_window(void)
{
    uint64_t L = A377733_L;
    uint64_t H = L + A377733_WBITS;
    if (!A377733_bits)
        A377733_bits = (uint64_t *)malloc((size_t)A377733_WWORDS * sizeof(uint64_t));
    memset(A377733_bits, 0, (size_t)A377733_WWORDS * sizeof(uint64_t));

    /* activate new d with d*d < H */
    while ((uint64_t)A377733_nd * (uint64_t)A377733_nd < H) {
        if (A377733_nd >= A377733_scap) {
            size_t nc = A377733_scap ? A377733_scap * 2 : 4096;
            A377733_s = (uint64_t *)realloc(A377733_s, nc * sizeof(uint64_t));
            A377733_scap = nc;
        }
        uint64_t d = (uint64_t)A377733_nd;
        uint64_t s0 = isqrt_u64(2 * d);
        if (s0 * s0 < 2 * d) ++s0;
        A377733_s[A377733_nd] = s0;
        ++A377733_nd;
    }

    for (size_t di = 1; di < A377733_nd; ++di) {
        uint64_t d = (uint64_t)di;
        uint64_t s = A377733_s[di];
        for (;;) {
            uint64_t k = d * (s * s - d);
            if (k >= H) break;
            if (k >= L) {
                uint64_t off = k - L;
                A377733_bits[off >> 6] |= (uint64_t)1 << (off & 63);
            }
            ++s;
        }
        A377733_s[di] = s;
    }

    /* cross-window pair (L-1, L) */
    if (A377733_prevbit && (A377733_bits[0] & 1)) A377733_check_pair(L - 1);

    for (uint64_t j = 0; j < A377733_WWORDS; ++j) {
        uint64_t w = A377733_bits[j];
        if (!w) continue;
        uint64_t nxt = (j + 1 < A377733_WWORDS) ? (A377733_bits[j + 1] & 1) : 0;
        uint64_t m = w & ((w >> 1) | (nxt << 63));
        while (m) {
            int b = ctz_u64(m);
            m &= m - 1;
            A377733_check_pair(L + 64 * j + (uint64_t)b);
        }
    }

    A377733_prevbit = (A377733_bits[A377733_WWORDS - 1] >> 63) & 1;
    A377733_L = H;
}

seq_t a(idx_t n)
{
    while (A377733_len < (size_t)n)
        A377733_window();
    return (seq_t)A377733_terms[(size_t)n - 1];
}
