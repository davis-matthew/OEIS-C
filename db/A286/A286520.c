/*
 * A286520  Number of finite connected sets of pairwise indivisible positive
 *          integers greater than one with least common multiple n.
 * https://oeis.org/A286520
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of finite connected sets of pairwise indivisible
 * positive integers >1 with least common multiple n.
 * Computation is by enumerating all non‑empty subsets of the set of
 * divisors >1 of n, checking that the set covers the prime factors of n,
 * contains only pairwise non‑divisible elements, and that the corresponding
 * graph is connected.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

#define MAX_K 6   /* maximum number of distinct primes for n ≤ 10000 */
#define MAX_D 128 /* maximum number of divisors >1 for n ≤ 10000 */

/* -------------------------------------------------------------------- */
/* Helper structures and global temporary storage                        */
/* -------------------------------------------------------------------- */
struct Divisor {
    uint64_t val;                 /* divisor value (>1)            */
    uint8_t exp[MAX_K];           /* exponent of each prime       */
    uint32_t pmask;               /* bitmask of present primes    */
    unsigned __int128 compMask;   /* mask of comparable divisors  */
};

static int g_k;                     /* number of distinct primes of current n */
static int g_e[MAX_K];               /* exponent of each prime in n */
static int g_d;                     /* number of divisors >1 for current n */
static struct Divisor g_div[MAX_D]; /* all divisors >1 */

/* adjacency lists for gcd>1 edges */
static int g_neiCnt[MAX_D];
static int g_nei[MAX_D][MAX_D];
static bool g_selFlag[MAX_D];
static int g_selIdx[MAX_D];
static int g_selCnt;

/* bitmask of primes already covered */
static uint32_t g_fullMask;

/* precomputed masks */
static uint32_t g_unionRem[MAX_D + 1];
static int g_suffixMax[MAX_D + 1][MAX_K];

/* current maximum exponents for each prime while exploring subsets */
static uint8_t g_curMax[MAX_K];

/* -------------------------------------------------------------------- */
/* Basic utilities                                                     */
/* -------------------------------------------------------------------- */

/* factorise n into primes and exponents (n ≤ 10000) */
static void factorise(uint64_t n, uint64_t *primes, int *exps, int *cnt)
{
    int c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        primes[c] = n;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* recursively generate all divisors >1 of n */
static void gen_divs_rec(int idx, uint64_t val,
                         uint8_t *curExp,
                         uint64_t *primes,
                         int *exps,
                         int k,
                         int *cnt)
{
    if (idx == k) {
        if (val > 1) {
            int d = *cnt;
            g_div[d].val = val;
            for (int i = 0; i < k; ++i)
                g_div[d].exp[i] = curExp[i];
            uint32_t pm = 0;
            for (int i = 0; i < k; ++i)
                if (curExp[i] > 0)
                    pm |= 1u << i;
            g_div[d].pmask = pm;
            ++(*cnt);
        }
        return;
    }

    /* precompute powers of primes[idx] */
    uint64_t pows[MAX_K + 1];   /* exponents from 0 to exps[idx] */
    pows[0] = 1;
    for (int e = 1; e <= exps[idx]; ++e)
        pows[e] = pows[e - 1] * primes[idx];

    for (int e = 0; e <= exps[idx]; ++e) {
        curExp[idx] = e;
        gen_divs_rec(idx + 1,
                     val * pows[e],
                     curExp,
                     primes,
                     exps,
                     k,
                     cnt);
    }
}

/* test whether divisor i divides divisor j (by exponents) */
static bool divides(const struct Divisor *i, const struct Divisor *j)
{
    for (int t = 0; t < g_k; ++t)
        if (i->exp[t] > j->exp[t])
            return false;
    return true;
}

/* -------------------------------------------------------------------- */
/* Recursive search for valid subsets (antichains that cover all primes) */
/* -------------------------------------------------------------------- */
static seq_t dfs_rec(uint32_t pos, unsigned __int128 prohibited,
                     uint32_t cover,
                     uint8_t curMax[])
{
    /* pruning: check that all primes can still be covered */
    if ((cover | g_unionRem[pos]) != g_fullMask)
        return 0;
    for (int i = 0; i < g_k; ++i)
        if (curMax[i] < g_e[i] && g_suffixMax[pos][i] < g_e[i])
            return 0;

    if (pos == (uint32_t)g_d) {
        if (g_selCnt == 0)
            return 0;                /* empty set not counted */
        for (int i = 0; i < g_k; ++i)
            if (curMax[i] != g_e[i])
                return 0;            /* not all prime exponents covered */
        /* connectivity check */
        if (g_selCnt == 1)
            return 1;
        bool vis[MAX_D] = {0};
        int q[MAX_D];
        int qf = 0, qb = 0;
        int start = g_selIdx[0];
        q[qb++] = start;
        vis[start] = true;
        int visited = 1;
        while (qf < qb) {
            int u = q[qf++];
            for (int vIdx = 0; vIdx < g_neiCnt[u]; ++vIdx) {
                int v = g_nei[u][vIdx];
                if (g_selFlag[v] && !vis[v]) {
                    vis[v] = true;
                    q[qb++] = v;
                    ++visited;
                }
            }
        }
        return (visited == g_selCnt) ? 1 : 0;
    }

    /* skip current divisor */
    seq_t cnt = dfs_rec(pos + 1, prohibited, cover, curMax);

    /* try selecting current divisor if not prohibited */
    if ((prohibited & ((unsigned __int128)1 << pos)) == 0) {
        uint32_t newCover = cover | g_div[pos].pmask;
        unsigned __int128 newProv = prohibited | g_div[pos].compMask;

        uint8_t oldMax[MAX_K];
        for (int i = 0; i < g_k; ++i)
            oldMax[i] = curMax[i];
        for (int i = 0; i < g_k; ++i)
            if (g_div[pos].exp[i] > curMax[i])
                curMax[i] = g_div[pos].exp[i];

        g_selFlag[pos] = true;
        g_selIdx[g_selCnt++] = pos;

        cnt += dfs_rec(pos + 1, newProv, newCover, curMax);

        /* revert */
        g_selCnt--;
        g_selFlag[pos] = false;
        for (int i = 0; i < g_k; ++i)
            curMax[i] = oldMax[i];
    }
    return cnt;
}

/* -------------------------------------------------------------------- */
/* Memoisation tables for n ≤ 10000                                     */
/* -------------------------------------------------------------------- */
static bool g_computed[10001];
static seq_t g_cache[10001];

/* -------------------------------------------------------------------- */
/* Main sequence function                                                */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0UL;              /* values below offset irrelevant */

    if (n <= 10000 && g_computed[n])
        return g_cache[n];

    /* factorise n */
    uint64_t primes[MAX_K];
    int exps[MAX_K];
    int kcnt;
    factorise((uint64_t)n, primes, exps, &kcnt);
    g_k = kcnt;
    for (int i = 0; i < kcnt; ++i)
        g_e[i] = exps[i];
    g_fullMask = (1u << g_k) - 1;

    /* generate all divisors >1 */
    int dcnt = 0;
    uint8_t curExp[MAX_K] = {0};
    gen_divs_rec(0, 1ULL, curExp, primes, exps, kcnt, &dcnt);
    g_d = dcnt;

    /* precompute comparability masks */
    for (int i = 0; i < g_d; ++i) {
        unsigned __int128 mask = (unsigned __int128)1 << i; /* include self */
        for (int j = 0; j < g_d; ++j) {
            if (i == j)
                continue;
            if (divides(&g_div[i], &g_div[j]) || divides(&g_div[j], &g_div[i]))
                mask |= (unsigned __int128)1 << j;
        }
        g_div[i].compMask = mask;
    }

    /* precompute edges for gcd>1 adjacency */
    for (int i = 0; i < g_d; ++i)
        g_neiCnt[i] = 0;
    for (int i = 0; i < g_d; ++i) {
        for (int j = i + 1; j < g_d; ++j) {
            if (gcd_u64(g_div[i].val, g_div[j].val) > 1) {
                g_nei[i][g_neiCnt[i]++] = j;
                g_nei[j][g_neiCnt[j]++] = i;
            }
        }
    }

    /* precompute unionRem */
    g_unionRem[g_d] = 0;
    for (int i = g_d - 1; i >= 0; --i)
        g_unionRem[i] = g_unionRem[i + 1] | g_div[i].pmask;

    /* precompute suffixMax exponents */
    for (int i = g_k; i-- > 0;)           /* initialise last row */
        g_suffixMax[g_d][i] = 0;
    for (int i = g_d - 1; i >= 0; --i) {
        for (int t = 0; t < g_k; ++t) {
            int val = g_div[i].exp[t];
            g_suffixMax[i][t] = val > g_suffixMax[i + 1][t] ? val
                                                             : g_suffixMax[i + 1][t];
        }
    }

    /* initialise selection flags and counters */
    for (int i = 0; i < g_d; ++i)
        g_selFlag[i] = false;
    g_selCnt = 0;
    for (int i = 0; i < g_k; ++i)
        g_curMax[i] = 0;

    /* enumerate valid subsets */
    seq_t result = dfs_rec(0, 0, 0, g_curMax);

    if (n <= 10000) {
        g_cache[n] = result;
        g_computed[n] = true;
    }
    return result;
}
