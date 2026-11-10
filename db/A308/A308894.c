/*
 * A308894  Positions of 4's on the spiral defined in A308890.
 * https://oeis.org/A308894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index k of the n-th 4 in A308890 (knight-mex spiral);
 *            computed by direct simulation of the spiral
 * Range:     u8 n <= 43, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t A308894_xy_to_index(int64_t x, int64_t y)
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

static idx_t A308894_isqrt(idx_t n)
{
    if (n <= 1) return n;
    idx_t low = 0, high = n < 4294967296ULL ? n : 4294967296ULL;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

static void A308894_index_to_xy(idx_t n, int64_t *x, int64_t *y)
{
    if (n <= 1) { *x = 0; *y = 0; return; }
    idx_t isq = A308894_isqrt(n);
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

static uint8_t *A308894_vals = NULL;
static idx_t A308894_nvals = 0, A308894_cap = 0;
static idx_t *A308894_pos = NULL;
static idx_t A308894_npos = 0, A308894_poscap = 0;

static bool A308894_step(void)
{
    idx_t n = A308894_nvals + 1;
    if (A308894_nvals >= A308894_cap) {
        idx_t nc = A308894_cap ? A308894_cap * 2 : 4096;
        uint8_t *p = (uint8_t *)realloc(A308894_vals, (size_t)nc);
        if (!p) return false;
        A308894_vals = p;
        A308894_cap = nc;
    }
    uint8_t result = 1;
    if (n > 1) {
        static const int dx[8] = { 1, 1, -1, -1,  2,  2, -2, -2};
        static const int dy[8] = { 2,-2,  2, -2,  1, -1,  1, -1};
        int64_t x, y;
        bool seen[8] = {false,false,false,false,false,false,false,false};
        A308894_index_to_xy(n, &x, &y);
        for (int i = 0; i < 8; ++i) {
            idx_t m = A308894_xy_to_index(x + dx[i], y + dy[i]);
            if (m >= 1 && m < n) {
                uint8_t v = A308894_vals[m - 1];
                if (v <= 6) seen[v] = true;
            }
        }
        while (result <= 6 && seen[result]) ++result;
    }
    A308894_vals[A308894_nvals++] = result;
    if (result == 4) {
        if (A308894_npos >= A308894_poscap) {
            idx_t nc = A308894_poscap ? A308894_poscap * 2 : 256;
            idx_t *p = (idx_t *)realloc(A308894_pos, (size_t)nc * sizeof(idx_t));
            if (!p) return false;
            A308894_pos = p;
            A308894_poscap = nc;
        }
        A308894_pos[A308894_npos++] = n;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A308894_npos < n) {
        if (!A308894_step()) return 0;
    }
    return (seq_t)A308894_pos[n - 1];
}
