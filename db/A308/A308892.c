/*
 * A308892  Positions of 2's on the spiral defined in A308890.
 * https://oeis.org/A308892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th 2 in A308890 (knight-mex spiral);
 *            computed by simulating the spiral
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t A308892_xy_to_index(int64_t x, int64_t y)
{
    int64_t absx = x >= 0 ? x : -x;
    int64_t absy = y >= 0 ? y : -y;
    int64_t r = absx > absy ? absx : absy;
    idx_t side = 2 * (idx_t)r + 1;
    idx_t mmax = side * side;
    idx_t d;
    if (y == -r) d = (idx_t)(r - x);
    else if (x == -r) d = 2 * (idx_t)r + (idx_t)(y + r);
    else if (y == r) d = 4 * (idx_t)r + (idx_t)(x + r);
    else d = 6 * (idx_t)r + (idx_t)(r - y);
    return mmax - d;
}

static idx_t A308892_isqrt(idx_t n)
{
    if (n <= 1) return n;
    idx_t low = 0, high = n < 4294967296ULL ? n : 4294967296ULL;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

static void A308892_index_to_xy(idx_t n, int64_t *x, int64_t *y)
{
    if (n <= 1) { *x = 0; *y = 0; return; }
    idx_t isq = A308892_isqrt(n);
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

static uint8_t *A308892_vals = NULL;   /* vals[m-1] = A308890(m) */
static size_t A308892_nvals = 0, A308892_valcap = 0;
static idx_t *A308892_pos = NULL;      /* positions of 2's */
static size_t A308892_npos = 0, A308892_poscap = 0;

static void A308892_step(void)
{
    idx_t m = (idx_t)A308892_nvals + 1;
    if (A308892_nvals >= A308892_valcap) {
        size_t nc = A308892_valcap ? A308892_valcap * 2 : 1024;
        A308892_vals = (uint8_t *)realloc(A308892_vals, nc);
        A308892_valcap = nc;
    }
    uint8_t res = 1;
    if (m > 1) {
        int64_t x, y;
        A308892_index_to_xy(m, &x, &y);
        static const int dx[8] = {1, 1, -1, -1, 2, 2, -2, -2};
        static const int dy[8] = {2, -2, 2, -2, 1, -1, 1, -1};
        bool seen[8] = {false, false, false, false, false, false, false, false};
        for (int i = 0; i < 8; ++i) {
            idx_t k = A308892_xy_to_index(x + dx[i], y + dy[i]);
            if (k >= 1 && k < m) {
                uint8_t v = A308892_vals[k - 1];
                if (v < 8) seen[v] = true;
            }
        }
        while (res < 8 && seen[res]) ++res;
    }
    A308892_vals[A308892_nvals++] = res;
    if (res == 2) {
        if (A308892_npos >= A308892_poscap) {
            size_t nc = A308892_poscap ? A308892_poscap * 2 : 256;
            A308892_pos = (idx_t *)realloc(A308892_pos, nc * sizeof(idx_t));
            A308892_poscap = nc;
        }
        A308892_pos[A308892_npos++] = m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A308892_npos < (size_t)n) A308892_step();
    return (seq_t)A308892_pos[n - 1];
}
