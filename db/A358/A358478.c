/*
 * A358478  Number of 5's that appeared in the n-th step when constructing
 *          A030717.
 * https://oeis.org/A358478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n >= 2, a(n) = number of 5's among the terms
 *            appended to the first list of A030717 in its (n-1)-th step. This
 *            is computed by direct simulation
 * Range:     u8 n <= 32, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A358478_cnt = NULL;   /* cnt[v] = frequency of value v in the first list */
static size_t A358478_cap = 0;
static size_t A358478_maxv = 0;
static seq_t *A358478_res = NULL;   /* res[k] = number of 5's appended in step k+1 */
static size_t A358478_nres = 0;
static size_t A358478_rescap = 0;
static seq_t *A358478_snap = NULL;
static size_t A358478_snapcap = 0;
static bool A358478_init = false;

static void A358478_grow_cnt(size_t need)
{
    if (need < A358478_cap) return;
    size_t nc = A358478_cap;
    while (nc <= need) nc <<= 1;
    A358478_cnt = mem_resize(seq_t, A358478_cnt, nc);
    for (size_t i = A358478_cap; i < nc; ++i) A358478_cnt[i] = (seq_t)0;
    A358478_cap = nc;
}

static void A358478_step(void)
{
    size_t k = 0;
    if (A358478_snapcap < A358478_maxv + 1) {
        A358478_snapcap = (A358478_maxv + 1) * 2;
        A358478_snap = mem_resize(seq_t, A358478_snap, A358478_snapcap);
    }
    for (size_t v = 1; v <= A358478_maxv; ++v) {
        if (A358478_cnt[v] > (seq_t)0) A358478_snap[k++] = A358478_cnt[v];
    }
    seq_t c5 = (seq_t)0;
    for (size_t i = 0; i < k; ++i) {
        size_t f = (size_t)(long long)A358478_snap[i];
        A358478_grow_cnt(f + 1);
        A358478_cnt[f] += (seq_t)1;
        if (f > A358478_maxv) A358478_maxv = f;
        if (f == 5) c5 += (seq_t)1;
    }
    if (A358478_nres >= A358478_rescap) {
        size_t nc = A358478_rescap * 2;
        A358478_res = mem_resize(seq_t, A358478_res, nc);
        for (size_t i = A358478_rescap; i < nc; ++i) A358478_res[i] = (seq_t)0;
        A358478_rescap = nc;
    }
    A358478_res[A358478_nres++] = c5;
}

seq_t a(idx_t n)
{
    if (!A358478_init) {
        A358478_cap = 64;
        A358478_cnt = mem_new(seq_t, A358478_cap);
        A358478_cnt[1] = (seq_t)1;
        A358478_maxv = 1;
        A358478_rescap = 256;
        A358478_res = mem_new(seq_t, A358478_rescap);
        A358478_snapcap = 64;
        A358478_snap = mem_new(seq_t, A358478_snapcap);
        A358478_init = true;
    }
    if (n <= 1) return (seq_t)0;
    size_t steps = (size_t)(n - 1);
    while (A358478_nres < steps) A358478_step();
    return A358478_res[steps - 1];
}
