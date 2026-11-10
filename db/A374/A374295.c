/*
 * A374295  a(n) is the smallest positive integer k such that A096936(k) = n.
 * https://oeis.org/A374295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min k with A096936(k) = n. A096936 is multiplicative,
 *            with value 3 at 2^e (e even, e>0), 1 at 3^e, 1+e at p^e (p == 1
 *            mod 3), and 1 at p^e (p == 2 mod 3, e even). The code searches
 *            the factorizations of n (optionally using a factor 3 from 4) and
 *            assigns exponents to 7, 13, 19,
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 28, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static double g_best;
static int g_found;
static int g_flag, g_bflag;
static uint64_t g_cur[64], g_bexp[64];
static int g_bl;
static double g_logp[64];
static uint64_t g_pr[64];
static int g_init = 0;

static void A374295_init(void)
{
    if (g_init) return;
    int c = 0;
    uint64_t p = 7;
    while (c < 64) {
        if (p % 3 == 1) {
            bool pr = true;
            for (uint64_t d = 2; d * d <= p; d++)
                if (p % d == 0) { pr = false; break; }
            if (pr) {
                g_pr[c] = p;
                g_logp[c] = log((double)p);
                c++;
            }
        }
        p++;
    }
    g_init = 1;
}

static void A374295_rec(uint64_t m, uint64_t maxf, int depth, double lc)
{
    if (g_found && lc >= g_best) return;
    if (m == 1) {
        g_best = lc;
        g_found = 1;
        g_bflag = g_flag;
        g_bl = depth;
        for (int i = 0; i < depth; i++) g_bexp[i] = g_cur[i];
        return;
    }
    if (depth >= 60) return;
    for (uint64_t d = 1; d * d <= m; d++) {
        if (m % d) continue;
        uint64_t fs[2];
        int nf = 0;
        fs[nf++] = d;
        if (m / d != d) fs[nf++] = m / d;
        for (int t = 0; t < nf; t++) {
            uint64_t f = fs[t];
            if (f < 2 || f > maxf) continue;
            g_cur[depth] = f - 1;
            A374295_rec(m / f, f, depth + 1, lc + (double)(f - 1) * g_logp[depth]);
        }
    }
}

extern seq_t a(idx_t n)
{
    A374295_init();
    if (n <= 1) return (seq_t)1;
    uint64_t N = (uint64_t)n;
    g_found = 0;
    g_flag = 0;
    A374295_rec(N, N, 0, 0.0);
    if (N % 3 == 0) {
        g_flag = 1;
        A374295_rec(N / 3, N / 3, 0, log(4.0));
    }
    seq_t res = (seq_t)1;
    if (g_bflag) res = (seq_t)4;
    for (int i = 0; i < g_bl; i++) {
        for (uint64_t j = 0; j < g_bexp[i]; j++)
            res *= (seq_t)g_pr[i];
    }
    return res;
}
