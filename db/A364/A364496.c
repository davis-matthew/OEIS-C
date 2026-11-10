/*
 * A364496  Numbers k such that k is a multiple of A163511(k).
 * https://oeis.org/A364496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 0 with A163511(k) | k (A163511(0)=1, so
 *            a(1)=0). A163511(2k) = 2*A163511(k), so terms are 0 and odd
 *            terms t times 2^j. Odd terms are found by a pruned DFS over the
 *            gap vectors between set bits (A163511(k) = p_r * prod p_i^gap_i,
 *            r = popcount), with increasing bit-length bound
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 53 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A364496_MAXODD 2048
#define A364496_MAXALL 65536

static uint64_t A364496_pr[70];
static int A364496_pinit = 0;
static uint64_t A364496_odd[A364496_MAXODD];
static int A364496_nodd = 0;
static int A364496_cachedL = 0;
static uint64_t A364496_all[A364496_MAXALL];

static void A364496_init_primes(void)
{
    int cnt = 0;
    uint64_t c;
    if (A364496_pinit) return;
    for (c = 2; cnt < 66; ++c) {
        uint64_t d;
        int ok = 1;
        for (d = 2; d * d <= c; ++d)
            if (c % d == 0) { ok = 0; break; }
        if (ok) A364496_pr[++cnt] = c;
    }
    A364496_pinit = 1;
}

static void A364496_dfs(int i, int r, int pos, uint64_t k, uint64_t P,
                        uint64_t lim, int L)
{
    uint64_t Pcur = P;
    int g;
    if (i > r) {
        uint64_t A = P * A364496_pr[r];
        if (k % A == 0 && A364496_nodd < A364496_MAXODD)
            A364496_odd[A364496_nodd++] = k;
        return;
    }
    for (g = 0;; ++g) {
        int np;
        if (g > 0) {
            if (Pcur > lim / A364496_pr[i]) break;
            Pcur *= A364496_pr[i];
        }
        np = pos + g + 1;
        if (np + (r - i) > L - 1) break;
        A364496_dfs(i + 1, r, np, k | ((uint64_t)1 << np), Pcur, lim, L);
    }
}

static void A364496_search(int L)
{
    int r;
    A364496_nodd = 0;
    for (r = 1; r <= L && r <= 64; ++r) {
        uint64_t lim = (((uint64_t)1 << L) - 1) / A364496_pr[r];
        A364496_dfs(2, r, 0, 1, 1, lim, L);
    }
    A364496_cachedL = L;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    A364496_init_primes();
    for (;;) {
        int cnt = 0, i;
        int L = A364496_cachedL;
        if (L > 0) {
            for (i = 0; i < A364496_nodd; ++i) {
                uint64_t v = A364496_odd[i];
                while (v < ((uint64_t)1 << L) && cnt < A364496_MAXALL) {
                    A364496_all[cnt++] = v;
                    v <<= 1;
                }
            }
            if ((uint64_t)cnt >= (uint64_t)(n - 1)) {
                /* sort */
                int x, y;
                for (x = 1; x < cnt; ++x) {
                    uint64_t key = A364496_all[x];
                    y = x - 1;
                    while (y >= 0 && A364496_all[y] > key) {
                        A364496_all[y + 1] = A364496_all[y];
                        --y;
                    }
                    A364496_all[y + 1] = key;
                }
                return (seq_t)A364496_all[n - 2];
            }
        }
        if (L >= 62) return (seq_t)0;
        L = (L == 0) ? 12 : L + 4;
        if (L > 62) L = 62;
        A364496_search(L);
    }
}
