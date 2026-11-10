/*
 * A345291  a(n) is the least k >= 0 such that A345290(k) = n.
 * https://oeis.org/A345291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k >= 0 with A345290(k) = n, where A345290(k) =
 *            Sum_{bits j of k set} (-1)^(j+1)*Fib(j+2). Computed by greedy
 *            search: subset sums of the first j weights fill a contiguous
 *            range, so pick the lowest top bit, then clear bits where
 *            possible
 * Range:     u8 n <= 54, u16 n <= 2583, u32 n <= 6764, u64 n <= 6764,
 *            u128 n <= 6764, big any n
 * Validated: standard, pass, 73 terms through n = 6555 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool A345291_inited = false;
static long long A345291_w[90];
static long long A345291_lo[91];
static long long A345291_hi[91];

static void A345291_init(void)
{
    if (A345291_inited) return;
    long long f[92];
    f[0] = 0; f[1] = 1;
    for (int i = 2; i < 92; ++i) f[i] = f[i-1] + f[i-2];
    A345291_lo[0] = 0;
    A345291_hi[0] = 0;
    for (int j = 0; j < 90; ++j) {
        long long wj = f[j+2];
        if ((j & 1) == 0) wj = -wj;
        A345291_w[j] = wj;
        A345291_lo[j+1] = A345291_lo[j] + (wj < 0 ? wj : 0);
        A345291_hi[j+1] = A345291_hi[j] + (wj > 0 ? wj : 0);
    }
    A345291_inited = true;
}

seq_t a(idx_t n)
{
    A345291_init();
    if (n == 0) return (seq_t)0;
    long long t = (long long)n;
    int K = 0;
    for (; K < 89; ++K) {
        long long r = t - A345291_w[K];
        if (r >= A345291_lo[K] && r <= A345291_hi[K]) {
            t = r;
            break;
        }
    }
    seq_t res = (seq_t)1 << K;
    for (int i = K - 1; i >= 0; --i) {
        if (t >= A345291_lo[i] && t <= A345291_hi[i]) {
            /* bit i stays 0 */
        } else {
            t -= A345291_w[i];
            res |= ((seq_t)1 << i);
        }
    }
    return res;
}
