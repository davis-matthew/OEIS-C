/*
 * A266999  Smallest number m such that A266998(m) = n.
 * https://oeis.org/A266999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, more, nonn
 * Formula:   a(n) = smallest m with A266998(m) = n. Search: brute force over
 *            m < 10^6, else build m from digits. f(m) depends only on the
 *            multiset of nonzero digits, so the smallest m has nonzero digits
 *            in ascending order. Find the fewest digits d for which some t
 *            with A266998(t) = n-1 is a sum of d values T(...
 * Range:     u8 n <= 16, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: standard, pass, 19 terms through n = 18 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A266999_DMAX 120
#define A266999_SMAX (45 * A266999_DMAX)

static const unsigned A266999_T[10] = {0,1,3,6,10,15,21,28,36,45};

static int A266999_steps(uint64_t m)
{
    int steps = 0;
    while (m != 1) {
        uint64_t s = 0, t = m;
        while (t > 0) {
            s += A266999_T[t % 10];
            t /= 10;
        }
        m = s;
        steps++;
        if (steps > 500) return -1;
    }
    return steps;
}

/* R[k][r][s]: sum s achievable with exactly r digits from {k..9}, k=1..10 */
static unsigned char *A266999_R = 0;

static size_t A266999_idx(int k, int r, int s)
{
    return ((size_t)k * (A266999_DMAX + 1) + (size_t)r) * (A266999_SMAX + 1) + (size_t)s;
}

static void A266999_build(void)
{
    int k, r, s;
    if (A266999_R) return;
    A266999_R = (unsigned char *)calloc((size_t)11 * (A266999_DMAX + 1) * (A266999_SMAX + 1), 1);
    A266999_R[A266999_idx(10, 0, 0)] = 1;
    for (k = 9; k >= 1; k--) {
        int tk = (int)A266999_T[k];
        for (r = 0; r <= A266999_DMAX; r++)
            for (s = 0; s <= A266999_SMAX; s++) {
                unsigned char v = A266999_R[A266999_idx(k + 1, r, s)];
                if (!v && r > 0 && s >= tk) v = A266999_R[A266999_idx(k, r - 1, s - tk)];
                A266999_R[A266999_idx(k, r, s)] = v;
            }
    }
}

seq_t a(idx_t n)
{
    uint64_t m;
    int d;
    for (m = 1; m < 1000000ULL; m++) {
        int s = A266999_steps(m);
        if (s >= 0 && (uint64_t)s == (uint64_t)n) return (seq_t)m;
    }
    if (n == 0) return 0;
    A266999_build();
    for (d = 1; d <= A266999_DMAX; d++) {
        int best[10], have = 0, t;
        for (t = 1; t <= 45 * d; t++) {
            int cnt[10], k, r, s, better, ok;
            int st = A266999_steps((uint64_t)t);
            if (st < 0 || (uint64_t)st != (uint64_t)(n - 1)) continue;
            if (!A266999_R[A266999_idx(1, d, t)]) continue;
            r = d; s = t; ok = 1;
            for (k = 1; k <= 8; k++) {
                int tk = (int)A266999_T[k], c;
                int cmax = r;
                if (cmax > s / tk) cmax = s / tk;
                for (c = cmax; c >= 0; c--)
                    if (A266999_R[A266999_idx(k + 1, r - c, s - c * tk)]) break;
                if (c < 0) { ok = 0; break; }
                cnt[k] = c; r -= c; s -= c * tk;
            }
            if (!ok || s != 45 * r) continue;
            cnt[9] = r;
            better = !have;
            if (have) {
                for (k = 1; k <= 9; k++) {
                    if (cnt[k] != best[k]) { better = cnt[k] > best[k]; break; }
                }
            }
            if (better) {
                for (k = 1; k <= 9; k++) best[k] = cnt[k];
                have = 1;
            }
        }
        if (have) {
            seq_t v = 0;
            int k, i;
            for (k = 1; k <= 9; k++)
                for (i = 0; i < best[k]; i++) v = v * 10 + k;
            return v;
        }
    }
    return 0;
}
