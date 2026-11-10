/*
 * A295793  a(n) is the least k such that A295520(k) = n.
 * https://oeis.org/A295793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = min{k >= 0 : A295520(k) = n}. Computed by a search: for
 *            M/2 <= n < M (M a power of 2), the block of size M holding k
 *            must contain an aligned half-block with no primes. A segmented
 *            sieve walks the prime gaps and checks only the size-M blocks
 *            that have such a half inside a gap
 * Range:     u8 n <= 11, u16 n <= 63, u32 n <= 175, u64 n <= 175,
 *            u128 n <= 175, big any n
 * Validated: standard, pass, 68 terms through n = 151 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

#define A295793_CAP 65536
#define A295793_SEG 65536
#define A295793_BPLIM (1u << 20)

static uint64_t A295793_first[A295793_CAP];
static bool A295793_found[A295793_CAP];
static uint64_t A295793_cnt[64];
static bool A295793_done[64];
static uint64_t A295793_lastB1[64];
static uint32_t *A295793_bp = NULL;
static size_t A295793_bpc = 0;
static uint64_t A295793_lo = 3;
static uint64_t A295793_prev = 2;
static unsigned char A295793_seg[A295793_SEG];

static void A295793_init(void)
{
    unsigned char *c = (unsigned char *)calloc(A295793_BPLIM + 1, 1);
    size_t cnt = 0;
    for (uint32_t i = 2; i <= A295793_BPLIM; i++) {
        if (!c[i]) {
            cnt++;
            for (uint64_t j = (uint64_t)i * i; j <= A295793_BPLIM; j += i) c[j] = 1;
        }
    }
    A295793_bp = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    A295793_bpc = 0;
    for (uint32_t i = 3; i <= A295793_BPLIM; i++)
        if (!c[i]) A295793_bp[A295793_bpc++] = i;
    free(c);
}

static void A295793_check_block(uint64_t b0, unsigned m)
{
    uint64_t M = (uint64_t)1 << m, H = M >> 1;
    unsigned char *isp = (unsigned char *)malloc((size_t)M);
    for (uint64_t r = 0; r < M; r++) isp[r] = is_prime_u64(b0 + r) ? 1 : 0;
    for (uint64_t r = 0; r < M; r++) {
        uint64_t j = 0;
        while (j < M && !isp[r ^ j]) j++;
        if (j >= H && j < M && j < A295793_CAP && !A295793_found[j]) {
            A295793_found[j] = true;
            A295793_first[j] = b0 + r;
            A295793_cnt[m]++;
            if (A295793_cnt[m] == H) A295793_done[m] = true;
        }
    }
    free(isp);
}

static void A295793_gap(uint64_t p, uint64_t q)
{
    uint64_t g = q - p - 1;
    for (unsigned m = 4; m < 63; m++) {
        uint64_t H = (uint64_t)1 << (m - 1);
        if (H > g) break;
        if (((uint64_t)1 << m) > A295793_CAP) break;
        if (A295793_done[m]) continue;
        uint64_t s = ((p + 1 + H - 1) / H) * H;
        while (s + H - 1 < q) {
            uint64_t B = s >> m;
            if (B + 1 > A295793_lastB1[m]) {
                A295793_lastB1[m] = B + 1;
                A295793_check_block(B << m, m);
            }
            s += H;
        }
    }
}

static void A295793_segment(void)
{
    uint64_t lo = A295793_lo, hi = lo + 2 * (uint64_t)A295793_SEG;
    memset(A295793_seg, 0, sizeof(A295793_seg));
    for (size_t i = 0; i < A295793_bpc; i++) {
        uint64_t p = A295793_bp[i];
        if (p * p >= hi) break;
        uint64_t st = p * p;
        if (st < lo) {
            st = ((lo + p - 1) / p) * p;
            if ((st & 1) == 0) st += p;
        }
        for (uint64_t x = st; x < hi; x += 2 * p) A295793_seg[(x - lo) >> 1] = 1;
    }
    for (uint64_t i = 0; i < A295793_SEG; i++) {
        if (!A295793_seg[i]) {
            uint64_t x = lo + 2 * i;
            A295793_gap(A295793_prev, x);
            A295793_prev = x;
        }
    }
    A295793_lo = hi;
}

static uint64_t A295793_f(uint64_t k)
{
    for (uint64_t j = 0; ; ++j)
        if (is_prime_u64(k ^ j)) return j;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N < 8) {
        for (uint64_t k = 0; ; ++k)
            if (A295793_f(k) == N) return (seq_t)k;
    }
    if (N >= A295793_CAP) {
        for (uint64_t k = 0; ; ++k)
            if (A295793_f(k) == N) return (seq_t)k;
    }
    if (!A295793_bp) A295793_init();
    while (!A295793_found[N]) A295793_segment();
    return (seq_t)A295793_first[N];
}
