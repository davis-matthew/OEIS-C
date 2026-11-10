/*
 * A393070  Numbers k such that A276086(k) <= A003959(k).
 * https://oeis.org/A393070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. a(n) is the n-th k >= 1 with
 *            A276086(k) <= A003959(k). The search goes through the
 *            primorial-base digits of k, highest digit first, so the terms
 *            come out in increasing order. Because A003959(k) <= k^2, a
 *            branch is skipped once the partial A276086 product is larger
 *            than...
 * Range:     u8 n <= 29, u16 n <= 397, u32 n <= 20490, u64 n <= 70825,
 *            u128 n <= 70825, big any n
 * Validated: standard, pass, 78 terms through n = 9744 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 A393070_u128;

static const uint64_t A393070_PR[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
static uint64_t A393070_PRIM[16];

static uint64_t *A393070_terms = NULL;
static size_t A393070_cnt = 0, A393070_cap = 0;

/* A003959(k) = Product (p+1)^e, computed natively */
static A393070_u128 A393070_A003959(uint64_t n)
{
    A393070_u128 r = 1;
    uint64_t t = n;
    if ((t & 1) == 0) {
        while ((t & 1) == 0) { t >>= 1; r *= 3; }
    }
    for (uint64_t p = 3; p * p <= t; p += 2) {
        while (t % p == 0) { t /= p; r *= (A393070_u128)(p + 1); }
    }
    if (t > 1) r *= (A393070_u128)(t + 1);
    return r;
}

static void A393070_push(uint64_t k)
{
    if (A393070_cnt == A393070_cap) {
        A393070_cap = A393070_cap ? A393070_cap * 2 : 1024;
        A393070_terms = (uint64_t *)realloc(A393070_terms, A393070_cap * sizeof(uint64_t));
        if (!A393070_terms) abort();
    }
    A393070_terms[A393070_cnt++] = k;
}

/* i: current digit position; v: prefix value; q: A276086 of prefix */
static void A393070_dfs(int i, int top, uint64_t v, A393070_u128 q)
{
    if (i < 0) {
        if (q <= A393070_A003959(v)) A393070_push(v);
        return;
    }
    uint64_t p = A393070_PR[i];
    uint64_t P = A393070_PRIM[i];
    A393070_u128 nq = q;
    uint64_t d0 = (i == top) ? 1 : 0;
    if (d0 == 1) nq *= p;
    for (uint64_t d = d0; d < p; d++) {
        if (d > d0) nq *= p;
        uint64_t nv = v + d * P;
        A393070_u128 M = (A393070_u128)(nv + P - 1);
        if (nq > M * M) continue;
        A393070_dfs(i - 1, top, nv, nq);
    }
}

seq_t a(idx_t n)
{
    static int next_block = 0;
    static bool init = false;
    if (!init) {
        A393070_PRIM[0] = 1;
        for (int i = 1; i < 16; i++)
            A393070_PRIM[i] = A393070_PRIM[i - 1] * A393070_PR[i - 1];
        init = true;
    }
    while (A393070_cnt < (size_t)n && next_block < 14) {
        A393070_dfs(next_block, next_block, 0, 1);
        next_block++;
    }
    if ((size_t)n > A393070_cnt) return 0;
    return (seq_t)A393070_terms[(size_t)n - 1];
}
