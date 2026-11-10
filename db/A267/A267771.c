/*
 * A267771  "Late birds" (a(n) < a(k) for all k > n) in A267760 (last digit of
 *          a(n) + first digit of a(n+1) occurs as substring in a(n+1)).
 * https://oeis.org/A267771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th term of A267760 (starting at its a(0)=0) that is
 *            smaller than all later terms; computed by generating A267760 and
 *            recording each term equal to the least value not yet used
 * Range:     u8 n <= 71, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 88 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "../utils/mem.h"

static bool *A267771_used = NULL;
static size_t A267771_cap = 0;
static uint64_t A267771_mex = 0;
static int A267771_last = 0;
static bool A267771_init = false;
static uint64_t *A267771_lb = NULL;
static size_t A267771_cnt = 0, A267771_lbcap = 0;

static void A267771_ensure(uint64_t v)
{
    if (v >= A267771_cap) {
        size_t nc = A267771_cap ? A267771_cap * 2 : 1024;
        while (nc <= v) nc <<= 1;
        bool *t = (bool *)realloc(A267771_used, nc * sizeof(bool));
        if (!t) exit(1);
        memset(t + A267771_cap, 0, (nc - A267771_cap) * sizeof(bool));
        A267771_used = t;
        A267771_cap = nc;
    }
}

static bool A267771_contains(uint64_t cand, int sum)
{
    char a[32], b[8];
    snprintf(a, sizeof a, "%llu", (unsigned long long)cand);
    snprintf(b, sizeof b, "%d", sum);
    return strstr(a, b) != NULL;
}

static int A267771_msd(uint64_t x)
{
    while (x >= 10) x /= 10;
    return (int)x;
}

static void A267771_record(uint64_t v)
{
    if (A267771_cnt >= A267771_lbcap) {
        size_t nc = A267771_lbcap ? A267771_lbcap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(A267771_lb, nc * sizeof(uint64_t));
        if (!t) exit(1);
        A267771_lb = t;
        A267771_lbcap = nc;
    }
    A267771_lb[A267771_cnt++] = v;
}

static void A267771_step(void)
{
    uint64_t cand = A267771_mex;
    for (;; ++cand) {
        A267771_ensure(cand);
        if (A267771_used[cand]) continue;
        int sum = A267771_msd(cand) + A267771_last;
        if (A267771_contains(cand, sum)) break;
    }
    A267771_used[cand] = true;
    if (cand == A267771_mex) {
        A267771_record(cand);
        while (1) {
            A267771_ensure(A267771_mex);
            if (!A267771_used[A267771_mex]) break;
            A267771_mex++;
        }
    }
    A267771_last = (int)(cand % 10);
}

seq_t a(idx_t n)
{
    if (!A267771_init) {
        A267771_init = true;
        A267771_ensure(16);
        A267771_used[0] = true;
        A267771_record(0);
        A267771_mex = 1;
        A267771_last = 0;
    }
    while (A267771_cnt < (size_t)n) A267771_step();
    return (seq_t)A267771_lb[n - 1];
}
