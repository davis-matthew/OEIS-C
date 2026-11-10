/*
 * A382168  Number of triples (i,j,k), 1 <= i < j < k <= n such that
 *          A019444(i) < A019444(k) < A019444(j).
 * https://oeis.org/A382168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} #{(i,j): i<j<k, A019444(i) < A019444(k) <
 *            A019444(j)}, with A019444 the greedy integer-average
 *            permutation; computed by O(n^2) counting
 * Range:     u8 n <= 20, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static size_t A019444_max_computed = 0;
static seq_t *A019444_a_vals = NULL;
static size_t A019444_a_cap = 0;
static seq_t A019444_current_s;
static bool *A019444_used_vals = NULL;
static size_t A019444_used_cap = 0;

static void A019444_ensure_a_cap(idx_t n)
{
    size_t new_cap = (size_t)n + 1;
    if (new_cap > A019444_a_cap) {
        seq_t *tmp = mem_resize(seq_t, A019444_a_vals, new_cap);
        if (!tmp) abort();
        A019444_a_vals = tmp;
        A019444_a_cap = new_cap;
    }
}

static void A019444_ensure_used_cap(size_t min_idx)
{
    if (min_idx < A019444_used_cap) return;
    size_t old = A019444_used_cap;
    size_t new_cap = (A019444_used_cap == 0) ? 8 : A019444_used_cap;
    while (new_cap <= min_idx) new_cap <<= 1;
    bool *tmp = (bool*)realloc(A019444_used_vals, new_cap * sizeof(bool));
    if (!tmp) abort();
    for (size_t i = old; i < new_cap; ++i) tmp[i] = false;
    A019444_used_vals = tmp;
    A019444_used_cap = new_cap;
}

static seq_t A019444_a(idx_t n)
{
    if (n == 0) abort();
    if (A019444_max_computed == 0) {
        A019444_ensure_a_cap(1);
        A019444_ensure_used_cap(1);
        A019444_a_vals[1] = (seq_t)1;
        A019444_used_vals[(size_t)1] = true;
        A019444_current_s = (seq_t)1;
        A019444_max_computed = 1;
    }
    if ((size_t)n <= A019444_max_computed) return A019444_a_vals[(size_t)n];
    for (idx_t cur = A019444_max_computed; cur < n; ++cur) {
        idx_t next = cur + 1;
        A019444_ensure_a_cap(next);
        size_t cs_idx = (size_t)A019444_current_s;
        if (cs_idx >= A019444_used_cap) A019444_ensure_used_cap(cs_idx);
        seq_t cand;
        if (A019444_used_vals[cs_idx]) {
            cand = A019444_current_s + (seq_t)next;
            A019444_current_s = A019444_current_s + (seq_t)1;
        } else {
            cand = A019444_current_s;
        }
        size_t cand_idx = (size_t)cand;
        if (cand_idx >= A019444_used_cap) A019444_ensure_used_cap(cand_idx);
        A019444_a_vals[next] = cand;
        A019444_used_vals[cand_idx] = true;
    }
    A019444_max_computed = (size_t)n;
    return A019444_a_vals[(size_t)n];
}

static seq_t *res_tab = NULL;
static size_t res_done = 0;
static size_t res_cap = 0;

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (res_done == 0) {
        res_tab = mem_resize(seq_t, res_tab, 2);
        if (!res_tab) abort();
        res_cap = 2;
        res_tab[0] = (seq_t)0;
        res_done = 0;
        res_tab[0] = (seq_t)0;
        res_done = 1; /* res_tab[0..res_done-1] valid; index = n */
    }
    if (N >= res_done) {
        if (N + 1 > res_cap) {
            size_t nc = N + 1;
            seq_t *t = mem_resize(seq_t, res_tab, nc);
            if (!t) abort();
            res_tab = t;
            res_cap = nc;
        }
        A019444_a((idx_t)N);
        for (size_t k = res_done; k <= N; k++) {
            seq_t vk = A019444_a((idx_t)k);
            seq_t c = 0;      /* number of j in (i,k) with a_j > a_k */
            seq_t cnt = 0;
            for (size_t i = k - 1; i >= 1; i--) {
                seq_t vi = A019444_a_vals[i];
                if (vi < vk) cnt += c;
                else c++;
            }
            res_tab[k] = res_tab[k - 1] + cnt;
        }
        res_done = N + 1;
    }
    return res_tab[N];
}
