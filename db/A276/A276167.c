/*
 * A276167  a(n) is the second player's score in a "Coins in a Row" game over
 *          the n-th row of A066099 using a minimax strategy.
 * https://oeis.org/A276167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A029837(n+1) - A276166(n); computed by minimax interval
 *            DP over the coins of the n-th row of A066099 (row n =
 *            composition of k=floor(log2 n)+1 with binary index n-2^(k-1);
 *            row 0 = [0])
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9887 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    int k = 0;
    {
        idx_t m = n;
        while (m > 0) { m >>= 1; k++; }
    }
    idx_t j = n - (((idx_t)1) << (k - 1));

    long long parts[70];
    int cnt = 0;
    long long len = 1;
    for (int p = 1; p < k; ++p) {
        if ((j >> (k - 1 - p)) & 1) {
            parts[cnt++] = len;
            len = 1;
        } else {
            ++len;
        }
    }
    parts[cnt++] = len;

    static long long f[70][70];
    long long pre[71];
    pre[0] = 0;
    for (int i = 0; i < cnt; ++i) pre[i + 1] = pre[i] + parts[i];

    for (int i = 0; i < cnt; ++i) f[i][i] = parts[i];
    for (int w = 2; w <= cnt; ++w) {
        for (int i = 0; i + w - 1 < cnt; ++i) {
            int e = i + w - 1;
            long long s = pre[e + 1] - pre[i];
            long long x = f[i + 1][e];
            long long y = f[i][e - 1];
            f[i][e] = s - (x < y ? x : y);
        }
    }
    long long total = pre[cnt];
    return (seq_t)(total - f[0][cnt - 1]);
}
