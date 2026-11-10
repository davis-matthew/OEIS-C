/*
 * A309203  Numbers k such that A111273(k-1) = k.
 * https://oeis.org/A309203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A111273(k-1) = k; computed by simulating
 *            A111273 (smallest unused divisor of T(m)) and collecting matches
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9978 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static unsigned char *A309203_seen = NULL;
static size_t A309203_seen_cap = 0;
static uint64_t A309203_cur = 0;
static uint64_t *A309203_found = NULL;
static size_t A309203_found_len = 0, A309203_found_cap = 0;

static bool A309203_is_seen(uint64_t d)
{
    return d < A309203_seen_cap && A309203_seen[d];
}

static void A309203_mark(uint64_t d)
{
    if (d >= A309203_seen_cap) {
        size_t nc = A309203_seen_cap ? A309203_seen_cap : 1024;
        while (nc <= d) nc *= 2;
        A309203_seen = (unsigned char *)realloc(A309203_seen, nc);
        memset(A309203_seen + A309203_seen_cap, 0, nc - A309203_seen_cap);
        A309203_seen_cap = nc;
    }
    A309203_seen[d] = 1;
}

static uint64_t A309203_step(void)
{
    uint64_t idx = ++A309203_cur;
    uint64_t T = idx * (idx + 1) / 2;
    uint64_t ans = 0, d, s = 1;
    for (d = 1; d * d <= T; ++d) {
        s = d;
        if (T % d == 0 && !A309203_is_seen(d)) { ans = d; break; }
    }
    if (!ans) {
        for (d = s; d >= 1; --d) {
            if (T % d == 0 && !A309203_is_seen(T / d)) { ans = T / d; break; }
        }
    }
    A309203_mark(ans);
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A309203_found_len < (size_t)n) {
        uint64_t v = A309203_step();
        if (v == A309203_cur + 1) {
            if (A309203_found_len == A309203_found_cap) {
                A309203_found_cap = A309203_found_cap ? A309203_found_cap * 2 : 64;
                A309203_found = (uint64_t *)realloc(A309203_found, A309203_found_cap * sizeof(uint64_t));
            }
            A309203_found[A309203_found_len++] = v;
        }
    }
    return (seq_t)A309203_found[(size_t)n - 1];
}
