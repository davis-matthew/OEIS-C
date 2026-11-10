/*
 * A339973  Numbers k for which A019565(2k)-1 is a multiple of
 *          A000265(phi(A019565(2k))).
 * https://oeis.org/A339973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) lists the k for which S = { prime(i+2) : bit i of k
 *            is set } * satisfies prod(S) == 1 (mod prod_{p in S}
 *            A000265(p-1)), sorted ascending. * The search enumerates sets
 *            with highest bit t = 0, 1, 2, ... in turn, so terms * come out
 *            in increasing order. It prunes any set in which one ch...
 * Range:     u8 n <= 19, u16 n <= 37, u32 n <= 93, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Validated: standard, pass, 70 terms through n = 93 (2026-11-10)
 * Helpers:   mulmod_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mulmod_u128.h"

#define NP 56

typedef unsigned __int128 u128_t;

static uint32_t P[NP];          /* odd primes 3,5,7,... */
static uint64_t conf[NP];       /* conflict masks */
static uint8_t fr[NP][8], fe[NP][8], fc[NP]; /* factorisation of oddpart(P[t]-1) */
static int exps[NP];
static int chosen[NP], nch = 0;
static bool inited = false;

static uint64_t *g_list = NULL;
static size_t g_cnt = 0, g_cap = 0;
static int g_t = 0;             /* next top bit to process */

static void push_term(uint64_t k)
{
    if (g_cnt == g_cap) {
        g_cap = g_cap ? g_cap * 2 : 256;
        g_list = (uint64_t *)realloc(g_list, g_cap * sizeof(uint64_t));
        if (!g_list) abort();
    }
    g_list[g_cnt++] = k;
}

static void init(void)
{
    int c = 0;
    uint32_t x = 3;
    while (c < NP) {
        bool pr = true;
        for (uint32_t d = 3; d * d <= x; d += 2)
            if (x % d == 0) { pr = false; break; }
        if (pr) P[c++] = x;
        x += 2;
    }
    for (int t = 0; t < NP; ++t) {
        uint32_t v = P[t] - 1;
        while ((v & 1) == 0) v >>= 1;
        fc[t] = 0;
        for (int i = 0; i < t && v > 1; ++i) {
            if (v % P[i] == 0) {
                int e = 0;
                while (v % P[i] == 0) { v /= P[i]; ++e; }
                fr[t][fc[t]] = (uint8_t)i;
                fe[t][fc[t]] = (uint8_t)e;
                fc[t]++;
            }
        }
        conf[t] = 0;
    }
    for (int s = 0; s < NP; ++s)
        for (int t = 0; t < NP; ++t)
            if (s != t && ((P[t] - 1) % P[s] == 0 || (P[s] - 1) % P[t] == 0))
                conf[s] |= 1ULL << t;
    inited = true;
}

static void add_idx(int t, int sign)
{
    for (int j = 0; j < fc[t]; ++j) exps[fr[t][j]] += sign * fe[t][j];
    if (sign > 0) chosen[nch++] = t; else --nch;
}

static bool check(int top)
{
    for (int i = 0; i < top; ++i) {
        if (exps[i] == 0) continue;
        u128_t mod = 1;
        for (int e = 0; e < exps[i]; ++e) {
            if (mod > ((~(u128_t)0) / P[i])) return false; /* practically unreachable */
            mod *= P[i];
        }
        bool small = (mod >> 64) == 0;
        u128_t m = 1;
        for (int c = 0; c < nch; ++c) {
            if (small) m = (m * P[chosen[c]]) % mod;
            else m = mulmod_u128(m, (u128_t)P[chosen[c]], mod);
        }
        if (m != 1) return false;
    }
    return true;
}

static void dfs(int below, uint64_t mask, int top)
{
    if (check(top)) push_term(mask);
    for (int s = below - 1; s >= 0; --s) {
        if (conf[s] & mask) continue;
        add_idx(s, 1);
        dfs(s, mask | (1ULL << s), top);
        add_idx(s, -1);
    }
}

static int cmp_u64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return x < y ? -1 : (x > y);
}

seq_t a(idx_t n)
{
    if (!inited) {
        init();
        push_term(0); /* empty set: k = 0 */
    }
    while (g_cnt < (size_t)n && g_t < NP) {
        int t = g_t;
        size_t start = g_cnt;
        add_idx(t, 1);
        dfs(t, 1ULL << t, t + 1);
        add_idx(t, -1);
        qsort(g_list + start, g_cnt - start, sizeof(uint64_t), cmp_u64);
        ++g_t;
    }
    if (n == 0 || (size_t)n > g_cnt) return (seq_t)0;
    return (seq_t)g_list[n - 1];
}
