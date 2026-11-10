/*
 * A308895  Positions of 5's on the spiral defined in A308890.
 * https://oeis.org/A308895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index m of the n-th 5 in A308890, found by computing
 *            A308890 along the spiral (a(m) = mex of earlier knight-move
 *            neighbours)
 * Range:     u8 n <= 3, u16 n <= 144, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 998 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint8_t *A308895_vals = NULL;     /* vals[m-1] = A308890(m) */
static uint64_t *A308895_pos = NULL;     /* positions of 5's */
static size_t A308895_cap = 0, A308895_len = 0, A308895_npos = 0, A308895_pcap = 0;

static uint64_t A308895_isqrt(uint64_t n)
{
    if (n <= 1) return n;
    uint64_t lo = 0, hi = n < 4294967296ULL ? n : 4294967296ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= n) lo = mid; else hi = mid - 1;
    }
    return lo;
}

static uint64_t A308895_xy_to_index(int64_t x, int64_t y)
{
    int64_t ax = x >= 0 ? x : -x;
    int64_t ay = y >= 0 ? y : -y;
    int64_t r = ax > ay ? ax : ay;
    uint64_t side = 2 * (uint64_t)r + 1;
    uint64_t mmax = side * side;
    uint64_t d;
    if (y == -r) d = (uint64_t)(r - x);
    else if (x == -r) d = 2 * (uint64_t)r + (uint64_t)(y + r);
    else if (y == r) d = 4 * (uint64_t)r + (uint64_t)(x + r);
    else d = 6 * (uint64_t)r + (uint64_t)(r - y);
    return mmax - d;
}

static void A308895_index_to_xy(uint64_t n, int64_t *x, int64_t *y)
{
    if (n <= 1) { *x = 0; *y = 0; return; }
    uint64_t isq = A308895_isqrt(n);
    if (isq * isq < n) ++isq;
    if ((isq & 1) == 0) ++isq;
    uint64_t r = (isq - 1) >> 1;
    uint64_t mmax = isq * isq;
    uint64_t d = mmax - n;
    if (d <= 2 * r) { *x = (int64_t)r - (int64_t)d; *y = -(int64_t)r; }
    else if (d <= 4 * r) { *x = -(int64_t)r; *y = -(int64_t)r + (int64_t)(d - 2 * r); }
    else if (d <= 6 * r) { *x = -(int64_t)r + (int64_t)(d - 4 * r); *y = (int64_t)r; }
    else { *x = (int64_t)r; *y = (int64_t)r - (int64_t)(d - 6 * r); }
}

seq_t a(idx_t n)
{
    static const int dx[8] = { 1, 1, -1, -1, 2, 2, -2, -2};
    static const int dy[8] = { 2,-2, 2, -2, 1,-1, 1, -1};
    if (n == 0) return 0;
    while (A308895_npos < (size_t)n) {
        uint64_t m = (uint64_t)A308895_len + 1;
        if (A308895_len >= A308895_cap) {
            size_t nc = A308895_cap ? A308895_cap * 2 : 4096;
            uint8_t *p = (uint8_t *)realloc(A308895_vals, nc);
            if (!p) return 0;
            A308895_vals = p;
            A308895_cap = nc;
        }
        uint8_t v;
        if (m == 1) {
            v = 1;
        } else {
            int64_t x, y;
            A308895_index_to_xy(m, &x, &y);
            bool seen[16];
            for (int i = 0; i < 16; ++i) seen[i] = false;
            for (int i = 0; i < 8; ++i) {
                uint64_t k = A308895_xy_to_index(x + dx[i], y + dy[i]);
                if (k >= 1 && k < m) {
                    uint8_t w = A308895_vals[k - 1];
                    if (w < 16) seen[w] = true;
                }
            }
            int r = 1;
            while (r < 15 && seen[r]) ++r;
            v = (uint8_t)r;
        }
        A308895_vals[A308895_len++] = v;
        if (v == 5) {
            if (A308895_npos >= A308895_pcap) {
                size_t nc = A308895_pcap ? A308895_pcap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(A308895_pos, nc * sizeof(uint64_t));
                if (!p) return 0;
                A308895_pos = p;
                A308895_pcap = nc;
            }
            A308895_pos[A308895_npos++] = m;
        }
    }
    return (seq_t)A308895_pos[(size_t)n - 1];
}
