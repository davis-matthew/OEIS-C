/*
 * A337159  Lexicographically earliest sequence of distinct nonnegative
 *          integers such that for any n >= 0, A033307(n + a(n)) = A033307(n).
 * https://oeis.org/A337159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   greedy search: a(n) = least unused k >= 0 with A033307(n+k) =
 *            A033307(n), digits of Champernowne's constant (offset 0 = digit
 *            1)
 * Range:     u8 n <= 201, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9968 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *A337159_dig = 0;
static size_t A337159_dlen = 0, A337159_dcap = 0;
static uint64_t A337159_next = 1;

static void A337159_ensure(size_t len) {
    while (A337159_dlen < len) {
        char buf[24];
        int l = 0;
        uint64_t v = A337159_next++;
        while (v) { buf[l++] = (char)(v % 10); v /= 10; }
        if (A337159_dlen + (size_t)l > A337159_dcap) {
            size_t nc = A337159_dcap ? A337159_dcap * 2 : 1024;
            A337159_dig = (unsigned char *)realloc(A337159_dig, nc);
            A337159_dcap = nc;
        }
        while (l > 0) A337159_dig[A337159_dlen++] = (unsigned char)buf[--l];
    }
}

static unsigned char *A337159_used = 0;
static size_t A337159_ucap = 0;
static uint64_t *A337159_res = 0;
static size_t A337159_cnt = 0, A337159_rcap = 0;
static size_t A337159_lo = 0;

static void A337159_useden(size_t k) {
    if (k < A337159_ucap) return;
    size_t nc = A337159_ucap ? A337159_ucap : 1024;
    while (nc <= k) nc *= 2;
    A337159_used = (unsigned char *)realloc(A337159_used, nc);
    memset(A337159_used + A337159_ucap, 0, nc - A337159_ucap);
    A337159_ucap = nc;
}

seq_t a(idx_t n) {
    while (A337159_cnt <= (size_t)n) {
        size_t m = A337159_cnt;
        if (m >= A337159_rcap) {
            size_t nc = A337159_rcap ? A337159_rcap * 2 : 1024;
            A337159_res = (uint64_t *)realloc(A337159_res, nc * sizeof(uint64_t));
            A337159_rcap = nc;
        }
        A337159_ensure(m + 1);
        unsigned char d = A337159_dig[m];
        size_t k = A337159_lo;
        for (;;) {
            A337159_useden(k);
            if (!A337159_used[k]) {
                A337159_ensure(m + k + 1);
                if (A337159_dig[m + k] == d) break;
            }
            k++;
        }
        A337159_used[k] = 1;
        while (1) {
            A337159_useden(A337159_lo);
            if (!A337159_used[A337159_lo]) break;
            A337159_lo++;
        }
        A337159_res[m] = (uint64_t)k;
        A337159_cnt++;
    }
    return (seq_t)A337159_res[n];
}
