/*
 * A328761  The least integer k >= 0 for which A328578(k) =
 *          A257993(A276086(A276086(k))) = n.
 * https://oeis.org/A328761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 0 with A328578(k) = n, where A328578(k) = 1 +
 *            (number of consecutive nonzero low-order primorial-base digits
 *            of m = A276086(k)). Computed by a search over the primorial
 *            digits of k in increasing order. A subtree is skipped when m
 *            cannot reach p_1*...*p_{n-2}, because digit n-2 o...
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define NP_A328761 80   /* primes available (eval needs n of them) */
#define ND_A328761 15   /* primorial digits of k: k < 47# ~ 6.1e17  */
#define ME_A328761 48   /* max digit value + 1 (digit < 47)          */

static unsigned P_a328761[NP_A328761];
static uint16_t pw_a328761[NP_A328761][ND_A328761][ME_A328761];
static uint16_t inv_a328761[NP_A328761][NP_A328761];
static double lg_a328761[NP_A328761];
static double S_a328761[ND_A328761 + 1];

static void init_a328761(void)
{
    int cnt = 0;
    for (unsigned c = 2; cnt < NP_A328761; ++c) {
        bool ok = true;
        for (int t = 0; t < cnt; ++t) {
            unsigned p = P_a328761[t];
            if (p * p > c) break;
            if (c % p == 0) { ok = false; break; }
        }
        if (ok) P_a328761[cnt++] = c;
    }
    for (int i = 0; i < NP_A328761; ++i) {
        unsigned pi = P_a328761[i];
        lg_a328761[i] = log((double)pi);
        for (int t = 0; t < ND_A328761; ++t) {
            unsigned q = P_a328761[t] % pi, r = 1 % pi;
            for (int e = 0; e < ME_A328761; ++e) {
                pw_a328761[i][t][e] = (uint16_t)r;
                r = (r * q) % pi;
            }
        }
        for (int j = 0; j < i; ++j) {
            unsigned q = P_a328761[j] % pi, x;
            for (x = 1; x < pi; ++x)
                if ((q * x) % pi == 1) break;
            inv_a328761[j][i] = (uint16_t)x;
        }
    }
    /* S[t] = max log contribution of digits 1..t-1 (digit 0 is fixed to 0) */
    S_a328761[0] = 0.0;
    S_a328761[1] = 0.0;
    for (int t = 2; t <= ND_A328761; ++t)
        S_a328761[t] = S_a328761[t - 1] +
                       (double)(P_a328761[t - 1] - 1) * lg_a328761[t - 1];
}

static int n_a328761;
static double thr_a328761;
static unsigned res_a328761[ND_A328761 + 2][NP_A328761];
static uint64_t found_a328761;

/* A328578 value (capped at n+1) from residues r[i] = m mod prime(i+1), i < n */
static int eval_a328761(const unsigned *r, int n)
{
    unsigned v[NP_A328761];
    for (int i = 0; i < n; ++i) {
        unsigned pi = P_a328761[i];
        unsigned t = r[i];
        for (int j = 0; j < i; ++j)
            t = ((t + pi - v[j]) % pi) * inv_a328761[j][i] % pi;
        v[i] = t;
        if (t == 0) return i + 1;
    }
    return n + 1;
}

static bool dfs_a328761(int t, double curlog, uint64_t k, uint64_t weight_unused)
{
    (void)weight_unused;
    int n = n_a328761;
    const unsigned *res = res_a328761[t + 1];
    if (t == 0) {
        if (eval_a328761(res, n) == n) { found_a328761 = k; return true; }
        return false;
    }
    uint64_t w = 1;
    for (int u = 0; u < t; ++u) w *= P_a328761[u];
    unsigned base = P_a328761[t];
    unsigned *nr = res_a328761[t];
    for (unsigned d = 0; d < base; ++d) {
        double nl = curlog + (double)d * lg_a328761[t];
        if (nl + S_a328761[t] < thr_a328761 - 1e-9) continue;
        for (int i = 0; i < n; ++i)
            nr[i] = (res[i] * pw_a328761[i][t][d]) % P_a328761[i];
        if (dfs_a328761(t - 1, nl, k + (uint64_t)d * w, 0)) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    static bool inited = false;
    static bool have[NP_A328761 + 1];
    static uint64_t memo[NP_A328761 + 1];

    if (n == 1) return (seq_t)1;
    if (n == 0 || n > NP_A328761 - 2) return (seq_t)0;
    if (!inited) { init_a328761(); inited = true; }
    if (have[n]) return (seq_t)memo[n];

    n_a328761 = (int)n;
    thr_a328761 = 0.0;
    for (int t = 0; t + 2 < (int)n; ++t) thr_a328761 += lg_a328761[t];
    for (int i = 0; i < (int)n; ++i)
        res_a328761[ND_A328761][i] = 1 % P_a328761[i];
    found_a328761 = 0;
    dfs_a328761(ND_A328761 - 1, 0.0, 0, 0);
    have[n] = true;
    memo[n] = found_a328761;
    return (seq_t)memo[n];
}
