/*
 * A308893  Positions of 3's on the spiral defined in A308890.
 * https://oeis.org/A308893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 3 in A308890 (equivalently
 *            A308887(n)+1); computed by sequentially simulating the
 *            knight-mex spiral of A308890 and recording positions of 3's
 * Range:     u8 n <= 54, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t A308893_xy_to_index(int64_t x, int64_t y)
{
    int64_t absx = x >= 0 ? x : -x;
    int64_t absy = y >= 0 ? y : -y;
    int64_t r = absx > absy ? absx : absy;
    idx_t side = 2 * (idx_t)r + 1;
    idx_t mmax = side * side;
    idx_t d;
    if (y == -r) {
        d = (idx_t)(r - x);
    } else if (x == -r) {
        d = 2 * (idx_t)r + (idx_t)(y + r);
    } else if (y == r) {
        d = 4 * (idx_t)r + (idx_t)(x + r);
    } else {
        d = 6 * (idx_t)r + (idx_t)(r - y);
    }
    return mmax - d;
}

static idx_t A308893_isqrt(idx_t n)
{
    if (n <= 1) return n;
    idx_t low = 0, high = n;
    if (high > 4294967296ULL) high = 4294967296ULL;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

static void A308893_index_to_xy(idx_t n, int64_t *x, int64_t *y)
{
    if (n <= 1) { *x = 0; *y = 0; return; }
    idx_t isq = A308893_isqrt(n);
    if (isq * isq < n) ++isq;
    if ((isq & 1) == 0) ++isq;
    idx_t r = (isq - 1) >> 1;
    idx_t side = 2 * r + 1;
    idx_t mmax = side * side;
    idx_t d = mmax - n;
    if (d <= 2 * r) {
        *x = (int64_t)r - (int64_t)d;
        *y = -(int64_t)r;
    } else if (d <= 4 * r) {
        *x = -(int64_t)r;
        *y = -(int64_t)r + (int64_t)(d - 2 * r);
    } else if (d <= 6 * r) {
        *x = -(int64_t)r + (int64_t)(d - 4 * r);
        *y = (int64_t)r;
    } else {
        *x = (int64_t)r;
        *y = (int64_t)r - (int64_t)(d - 6 * r);
    }
}

static uint8_t *A308893_vals = NULL;
static size_t A308893_len = 0, A308893_cap = 0;
static uint64_t *A308893_pos = NULL;
static size_t A308893_cnt = 0, A308893_pcap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    static const int dx[8] = { 1, 1, -1, -1,  2,  2, -2, -2};
    static const int dy[8] = { 2,-2,  2, -2,  1, -1,  1, -1};
    while (A308893_cnt < (size_t)n) {
        if (A308893_len == A308893_cap) {
            size_t nc = A308893_cap ? A308893_cap * 2 : 1024;
            uint8_t *p = (uint8_t*)realloc(A308893_vals, nc);
            if (!p) return 0;
            A308893_vals = p;
            A308893_cap = nc;
        }
        idx_t m = (idx_t)A308893_len + 1;
        uint8_t v;
        if (m == 1) {
            v = 1;
        } else {
            int64_t x, y;
            A308893_index_to_xy(m, &x, &y);
            bool seen[16];
            for (int i = 0; i < 16; ++i) seen[i] = false;
            for (int i = 0; i < 8; ++i) {
                idx_t k = A308893_xy_to_index(x + dx[i], y + dy[i]);
                if (k >= 1 && k < m) {
                    uint8_t w = A308893_vals[k - 1];
                    if (w < 16) seen[w] = true;
                }
            }
            int r = 1;
            while (r < 15 && seen[r]) ++r;
            v = (uint8_t)r;
        }
        A308893_vals[A308893_len++] = v;
        if (v == 3) {
            if (A308893_cnt == A308893_pcap) {
                size_t nc = A308893_pcap ? A308893_pcap * 2 : 256;
                uint64_t *p = (uint64_t*)realloc(A308893_pos, nc * sizeof(uint64_t));
                if (!p) return 0;
                A308893_pos = p;
                A308893_pcap = nc;
            }
            A308893_pos[A308893_cnt++] = (uint64_t)m;
        }
    }
    return (seq_t)A308893_pos[n - 1];
}
