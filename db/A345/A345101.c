/*
 * A345101  Irregular triangle T(n, k) read by rows, n >= 0, k =
 *          1..A000119(n); the n-th row contains the numbers m such that
 *          A022290(m) = n, in increasing order.
 * https://oeis.org/A345101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   flat triangle; row n lists, in increasing order, the m with
 *            A022290(m) = n (A022290(m) = sum of F(k+2) over set bits k of
 *            m). Row n has A000119(n) entries. The code locates row n by
 *            accumulating A000119 counts, then picks the j-th smallest m bit
 *            by bit, counting Fibonacci representations
 * Range:     u8 n <= 174, u16 n <= 10922, u32 n <= 10922, u64 n <= 10922,
 *            u128 n <= 10922, big any n
 * Validated: standard, pass, 84 terms through n = 9660 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t T345101_g[80];
static uint64_t T345101_s[80];
static int T345101_ready = 0;

static void T345101_init(void)
{
    int i;
    if (T345101_ready) return;
    T345101_g[0] = 1;
    T345101_g[1] = 2;
    for (i = 2; i < 80; i++) T345101_g[i] = T345101_g[i - 1] + T345101_g[i - 2];
    T345101_s[0] = T345101_g[0];
    for (i = 1; i < 80; i++) T345101_s[i] = T345101_s[i - 1] + T345101_g[i];
    T345101_ready = 1;
}

/* number of subsets of {g[0..i]} summing to rem */
static uint64_t T345101_cnt(uint64_t rem, int i)
{
    uint64_t r = 0;
    if (rem == 0) return 1;
    if (i < 0) return 0;
    if (rem > T345101_s[i]) return 0;
    r = T345101_cnt(rem, i - 1);
    if (T345101_g[i] <= rem) r += T345101_cnt(rem - T345101_g[i], i - 1);
    return r;
}

static int T345101_top(uint64_t n)
{
    int k = -1;
    while (k + 1 < 80 && T345101_g[k + 1] <= n) k++;
    return k;
}

seq_t a(idx_t N)
{
    uint64_t n = 0, j, rem;
    int i, K;
    seq_t m = 0;
    T345101_init();
    j = (uint64_t)N;
    for (;;) {
        uint64_t c = T345101_cnt(n, T345101_top(n));
        if (j < c) break;
        j -= c;
        n++;
    }
    rem = n;
    K = T345101_top(n);
    for (i = K; i >= 0; i--) {
        uint64_t c0 = T345101_cnt(rem, i - 1);
        m = m * 2;
        if (j < c0) {
            /* bit 0 */
        } else {
            j -= c0;
            rem -= T345101_g[i];
            m = m + 1;
        }
    }
    return m;
}
