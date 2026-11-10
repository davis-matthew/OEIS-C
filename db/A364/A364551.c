/*
 * A364551  Odd numbers k such that k is a multiple of A005941(k).
 * https://oeis.org/A364551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search. a(n) = n-th odd k such that A005941(k) = 1 + Sum_j
 *            2^(i_j - 1 + j - 1) divides k. Since A005941(k) <= k, every
 *            prime index i and the count of prime factors Omega satisfy
 *            2^(i-1+Omega-1) < k. So for a bound B we run a depth-first
 *            search over odd numbers built from small primes with nondecr...
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const uint64_t A364551_primes[64] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,
    179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,
    283,293,307,311
};

static uint64_t *g_list = NULL;
static size_t g_cnt = 0, g_lcap = 0;
static uint64_t g_B = 0;

static void push_val(uint64_t v) {
    if (g_cnt == g_lcap) {
        g_lcap = g_lcap ? g_lcap * 2 : 64;
        g_list = (uint64_t *)realloc(g_list, g_lcap * sizeof(uint64_t));
    }
    g_list[g_cnt++] = v;
}

/* k: current odd product, s: A156552(k), j: number of prime factors, mi: min prime index */
static void dfs(uint64_t k, uint64_t s, uint64_t j, uint64_t mi, uint64_t B) {
    if (k % (s + 1) == 0) push_val(k);
    for (uint64_t i = mi; i <= 64; i++) {
        uint64_t p = A364551_primes[i - 1];
        if (k > B / p) break;
        uint64_t e = i - 1 + j;
        if (e >= 62) break;
        uint64_t t = s + ((uint64_t)1 << e);
        if (t + 1 > B) break;
        dfs(k * p, t, j + 1, i, B);
    }
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void build(uint64_t B) {
    g_cnt = 0;
    dfs(1, 0, 0, 2, B);
    qsort(g_list, g_cnt, sizeof(uint64_t), cmp_u64);
    g_B = B;
}

seq_t a(idx_t n) {
    if (g_B == 0) build((uint64_t)1 << 24);
    while (g_cnt < n && g_B < ((uint64_t)1 << 61)) build(g_B * 4);
    return (seq_t)g_list[n - 1];
}
