/*
 * A381610  Irregular table T(n, k), n >= 0, k = 1..A381609(n), read by rows:
 *          the n-th row lists the numbers m such that A381607(m) = n.
 * https://oeis.org/A381610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   T(n,k) = k-th smallest m with A381607(m) = n, where A381607(m) =
 *            Sum t_j*F(2j+2) over the base-3 digits t_j of m. The code finds
 *            each row by a pruned DFS over digits, and walks rows in order to
 *            reach flat index n
 * Range:     u8 n <= 233, u16 n <= 4693, u32 n <= 4693, u64 n <= 4693,
 *            u128 n <= 4693, big any n
 * Validated: standard, pass, 84 terms through n = 4574 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned long long A381610_w[42];
static unsigned long long A381610_p3[42];
static unsigned long long A381610_cap[42];
static int A381610_ready = 0;
static unsigned long long A381610_buf[512];
static int A381610_cnt;

static void A381610_init(void)
{
    unsigned long long f0 = 0, f1 = 1;
    unsigned long long F[90];
    F[0] = 0; F[1] = 1;
    for (int i = 2; i < 90; ++i) F[i] = F[i-1] + F[i-2];
    (void)f0; (void)f1;
    for (int k = 0; k < 40; ++k) {
        A381610_w[k] = F[2*k + 2];
        A381610_p3[k] = (k == 0) ? 1ULL : A381610_p3[k-1] * 3ULL;
    }
    A381610_cap[0] = 0;
    for (int k = 1; k < 40; ++k)
        A381610_cap[k] = A381610_cap[k-1] + 2ULL * A381610_w[k-1];
    A381610_ready = 1;
}

static void A381610_dfs(int k, unsigned long long rem, unsigned long long m)
{
    if (k < 0) {
        if (rem == 0 && A381610_cnt < 512) A381610_buf[A381610_cnt++] = m;
        return;
    }
    for (unsigned t = 0; t < 3; ++t) {
        unsigned long long c = (unsigned long long)t * A381610_w[k];
        if (c > rem) break;
        if (rem - c <= A381610_cap[k])
            A381610_dfs(k - 1, rem - c, m + (unsigned long long)t * A381610_p3[k]);
    }
}

seq_t a(idx_t n)
{
    if (!A381610_ready) A381610_init();
    unsigned long long idx = (unsigned long long)n;
    for (unsigned long long v = 0;; ++v) {
        int top = 0;
        while (top + 1 < 40 && A381610_w[top + 1] <= v) ++top;
        A381610_cnt = 0;
        A381610_dfs(top, v, 0ULL);
        int c = A381610_cnt;
        /* insertion sort */
        for (int i = 1; i < c; ++i) {
            unsigned long long x = A381610_buf[i];
            int j = i - 1;
            while (j >= 0 && A381610_buf[j] > x) {
                A381610_buf[j+1] = A381610_buf[j];
                --j;
            }
            A381610_buf[j+1] = x;
        }
        if (idx < (unsigned long long)c) return (seq_t)A381610_buf[idx];
        idx -= (unsigned long long)c;
    }
}
