/*
 * A344145  Positive numbers m such that A020330^k(m) belongs to A344022 for
 *          any k >= 0 (where f^k denotes the k-th iterate of f).
 * https://oeis.org/A344145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th m >= 1 whose binary string (b_1..b_L), read as a
 *            periodic walk (turn +90 for 1, -90 for 0, then a unit step),
 *            never reuses an edge; found by search with caching
 * Range:     u8 n <= 45, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static int A344145_ok(seq_t m)
{
    int L = 0;
    seq_t t = m;
    while (t > 0) { t >>= 1; L++; }
    if (L < 2 || L > 250) return 0;
    static const int DX[4] = {1, 0, -1, 0};
    static const int DY[4] = {0, 1, 0, -1};
    int ex[256], ey[256], et[256];
    int x = 0, y = 0, dir = 0;
    for (int i = 0; i < L; i++) {
        int b = (int)((m >> (L - 1 - i)) & 1);
        dir = b ? ((dir + 1) & 3) : ((dir + 3) & 3);
        int nx = x + DX[dir], ny = y + DY[dir];
        et[i] = (dir & 1);
        ex[i] = (nx < x) ? nx : x;
        ey[i] = (ny < y) ? ny : y;
        x = nx; y = ny;
    }
    if (dir != 0) return 0;
    int vx = x, vy = y;
    if (vx == 0 && vy == 0) return 0;
    for (int i = 0; i < L; i++) {
        for (int j = i + 1; j < L; j++) {
            if (et[i] != et[j]) continue;
            int dx = ex[i] - ex[j], dy = ey[i] - ey[j];
            int mult = 0;
            if (vx != 0) {
                if (dx % vx == 0) {
                    int d = dx / vx;
                    if (dy == d * vy) mult = 1;
                }
            } else {
                if (dx == 0 && dy % vy == 0) mult = 1;
            }
            if (mult) return 0;
        }
    }
    return 1;
}

seq_t a(idx_t n)
{
    static seq_t *terms = 0;
    static size_t cnt = 0, cap = 0;
    static seq_t next_m = 1;
    if (terms == 0) {
        cap = 256;
        terms = mem_new(seq_t, cap);
    }
    while (cnt < (size_t)n) {
        if (A344145_ok(next_m)) {
            if (cnt == cap) {
                cap *= 2;
                terms = mem_resize(seq_t, terms, cap);
            }
            terms[cnt++] = next_m;
        }
        next_m++;
    }
    return terms[n - 1];
}
