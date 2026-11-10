/*
 * A351228  Numbers k for which A003415(k) >= A276086(k), where A003415 is the
 *          arithmetic derivative and A276086 is the primorial base
 *          exp-function.
 * https://oeis.org/A351228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search, in increasing order, for k with A003415(k) >=
 *            A276086(k). k is built digit by digit in primorial base. A
 *            branch is dropped once the partial product prod p_i^d_i exceeds
 *            the bound k*floor(log2 k)/2, which A003415(k) can never pass
 * Range:     u8 n <= 11, u16 n <= 211, u32 n <= 11643, u64 n <= 11643,
 *            u128 n <= 11643, big any n
 * Validated: standard, pass, 72 terms through n = 9305 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 s228_u128;

static const uint64_t s228_pr[15] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
static uint64_t s228_prim[16];      /* s228_prim[i] = product of first i primes */
static bool s228_init = false;

static uint64_t *s228_terms = NULL;
static size_t s228_cnt = 0, s228_cap = 0;
static int s228_block = 0;          /* next block [P_j, P_{j+1}) to scan */

static int s228_log2(uint64_t x)
{
    int r = -1;
    while (x) { x >>= 1; r++; }
    return r;
}

/* A003415(k), exact in 128 bits */
static s228_u128 s228_der(uint64_t k)
{
    if (k < 2) return 0;
    s228_u128 res = 0;
    uint64_t rem = k;
    for (uint64_t p = 2; p * p <= rem; p += (p == 2 ? 1 : 2)) {
        unsigned e = 0;
        while (rem % p == 0) { e++; rem /= p; }
        if (e) res += (s228_u128)(k / p) * e;
    }
    if (rem > 1) res += k / rem;
    return res;
}

static void s228_push(uint64_t k)
{
    if (s228_cnt == s228_cap) {
        s228_cap = s228_cap ? s228_cap * 2 : 1024;
        s228_terms = (uint64_t *)realloc(s228_terms, s228_cap * sizeof(uint64_t));
        if (!s228_terms) abort();
    }
    s228_terms[s228_cnt++] = k;
}

/* Upper bound on A003415(k) for every k <= kmax */
static s228_u128 s228_bound(uint64_t kmax)
{
    if (kmax < 2) return 0;
    return (s228_u128)kmax * (uint64_t)s228_log2(kmax) / 2;
}

static void s228_dfs(int pos, uint64_t prefix, s228_u128 v, bool top)
{
    if (pos < 0) {
        if (prefix >= 1 && s228_der(prefix) >= v) s228_push(prefix);
        return;
    }
    const s228_u128 cap = ((s228_u128)1) << 100;
    uint64_t p = s228_pr[pos];
    uint64_t P = s228_prim[pos];
    s228_u128 nv = v;
    for (uint64_t d = 0; d < p; d++) {
        if (d > 0) { nv *= p; if (nv > cap) nv = cap; }
        if (top && d == 0) continue;
        uint64_t np = prefix + d * P;
        uint64_t kmax = np + (P - 1);
        if (nv > s228_bound(kmax)) continue;
        s228_dfs(pos - 1, np, nv, false);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (!s228_init) {
        s228_prim[0] = 1;
        for (int i = 1; i <= 15; i++) s228_prim[i] = s228_prim[i - 1] * s228_pr[i - 1];
        s228_init = true;
    }
    while (s228_cnt < (size_t)n && s228_block < 15) {
        s228_dfs(s228_block, 0, 1, true);
        s228_block++;
    }
    if (s228_cnt < (size_t)n) return (seq_t)0;
    return (seq_t)s228_terms[(size_t)n - 1];
}
