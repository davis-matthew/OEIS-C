/*
 * A364732  a(n) is the index of the last occurrence of n in A364443, or -1 if
 *          n doesn't occur.
 * https://oeis.org/A364732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = last m with A364443(m) = n, else -1. Computed with a
 *            segmented sieve that marks every Loeschian number k = a^2 +
 *            3*b^2 and counts the marks inside each interval (m^2, (m+1)^2).
 *            The search keeps extending until a long window of counts lies
 *            well above n
 * Range:     i8 n <= 49, i16 n <= 9184, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8350 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A364732_SEGBITS (1u << 21)
#define A364732_WORDS (A364732_SEGBITS >> 6)
#define A364732_WIN 2000u

static uint64_t A364732_bits[A364732_WORDS];
static uint32_t *A364732_cnt = NULL;   /* A364443(m) for m < done */
static size_t A364732_ccap = 0;
static uint64_t A364732_done = 0;      /* number of m fully counted */
static uint64_t A364732_L = 0;         /* start of next segment */
static uint64_t *A364732_na = NULL;    /* next a for each b */
static size_t A364732_nacap = 0;
static uint64_t A364732_nb = 0;        /* number of active b */

static void A364732_ensure_cnt(size_t need)
{
    if (need <= A364732_ccap) return;
    size_t nc = A364732_ccap ? A364732_ccap : 1024;
    while (nc < need) nc *= 2;
    A364732_cnt = (uint32_t *)realloc(A364732_cnt, nc * sizeof(uint32_t));
    if (!A364732_cnt) abort();
    memset(A364732_cnt + A364732_ccap, 0, (nc - A364732_ccap) * sizeof(uint32_t));
    A364732_ccap = nc;
}

static uint64_t A364732_range(uint64_t x, uint64_t y)
{
    if (x >= y) return 0;
    uint64_t wx = x >> 6, wy = (y - 1) >> 6;
    uint64_t lm = ~0ULL << (x & 63);
    uint64_t hm = ~0ULL >> (63 - ((y - 1) & 63));
    if (wx == wy) return (uint64_t)__builtin_popcountll(A364732_bits[wx] & lm & hm);
    uint64_t c = (uint64_t)__builtin_popcountll(A364732_bits[wx] & lm);
    for (uint64_t w = wx + 1; w < wy; ++w) c += (uint64_t)__builtin_popcountll(A364732_bits[w]);
    c += (uint64_t)__builtin_popcountll(A364732_bits[wy] & hm);
    return c;
}

static void A364732_mark(uint64_t b, uint64_t lo, uint64_t hi)
{
    uint64_t a = A364732_na[b];
    uint64_t v = a * a + 3 * b * b;
    while (v < hi) {
        uint64_t off = v - lo;
        A364732_bits[off >> 6] |= 1ULL << (off & 63);
        v += 2 * a + 1;
        ++a;
    }
    A364732_na[b] = a;
}

static void A364732_extend(void)
{
    uint64_t lo = A364732_L, hi = lo + A364732_SEGBITS;
    memset(A364732_bits, 0, sizeof(A364732_bits));
    for (uint64_t b = 0; b < A364732_nb; ++b) A364732_mark(b, lo, hi);
    while (3 * A364732_nb * A364732_nb < hi) {
        if ((size_t)A364732_nb >= A364732_nacap) {
            size_t nc = A364732_nacap ? A364732_nacap * 2 : 1024;
            A364732_na = (uint64_t *)realloc(A364732_na, nc * sizeof(uint64_t));
            if (!A364732_na) abort();
            A364732_nacap = nc;
        }
        A364732_na[A364732_nb] = 0;
        A364732_mark(A364732_nb, lo, hi);
        ++A364732_nb;
    }
    for (uint64_t m = A364732_done;; ++m) {
        uint64_t s = m * m + 1, e = (m + 1) * (m + 1);
        A364732_ensure_cnt((size_t)m + 2);
        if (s >= hi) break;
        uint64_t x = s > lo ? s : lo;
        uint64_t y = e < hi ? e : hi;
        if (x < y) A364732_cnt[m] += (uint32_t)A364732_range(x - lo, y - lo);
        if (e <= hi) A364732_done = m + 1;
        else break;
    }
    A364732_L = hi;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    for (;;) {
        if (A364732_done > A364732_WIN) {
            uint64_t mn = UINT64_MAX, mx = 0;
            for (uint64_t i = A364732_done - A364732_WIN; i < A364732_done; ++i) {
                uint64_t c = A364732_cnt[i];
                if (c < mn) mn = c;
                if (c > mx) mx = c;
            }
            if (mn > nn && 2 * (mn - nn) >= mx - mn) break;
        }
        A364732_extend();
    }
    for (uint64_t m = A364732_done; m > 0; --m)
        if ((uint64_t)A364732_cnt[m - 1] == nn) return (seq_t)(m - 1);
    return (seq_t)(-1);
}
