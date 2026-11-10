/*
 * A278862  Numbers that are not the sum of 2 or fewer terms from A057890.
 * https://oeis.org/A278862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve. Build every element of A057890 (0, and each odd binary
 *            palindrome times a power of 2), mark all sums s+t of two of them
 *            segment by segment, and list the unmarked n >= 1 in increasing
 *            order
 * Range:     u8 none, u16 none, u32 n <= 262, u64 n <= 262, u128 n <= 262,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 261 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define A278862_CAPBITS 31
#define A278862_CAP ((uint64_t)1 << A278862_CAPBITS)
#define A278862_SEG ((uint64_t)1 << 22)

static uint64_t *A278862_S = NULL;
static size_t A278862_ns = 0;
static uint64_t *A278862_res = NULL;
static size_t A278862_cnt = 0, A278862_capres = 0;
static uint64_t A278862_lo = 0;          /* next segment start */
static unsigned char *A278862_seg = NULL;

static int A278862_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u < v) ? -1 : (u > v);
}

static void A278862_gen(void)
{
    size_t cap = 1024;
    A278862_S = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    A278862_S[A278862_ns++] = 0;
    unsigned k;
    for (k = 1; k <= A278862_CAPBITS; ++k) {
        unsigned hb = (k + 1) / 2, lowb = k / 2, b;
        uint64_t h;
        for (h = (uint64_t)1 << (hb - 1); h < ((uint64_t)1 << hb); ++h) {
            uint64_t p = h, t = (k & 1u) ? (h >> 1) : h, q;
            for (b = 0; b < lowb; ++b) { p = (p << 1) | (t & 1u); t >>= 1; }
            for (q = p; q <= A278862_CAP; q <<= 1) {
                if (A278862_ns >= cap) {
                    cap *= 2;
                    A278862_S = (uint64_t *)realloc(A278862_S, sizeof(uint64_t) * cap);
                }
                A278862_S[A278862_ns++] = q;
            }
        }
    }
    qsort(A278862_S, A278862_ns, sizeof(uint64_t), A278862_cmp);
    A278862_seg = (unsigned char *)malloc((size_t)A278862_SEG);
}

static size_t A278862_lb(uint64_t v)
{
    size_t lo = 0, hi = A278862_ns;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (A278862_S[mid] < v) lo = mid + 1; else hi = mid;
    }
    return lo;
}

static void A278862_segment(void)
{
    uint64_t lo = A278862_lo, hi = lo + A278862_SEG - 1;
    if (hi > A278862_CAP) hi = A278862_CAP;
    size_t len = (size_t)(hi - lo + 1), i;
    memset(A278862_seg, 0, len);
    for (i = 0; i < A278862_ns; ++i) {
        uint64_t s = A278862_S[i];
        if (s > hi) break;
        uint64_t need = (lo > s) ? lo - s : 0;
        if (need < s) need = s;
        size_t j = A278862_lb(need);
        while (j < A278862_ns) {
            uint64_t sum = s + A278862_S[j];
            if (sum > hi) break;
            A278862_seg[sum - lo] = 1;
            ++j;
        }
    }
    for (i = 0; i < len; ++i) {
        uint64_t x = lo + i;
        if (x == 0 || A278862_seg[i]) continue;
        if (A278862_cnt >= A278862_capres) {
            A278862_capres = A278862_capres ? A278862_capres * 2 : 256;
            A278862_res = (uint64_t *)realloc(A278862_res, sizeof(uint64_t) * A278862_capres);
        }
        A278862_res[A278862_cnt++] = x;
    }
    A278862_lo = hi + 1;
}

seq_t a(idx_t n)
{
    if (!A278862_S) A278862_gen();
    while (A278862_cnt < n && A278862_lo <= A278862_CAP)
        A278862_segment();
    if (n < 1 || n > A278862_cnt) return (seq_t)0;
    return (seq_t)A278862_res[n - 1];
}
