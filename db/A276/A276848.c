/*
 * A276848  For a lesser p of twin primes, let B_(p+2) and B_p be sequences
 *          defined as A159559, but with initial terms p+2 and p respectively.
 *          The sequence lists p for which all differences
 *          B_(p+2)(n)-B_p(n)<=6.
 * https://oeis.org/A276848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: twin-prime lesser members p (found with a segmented
 *            sieve) for which the walks B_p(n), B_(p+2)(n) of A159559 (m
 *            prime iff n prime), started at p and p+2, keep B_(p+2)(n)-B_p(n)
 *            <= 6 until they merge. Once they merge they stay equal. Because
 *            the next-term step is monotone, B_p(n) <= B_(p+2...
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 8371 (2026-11-08)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"

#define A276848_CAP 5000
#define A276848_SEG ((uint64_t)1 << 20)
#define A276848_MARGIN ((uint64_t)1 << 16)

static unsigned char *A276848_W = NULL;
static uint64_t A276848_wlo = 0, A276848_whi = 0;
static size_t A276848_wcap = 0;

static uint32_t *A276848_bp = NULL;
static size_t A276848_bpn = 0;
static uint64_t A276848_bplim = 0;

static unsigned char A276848_kp[A276848_CAP + 1];
static bool A276848_kp_init = false;

static void A276848_ensure_base(uint64_t lim)
{
    if (lim <= A276848_bplim) return;
    uint64_t nl = A276848_bplim * 2;
    if (nl < lim) nl = lim;
    if (nl < 65536) nl = 65536;
    unsigned char *s = (unsigned char *)malloc((size_t)nl + 1);
    memset(s, 1, (size_t)nl + 1);
    s[0] = 0; s[1] = 0;
    for (uint64_t i = 2; i * i <= nl; ++i)
        if (s[i])
            for (uint64_t j = i * i; j <= nl; j += i) s[j] = 0;
    size_t cnt = 0;
    for (uint64_t i = 3; i <= nl; i += 2) if (s[i]) cnt++;
    free(A276848_bp);
    A276848_bp = (uint32_t *)malloc((cnt + 1) * sizeof(uint32_t));
    A276848_bpn = 0;
    for (uint64_t i = 3; i <= nl; i += 2)
        if (s[i]) A276848_bp[A276848_bpn++] = (uint32_t)i;
    free(s);
    A276848_bplim = nl;
}

static void A276848_sieve_window(uint64_t lo, uint64_t hi)
{
    size_t len = (size_t)(hi - lo);
    if (len > A276848_wcap) {
        free(A276848_W);
        A276848_W = (unsigned char *)malloc(len);
        A276848_wcap = len;
    }
    unsigned char *W = A276848_W;
    for (size_t i = 0; i < len; ++i) W[i] = (unsigned char)((lo + i) & 1);
    if (lo <= 2 && 2 < hi) W[2 - lo] = 1;
    if (lo <= 1 && 1 < hi) W[1 - lo] = 0;
    if (lo == 0) W[0] = 0;
    uint64_t r = isqrt_u64(hi) + 1;
    A276848_ensure_base(r);
    for (size_t i = 0; i < A276848_bpn; ++i) {
        uint64_t q = A276848_bp[i];
        if (q * q >= hi) break;
        uint64_t s = q * q;
        if (s < lo) {
            s = ((lo + q - 1) / q) * q;
            if ((s & 1) == 0) s += q;
        }
        for (; s < hi; s += 2 * q) W[s - lo] = 0;
    }
    A276848_wlo = lo;
    A276848_whi = hi;
}

static inline bool A276848_isp(uint64_t v)
{
    if (v >= A276848_wlo && v < A276848_whi) return A276848_W[v - A276848_wlo] != 0;
    return is_prime_u64(v);
}

static void A276848_init_kp(void)
{
    if (A276848_kp_init) return;
    memset(A276848_kp, 1, sizeof(A276848_kp));
    A276848_kp[0] = 0; A276848_kp[1] = 0;
    for (int i = 2; i * i <= A276848_CAP; ++i)
        if (A276848_kp[i])
            for (int j = i * i; j <= A276848_CAP; j += i) A276848_kp[j] = 0;
    A276848_kp_init = true;
}

static bool A276848_good(uint64_t p)
{
    uint64_t x = p, y = p + 2;
    for (int k = 3; k <= A276848_CAP; ++k) {
        bool pk = A276848_kp[k] != 0;
        x++;
        while (A276848_isp(x) != pk) x++;
        y++;
        while (A276848_isp(y) != pk) y++;
        if (x == y) return true;
        if (y < x || y - x > 6) return false;
    }
    return true;
}

static uint64_t *A276848_terms = NULL;
static idx_t A276848_cnt = 0;
static idx_t A276848_cap = 0;
static uint64_t A276848_lo = 0; /* next segment start */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    A276848_init_kp();
    while (A276848_cnt < n) {
        uint64_t lo = A276848_lo;
        uint64_t top = lo + A276848_SEG;
        A276848_sieve_window(lo, top + A276848_MARGIN);
        uint64_t p = lo < 3 ? 3 : (lo | 1);
        for (; p < top; p += 2) {
            if (!A276848_W[p - lo] || !A276848_W[p + 2 - lo]) continue;
            if (!A276848_good(p)) continue;
            if (A276848_cnt >= A276848_cap) {
                idx_t nc = A276848_cap ? A276848_cap * 2 : 64;
                A276848_terms = (uint64_t *)realloc(A276848_terms, (size_t)nc * sizeof(uint64_t));
                A276848_cap = nc;
            }
            A276848_terms[A276848_cnt++] = p;
        }
        A276848_lo = top;
    }
    return (seq_t)A276848_terms[n - 1];
}
