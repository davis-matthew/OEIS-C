/*
 * A391248  Odd composites k such that for all divisor pairs (d, k/d), d <=
 *          k/d, it holds that k = A048720(k/d, A065621(d)).
 * https://oeis.org/A391248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search. a(n) is the n-th odd composite k such that A048720(k/d,
 *            A065621(d)) = k for every divisor d of k with d <= k/d. For each
 *            odd prime d (the smallest prime factor of k), a bitwise DFS
 *            finds every m >= d with A048720(m, A065621(d)) = d*m. The low t
 *            bits of both sides depend only on the low t...
 * Range:     u8 n <= 7, u16 n <= 158, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 8255 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t g_clmul(uint64_t i, uint64_t j)
{
    uint64_t r = 0;
    while (i) {
        if (i & 1) r ^= j;
        j <<= 1;
        i >>= 1;
    }
    return r;
}

static uint64_t g_a065621(uint64_t d)
{
    uint64_t low = d & (~d + 1);
    return d ^ ((d - low) << 1);
}

static uint64_t *g_terms = 0;
static uint64_t g_count = 0, g_tcap = 0;
static uint64_t g_limit = 0;

static uint64_t *g_primes = 0;
static uint64_t g_np = 0;

static uint64_t g_d, g_r, g_M;

static void g_push(uint64_t k)
{
    if (g_count == g_tcap) {
        g_tcap = g_tcap ? g_tcap * 2 : 1024;
        g_terms = (uint64_t *)realloc(g_terms, (size_t)g_tcap * sizeof(uint64_t));
    }
    g_terms[g_count++] = k;
}

/* full check of k: smallest prime factor must be g_d, all pairs hold */
static void g_verify(uint64_t k)
{
    uint64_t pr[64], ex[64];
    int c = 0, i;
    uint64_t x = k;
    uint64_t pi;
    for (pi = 0; pi < g_np; pi++) {
        uint64_t p = g_primes[pi];
        if (p * p > x) break;
        if (x % p == 0) {
            if (c == 0 && p != g_d) return;
            pr[c] = p; ex[c] = 0;
            while (x % p == 0) { x /= p; ex[c]++; }
            c++;
        }
    }
    if (x > 1) {
        if (c == 0 && x != g_d) return;
        pr[c] = x; ex[c] = 1; c++;
    }
    /* enumerate divisors */
    {
        uint64_t divs[4096];
        int nd = 1, j;
        uint64_t e;
        divs[0] = 1;
        for (i = 0; i < c; i++) {
            int cur = nd;
            uint64_t pw = 1;
            for (e = 1; e <= ex[i]; e++) {
                pw *= pr[i];
                for (j = 0; j < cur; j++) {
                    if (nd >= 4096) return;
                    divs[nd++] = divs[j] * pw;
                }
            }
        }
        for (j = 0; j < nd; j++) {
            uint64_t dd = divs[j];
            if (dd == 1 || dd > k / dd) continue;
            if (g_clmul(k / dd, g_a065621(dd)) != k) return;
        }
    }
    g_push(k);
}

static void g_dfs(uint64_t m, unsigned t)
{
    uint64_t mask, m2;
    if (t >= 63) return;
    if ((1ULL << t) > g_M) return;
    mask = (2ULL << t) - 1;
    if ((((g_d * m) ^ g_clmul(m, g_r)) & mask) == 0)
        g_dfs(m, t + 1);
    m2 = m | (1ULL << t);
    if (m2 <= g_M && (((g_d * m2) ^ g_clmul(m2, g_r)) & mask) == 0) {
        if (m2 >= g_d && g_clmul(m2, g_r) == g_d * m2)
            g_verify(g_d * m2);
        g_dfs(m2, t + 1);
    }
}

static int g_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void g_build(uint64_t N)
{
    uint64_t s = 1, i, j;
    unsigned char *comp;
    while ((s + 1) * (s + 1) <= N) s++;
    comp = (unsigned char *)calloc((size_t)(s + 2), 1);
    free(g_primes);
    g_primes = (uint64_t *)malloc((size_t)(s + 2) * sizeof(uint64_t));
    g_np = 0;
    for (i = 2; i <= s; i++) {
        if (comp[i]) continue;
        g_primes[g_np++] = i;
        for (j = i * i; j <= s; j += i) comp[j] = 1;
    }
    free(comp);

    g_count = 0;
    for (i = 0; i < g_np; i++) {
        uint64_t d = g_primes[i];
        if (d == 2) continue;
        if (d * d > N) break;
        g_d = d;
        g_r = g_a065621(d);
        g_M = N / d;
        g_dfs(1, 1);
    }
    if (g_count) qsort(g_terms, (size_t)g_count, sizeof(uint64_t), g_cmp);
    g_limit = N;
}

seq_t a(idx_t n)
{
    if (g_limit == 0) g_build(1ULL << 20);
    while (g_count < (uint64_t)n) g_build(g_limit * 2);
    return (seq_t)g_terms[n - 1];
}
