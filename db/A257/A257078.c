/*
 * A257078  Smallest number m such that A248756(m) = n.
 * https://oeis.org/A257078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m with A248756(m) = n, where A248756(j) = j - (last
 *            i<j with popcount(A248756(i)) = popcount(j)), or j if none;
 *            found by incremental search, recording the first m at which each
 *            value appears
 * Range:     u8 n <= 20, u16 n <= 529, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 900 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static idx_t *A257078_first = NULL;   /* first[v] = smallest m with A248756(m)=v, 0 if unseen */
static idx_t  A257078_cap = 0;
static idx_t  A257078_m = 0;
static idx_t  A257078_last[65];

static unsigned A257078_pc(uint64_t x)
{
    unsigned c = 0;
    while (x) { x &= (x - 1); ++c; }
    return c;
}

static int A257078_ensure(idx_t need)
{
    if (A257078_cap > need) return 1;
    idx_t nc = A257078_cap ? A257078_cap : 1024;
    while (nc <= need) nc *= 2;
    idx_t *t = mem_new(idx_t, nc);
    if (!t) return 0;
    for (idx_t i = 0; i < A257078_cap; ++i) t[i] = A257078_first[i];
    if (A257078_first) mem_free(A257078_first);
    A257078_first = t;
    A257078_cap = nc;
    return 1;
}

seq_t a(idx_t n)
{
    if (!A257078_ensure(n)) return 0;
    while (A257078_first[n] == 0) {
        idx_t j = A257078_m + 1;
        if (!A257078_ensure(j)) return 0;
        unsigned pj = A257078_pc((uint64_t)j);
        idx_t prev = A257078_last[pj];
        uint64_t val = prev == 0 ? (uint64_t)j : (uint64_t)(j - prev);
        if (A257078_first[val] == 0) A257078_first[val] = j;
        A257078_last[A257078_pc(val)] = j;
        A257078_m = j;
    }
    return (seq_t)A257078_first[n];
}
