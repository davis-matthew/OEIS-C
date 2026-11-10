/*
 * A308891  Positions of 1's on the spiral defined in A308890.
 * https://oeis.org/A308891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th 1 in the knight-mex spiral A308890
 *            (computed by simulating the spiral; a(n) = A308885(n)+1)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t A308891_xy_to_index(int64_t x, int64_t y)
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

static idx_t A308891_isqrt(idx_t n)
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

static void A308891_index_to_xy(idx_t n, int64_t *x, int64_t *y)
{
    if (n <= 1) { *x = 0; *y = 0; return; }
    idx_t isq = A308891_isqrt(n);
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

static uint8_t *A308891_vals = NULL;
static size_t A308891_vcap = 0;
static idx_t A308891_N = 0;
static idx_t *A308891_ones = NULL;
static size_t A308891_ocap = 0;
static size_t A308891_ocnt = 0;

static void A308891_step(void)
{
    idx_t m = A308891_N + 1;
    if ((size_t)m > A308891_vcap) {
        size_t nc = A308891_vcap ? A308891_vcap * 2 : 1024;
        A308891_vals = (uint8_t*)realloc(A308891_vals, nc);
        A308891_vcap = nc;
    }
    if (A308891_ocnt >= A308891_ocap) {
        size_t nc = A308891_ocap ? A308891_ocap * 2 : 256;
        A308891_ones = (idx_t*)realloc(A308891_ones, nc * sizeof(idx_t));
        A308891_ocap = nc;
    }
    uint8_t res;
    if (m == 1) {
        res = 1;
    } else {
        static const int dx[8] = { 1, 1, -1, -1,  2,  2, -2, -2};
        static const int dy[8] = { 2,-2,  2, -2,  1, -1,  1, -1};
        int64_t x, y;
        A308891_index_to_xy(m, &x, &y);
        bool seen[16] = {false};
        for (int i = 0; i < 8; ++i) {
            idx_t k = A308891_xy_to_index(x + dx[i], y + dy[i]);
            if (k >= 1 && k < m) {
                uint8_t v = A308891_vals[k - 1];
                if (v < 16) seen[v] = true;
            }
        }
        res = 1;
        while (res < 15 && seen[res]) ++res;
    }
    A308891_vals[m - 1] = res;
    A308891_N = m;
    if (res == 1) A308891_ones[A308891_ocnt++] = m;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A308891_ocnt < (size_t)n) A308891_step();
    return (seq_t)A308891_ones[n - 1];
}
