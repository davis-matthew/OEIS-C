/*
 * A289273  Complement of A242410.
 * https://oeis.org/A289273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not in A242410; computed by
 *            generating A242410 (a(1)=1, a(k)=least number > a(k-1) not
 *            divisible by a(d) for proper divisors d of k, 1<d<k) and
 *            collecting the gaps
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9859 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *cm_T = NULL;   /* A242410 terms, cm_T[k-1] = a(k) */
static size_t cm_m = 0, cm_tcap = 0;
static seq_t *cm_C = NULL;   /* complement terms */
static size_t cm_cnt = 0, cm_ccap = 0;

static void cm_push_T(seq_t v)
{
    if (cm_m == cm_tcap) {
        size_t nc = cm_tcap ? cm_tcap * 2 : 1024;
        cm_T = mem_resize(seq_t, cm_T, nc);
        cm_tcap = nc;
    }
    cm_T[cm_m++] = v;
}

static void cm_push_C(seq_t v)
{
    if (cm_cnt == cm_ccap) {
        size_t nc = cm_ccap ? cm_ccap * 2 : 1024;
        cm_C = mem_resize(seq_t, cm_C, nc);
        cm_ccap = nc;
    }
    cm_C[cm_cnt++] = v;
}

static bool cm_ok(size_t k, seq_t cand)
{
    for (size_t i = 1; i * i <= k; ++i) {
        if (k % i != 0) continue;
        size_t j = k / i;
        if (i >= 2 && i < k && cand % cm_T[i - 1] == (seq_t)0) return false;
        if (j != i && j >= 2 && j < k && cand % cm_T[j - 1] == (seq_t)0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (cm_m == 0) cm_push_T((seq_t)1);
    while (cm_cnt < (size_t)n) {
        size_t k = cm_m + 1;
        seq_t prev = cm_T[cm_m - 1];
        seq_t cand = prev + 1;
        while (!cm_ok(k, cand)) ++cand;
        for (seq_t v = prev + 1; v < cand; ++v) cm_push_C(v);
        cm_push_T(cand);
    }
    return cm_C[(size_t)n - 1];
}
