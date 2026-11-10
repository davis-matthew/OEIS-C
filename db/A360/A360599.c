/*
 * A360599  Ratios of consecutive terms of A360598: a(n) = max(A360598(n),
 *          A360598(n+1)) / min(A360598(n), A360598(n+1)).
 * https://oeis.org/A360599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(b(n),b(n+1))/min(b(n),b(n+1)) where b = A360598
 *            (b(1)=1), the lexicographically earliest positive-integer
 *            sequence with all these ratios integers and distinct; computed
 *            by greedy search, and a(n) is the n-th ratio chosen
 * Range:     u8 n <= 24, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A360599_ratios = NULL;
static size_t A360599_len = 0;
static size_t A360599_cap = 0;
static seq_t A360599_prev = 1;
static seq_t A360599_mex = 1;

static bool A360599_used(seq_t r)
{
    for (size_t i = 0; i < A360599_len; ++i)
        if (A360599_ratios[i] == r) return true;
    return false;
}

static void A360599_push(seq_t r)
{
    if (A360599_len == A360599_cap)
    {
        size_t nc = A360599_cap ? A360599_cap * 2 : 16;
        seq_t *t = mem_resize(seq_t, A360599_ratios, nc);
        if (!t) exit(1);
        A360599_ratios = t;
        A360599_cap = nc;
    }
    A360599_ratios[A360599_len++] = r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A360599_len < (size_t)n)
    {
        seq_t prev = A360599_prev;
        bool found = false;
        seq_t cand = 1, r = 1;
        /* divisors of prev (smaller than prev) first: they give the smallest candidates */
        for (seq_t c = 1; c < prev; ++c)
        {
            if (prev % c != 0) continue;
            seq_t q = prev / c;
            if (!A360599_used(q))
            {
                cand = c; r = q; found = true;
                break;
            }
        }
        if (!found)
        {
            while (A360599_used(A360599_mex)) ++A360599_mex;
            r = A360599_mex;
            cand = prev * r;
        }
        A360599_push(r);
        A360599_prev = cand;
    }
    return A360599_ratios[(size_t)n - 1];
}
