/*
 * A374396  a(n) is the last term in the n-th row of A374394.
 * https://oeis.org/A374396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the maximum sum of a subset of the lazy Fibonacci
 *            representation of n. The lazy representation of n is the unique
 *            representation using distinct Fibonacci numbers (F_1=1, F_2=2,
 *            …) that minimizes the value of the largest Fibonacci number used
 * Range:     u8 n <= 375, u16 n <= 10944, u32 n <= 10944, u64 n <= 10944,
 *            u128 n <= 10944, big any n
 * Validated: standard, pass, 84 terms through n = 9811 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A374396: the largest z <= n whose Zeckendorf representation and that of
 * n - z share no Fibonacci number and together make the lazy Fibonacci
 * representation of n (the last term of row n of A374394).
 *
 * With f_1, f_2, ... = 1, 2, 3, 5, ..., the lazy representation of n uses
 * f_1..f_k with k least such that f_1 + ... + f_k = f_(k+2) - 2 >= n, all but
 * the Zeckendorf representation of the excess f_(k+2) - 2 - n (no two
 * adjacent, so no two adjacent gaps remain). Splitting it into two Zeckendorf
 * representations means alternating along each run of consecutive indices, so
 * z is largest taking, in each run, the heavier of its two alternate halves.
 * (The original tabulated n <= 10000 only.) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    const uint64_t N = (uint64_t)n;
    uint64_t f[96];
    int k = 0;
    f[1] = 1;
    f[2] = 2;
    for (int i = 3; i < 96; ++i) f[i] = f[i - 1] + f[i - 2];
    uint64_t sum = 0;
    while (sum < N) sum += f[++k];
    /* used[i]: f_i in the lazy representation */
    unsigned char used[96] = {0};
    for (int i = 1; i <= k; ++i) used[i] = 1;
    uint64_t ex = sum - N;
    for (int i = k; i >= 1 && ex; --i)
        if (f[i] <= ex) { ex -= f[i]; used[i] = 0; }
    uint64_t z = 0;
    for (int i = 1; i <= k;) {
        if (!used[i]) { ++i; continue; }
        uint64_t half[2] = {0, 0};
        int j = i;
        for (; j <= k && used[j]; ++j) half[(j - i) & 1] += f[j];
        z += half[0] > half[1] ? half[0] : half[1];
        i = j;
    }
    return (seq_t)z;
}
