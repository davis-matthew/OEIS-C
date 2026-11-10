/*
 * A358469  Number of 2's that appeared in the n-th step when constructing
 *          A030717.
 * https://oeis.org/A358469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2's among the terms appended in step n-1 of the
 *            A030717 construction (step 0 is the initial list [1], so
 *            a(1)=0); computed by direct simulation
 * Range:     u8 n <= 34, u16 n <= 290, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Validated: standard, pass, 111 terms through n = 763 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A358469_keys = NULL;   /* distinct values in first list, ascending */
static seq_t *A358469_cnts = NULL;   /* their frequencies */
static seq_t *A358469_snap = NULL;   /* snapshot buffer */
static size_t A358469_d = 0, A358469_cap = 0;
static seq_t *A358469_res = NULL;    /* res[k] = twos in step k */
static size_t A358469_rlen = 0, A358469_rcap = 0;

static void A358469_add(seq_t v)
{
    size_t lo = 0, hi = A358469_d;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (A358469_keys[mid] < v) lo = mid + 1; else hi = mid;
    }
    if (lo < A358469_d && A358469_keys[lo] == v) {
        A358469_cnts[lo] += 1;
        return;
    }
    if (A358469_d + 1 > A358469_cap) {
        size_t nc = A358469_cap * 2;
        A358469_keys = mem_resize(seq_t, A358469_keys, nc);
        A358469_cnts = mem_resize(seq_t, A358469_cnts, nc);
        A358469_snap = mem_resize(seq_t, A358469_snap, nc);
        A358469_cap = nc;
    }
    for (size_t j = A358469_d; j > lo; --j) {
        A358469_keys[j] = A358469_keys[j - 1];
        A358469_cnts[j] = A358469_cnts[j - 1];
    }
    A358469_keys[lo] = v;
    A358469_cnts[lo] = 1;
    A358469_d++;
}

seq_t a(idx_t n)
{
    if (A358469_cap == 0) {
        A358469_cap = 64;
        A358469_keys = mem_new(seq_t, A358469_cap);
        A358469_cnts = mem_new(seq_t, A358469_cap);
        A358469_snap = mem_new(seq_t, A358469_cap);
        A358469_rcap = 1024;
        A358469_res = mem_new(seq_t, A358469_rcap);
        A358469_keys[0] = (seq_t)1;
        A358469_cnts[0] = (seq_t)1;
        A358469_d = 1;
        A358469_res[0] = (seq_t)0;   /* step 0 */
        A358469_rlen = 1;
    }
    size_t need = (size_t)n;   /* need steps 0..n-1 */
    while (A358469_rlen < need) {
        size_t d = A358469_d;
        seq_t twos = 0;
        for (size_t i = 0; i < d; ++i) A358469_snap[i] = A358469_cnts[i];
        for (size_t i = 0; i < d; ++i) {
            seq_t v = A358469_snap[i];
            if (v == 2) twos++;
            A358469_add(v);
        }
        if (A358469_rlen + 1 > A358469_rcap) {
            A358469_rcap *= 2;
            A358469_res = mem_resize(seq_t, A358469_res, A358469_rcap);
        }
        A358469_res[A358469_rlen++] = twos;
    }
    return A358469_res[need - 1];
}
