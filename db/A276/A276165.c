/*
 * A276165  a(n) is the first-player score difference of a "Coins in a Row"
 *          game over the n-th row of A066099 using a minimax strategy.
 * https://oeis.org/A276165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A276166(n) - A276167(n); row n is the composition of m (m
 *            = bit length of n) whose m-1 low bits of n mark cuts (msb
 *            first), a(0)=0; minimax interval DP over the parts
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9251 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    int m = 0;
    for (idx_t t = n; t; t >>= 1) ++m;
    int v[70];
    int cnt = 0, len = 1;
    for (int p = 1; p < m; ++p) {
        int bit = m - p - 1;
        if ((n >> bit) & 1) {
            v[cnt++] = len;
            len = 1;
        } else {
            ++len;
        }
    }
    v[cnt++] = len;
    static int f[70][70];
    for (int i = 0; i < cnt; ++i) f[i][i] = v[i];
    for (int d = 1; d < cnt; ++d) {
        for (int i = 0; i + d < cnt; ++i) {
            int j = i + d;
            int x = v[i] - f[i + 1][j];
            int y = v[j] - f[i][j - 1];
            f[i][j] = x > y ? x : y;
        }
    }
    return (seq_t)f[0][cnt - 1];
}
