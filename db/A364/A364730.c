/*
 * A364730  a(n) is the number of occurrences of n in A364443.
 * https://oeis.org/A364730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k >= 0 : A364443(k) = n}. Computed by a sieve: values
 *            x^2+x*y+y^2 are marked block by block, and the marks in each
 *            interval (k^2, (k+1)^2) are counted. The sieve keeps running
 *            until A364443(k) > n for every k in (k_last, 9*k_last/8 + 500],
 *            where k_last is the largest k with A364443(k) <=...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9883 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A364730_SPAN ((uint64_t)1 << 24)

static uint32_t *A364730_cnt = NULL;
static uint64_t A364730_len = 0;
static uint64_t A364730_cap = 0;
static uint64_t *A364730_bits = NULL;

static uint64_t A364730_isqrt(uint64_t v)
{
    if (v < 2) return v;
    int bl = 64 - __builtin_clzll(v);
    uint64_t r = (uint64_t)1 << ((bl + 1) / 2);
    for (;;) {
        uint64_t y = (r + v / r) >> 1;
        if (y >= r) break;
        r = y;
    }
    while (r * r > v) --r;
    while ((r + 1) * (r + 1) <= v) ++r;
    return r;
}

static uint32_t A364730_popc_range(const uint64_t *b, uint64_t lo, uint64_t hi)
{
    if (lo > hi) return 0;
    uint64_t lw = lo >> 6, hw = hi >> 6;
    unsigned lb = (unsigned)(lo & 63), hb = (unsigned)(hi & 63);
    uint32_t c = 0;
    if (lw == hw) {
        uint64_t m = (hb == 63 ? ~0ULL : ((1ULL << (hb + 1)) - 1)) & (~0ULL << lb);
        return (uint32_t)__builtin_popcountll(b[lw] & m);
    }
    c += (uint32_t)__builtin_popcountll(b[lw] & (~0ULL << lb));
    for (uint64_t w = lw + 1; w < hw; ++w) c += (uint32_t)__builtin_popcountll(b[w]);
    c += (uint32_t)__builtin_popcountll(b[hw] & (hb == 63 ? ~0ULL : ((1ULL << (hb + 1)) - 1)));
    return c;
}

static void A364730_extend(uint64_t newlen)
{
    if (newlen <= A364730_len) return;
    if (newlen > A364730_cap) {
        uint64_t nc = newlen + newlen / 2 + 64;
        uint32_t *p = (uint32_t *)realloc(A364730_cnt, (size_t)nc * sizeof(uint32_t));
        if (!p) abort();
        A364730_cnt = p;
        A364730_cap = nc;
    }
    if (!A364730_bits) {
        A364730_bits = (uint64_t *)malloc((size_t)((A364730_SPAN >> 6) + 4) * sizeof(uint64_t));
        if (!A364730_bits) abort();
    }
    uint64_t *bits = A364730_bits;
    uint64_t k0 = A364730_len;
    while (k0 < newlen) {
        uint64_t k1 = k0;
        while (k1 + 1 < newlen && (k1 + 2) * (k1 + 2) - k0 * k0 <= A364730_SPAN) ++k1;
        uint64_t L = k0 * k0 + 1;
        uint64_t R = (k1 + 1) * (k1 + 1) - 1;
        if (R >= L) {
            uint64_t size = R - L + 1;
            uint64_t words = (size >> 6) + 1;
            memset(bits, 0, (size_t)words * sizeof(uint64_t));
            uint64_t xlo = A364730_isqrt(L / 3);
            uint64_t xhi = A364730_isqrt(R);
            for (uint64_t x = xlo; x <= xhi; ++x) {
                uint64_t x2 = x * x;
                uint64_t y = 0;
                if (x2 < L) {
                    uint64_t D = 4 * L - 3 * x2;
                    uint64_t s = A364730_isqrt(D);
                    y = (s > x) ? (s - x) / 2 : 0;
                    while (y > 0 && x2 + x * (y - 1) + (y - 1) * (y - 1) >= L) --y;
                    while (y <= x && x2 + x * y + y * y < L) ++y;
                }
                if (y > x) continue;
                uint64_t v = x2 + x * y + y * y;
                while (y <= x && v <= R) {
                    uint64_t o = v - L;
                    bits[o >> 6] |= 1ULL << (o & 63);
                    v += x + 2 * y + 1;
                    ++y;
                }
            }
        }
        for (uint64_t k = k0; k <= k1; ++k) {
            if (k == 0) { A364730_cnt[k] = 0; continue; }
            uint64_t lo = k * k + 1 - L, hi = k * k + 2 * k - L;
            A364730_cnt[k] = A364730_popc_range(bits, lo, hi);
        }
        k0 = k1 + 1;
    }
    A364730_len = newlen;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    if (A364730_len == 0) A364730_extend(1);
    for (;;) {
        uint64_t kl = 0;
        for (uint64_t k = A364730_len; k-- > 0;) {
            if ((uint64_t)A364730_cnt[k] <= nn) { kl = k; break; }
        }
        uint64_t target = kl + kl / 8 + 500;
        if (A364730_len > target) break;
        A364730_extend(target + 1);
    }
    seq_t r = 0;
    for (uint64_t k = 0; k < A364730_len; ++k)
        if ((uint64_t)A364730_cnt[k] == nn) ++r;
    return r;
}
