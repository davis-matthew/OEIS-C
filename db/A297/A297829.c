/*
 * A297829  Duplicates in A297826.
 * https://oeis.org/A297829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th distinct value occurring more than once in A297826
 *            (A297826: a(0)=1, a(1)=2, b(0)=3, b(1)=4,
 *            a(n)=2*b(n-1)-b(n-2)+n, b = complement of a); computed by
 *            simulating A297826 with a sorted multiset and scanning for
 *            repeated values
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *dd_s = NULL;   /* sorted multiset of A297826 terms */
static size_t dd_sn = 0, dd_scap = 0;
static seq_t *dd_b = NULL;   /* complement terms */
static size_t dd_bn = 0, dd_bcap = 0;
static seq_t *dd_d = NULL;   /* duplicates found (final) */
static size_t dd_dn = 0, dd_dcap = 0;
static size_t dd_scan = 0;
static size_t dd_m = 0;      /* next index of A297826 to generate */
static seq_t dd_cand = 1;
static bool dd_init = false;

static void dd_grow(seq_t **p, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t nc = *cap ? *cap * 2 : 16;
        if (nc < need) nc = need;
        *p = mem_resize(seq_t, *p, nc);
        *cap = nc;
    }
}

static bool dd_contains(seq_t v)
{
    size_t lo = 0, hi = dd_sn;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (dd_s[mid] < v) lo = mid + 1;
        else if (dd_s[mid] > v) hi = mid;
        else return true;
    }
    return false;
}

static seq_t dd_next_b(void)
{
    seq_t v = dd_cand;
    while (dd_contains(v)) ++v;
    dd_cand = v + 1;
    return v;
}

static void dd_insert(seq_t v)
{
    dd_grow(&dd_s, &dd_scap, dd_sn + 1);
    size_t j = dd_sn;
    while (j > 0 && dd_s[j - 1] > v) {
        dd_s[j] = dd_s[j - 1];
        --j;
    }
    dd_s[j] = v;
    ++dd_sn;
}

static void dd_step(void)
{
    if (!dd_init) {
        dd_init = true;
        dd_insert((seq_t)1);
        dd_insert((seq_t)2);
        dd_grow(&dd_b, &dd_bcap, 2);
        dd_b[0] = dd_next_b();
        dd_b[1] = dd_next_b();
        dd_bn = 2;
        dd_m = 2;
    }
    size_t m = dd_m;
    while (dd_bn <= m - 1) {
        seq_t nb = dd_next_b();
        dd_grow(&dd_b, &dd_bcap, dd_bn + 1);
        dd_b[dd_bn++] = nb;
    }
    seq_t v = (seq_t)2 * dd_b[m - 1] - dd_b[m - 2] + (seq_t)m;
    dd_insert(v);
    dd_m = m + 1;

    seq_t T = dd_b[m - 1];
    while (dd_scan < dd_sn && dd_s[dd_scan] <= T) {
        seq_t val = dd_s[dd_scan];
        size_t r = 1;
        while (dd_scan + r < dd_sn && dd_s[dd_scan + r] == val) ++r;
        if (r >= 2) {
            dd_grow(&dd_d, &dd_dcap, dd_dn + 1);
            dd_d[dd_dn++] = val;
        }
        dd_scan += r;
    }
}

seq_t a(idx_t n)
{
    while (!dd_init || dd_dn < (size_t)n)
        dd_step();
    return dd_d[(size_t)n - 1];
}
