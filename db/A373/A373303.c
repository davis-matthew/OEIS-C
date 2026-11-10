/*
 * A373303  First differences of A371176.
 * https://oeis.org/A373303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A371176(n+1) - A371176(n); A371176 found by counting
 *            (digit DP on k = m*2^t, m odd, popcount(m) <= t+1) plus binary
 *            search, then scanning for the next term
 * Range:     u8 n <= 35, u16 n <= 1643, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9678 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t D_C[130][130];
static bool D_init = false;

static void d_init(void)
{
    if (D_init) return;
    for (int i = 0; i < 130; i++) {
        D_C[i][0] = 1;
        for (int j = 1; j <= i; j++) {
            seq_t v = D_C[i - 1][j - 1];
            if (j <= i - 1) v += D_C[i - 1][j];
            D_C[i][j] = v;
        }
        for (int j = i + 1; j < 130; j++) D_C[i][j] = 0;
    }
    D_init = true;
}

/* count j in [0,J] with popcount(j) <= r */
static seq_t d_cnt(seq_t J, int r)
{
    int len = 0;
    seq_t tmp = J;
    while (tmp != 0) { len++; tmp >>= 1; }
    int ones = 0;
    seq_t total = 0;
    for (int b = len - 1; b >= 0; b--) {
        if (((J >> b) & 1) != 0) {
            int lim = r - ones;
            if (lim >= 0) {
                if (lim > b) lim = b;
                for (int i = 0; i <= lim; i++) total += D_C[b][i];
            }
            ones++;
            if (ones > r) return total;
        }
    }
    if (ones <= r) total += 1;
    return total;
}

static seq_t d_count(seq_t X)
{
    seq_t total = 0;
    for (int t = 0; t < 128; t++) {
        seq_t M = X >> t;
        if (M == 0) break;
        total += d_cnt((M - 1) / 2, t);
    }
    return total;
}

static bool d_ok(seq_t k)
{
    int tz = 0;
    seq_t tmp = k;
    while ((tmp & 1) == 0) { tz++; tmp >>= 1; }
    int pop = 0;
    while (tmp != 0) { pop += (int)(tmp & 1); tmp >>= 1; }
    return pop <= tz + 1;
}

seq_t a(idx_t n)
{
    d_init();
    seq_t hi = 1;
    while (d_count(hi) < (seq_t)n) {
        seq_t nh = hi * 2;
        if (nh <= hi) break;
        hi = nh;
    }
    seq_t lo = 1;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (d_count(mid) >= (seq_t)n) hi = mid;
        else lo = mid + 1;
    }
    seq_t k = lo + 1;
    while (!d_ok(k)) k++;
    return k - lo;
}
