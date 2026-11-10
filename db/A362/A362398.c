/*
 * A362398  Records in A336830.
 * https://oeis.org/A362398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th record (new maximum) of A336830, where A336830(0)=0,
 *            A336830(1)=1, and for k>1 A336830(k) = min of unused {prev-k,
 *            prev/k} if any, else min of unused {prev+k, prev*k}, else
 *            prev+k; computed by direct simulation
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 26, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *R_vals = NULL;
static size_t R_nv = 0, R_cv = 0;
static seq_t *R_recs = NULL;
static size_t R_nr = 0, R_cr = 0;
static size_t *R_tab = NULL;
static size_t R_tabsz = 0;
static int R_ovf = 0;

static size_t R_hash(seq_t v)
{
    unsigned long long lo = (unsigned long long)(v & (seq_t)0xFFFFFFFFu);
    unsigned long long hi = (unsigned long long)((v >> 32) & (seq_t)0xFFFFFFFFu);
    unsigned long long h = (lo ^ (hi * 0xC2B2AE3D27D4EB4FULL)) * 0x9E3779B97F4A7C15ULL;
    return (size_t)(h >> 29);
}

static bool R_has(seq_t v)
{
    if (R_tabsz == 0) return false;
    size_t m = R_tabsz - 1, h = R_hash(v) & m;
    while (R_tab[h]) {
        if (R_vals[R_tab[h] - 1] == v) return true;
        h = (h + 1) & m;
    }
    return false;
}

static void R_tab_insert(size_t idx)
{
    size_t m = R_tabsz - 1, h = R_hash(R_vals[idx]) & m;
    while (R_tab[h]) h = (h + 1) & m;
    R_tab[h] = idx + 1;
}

static void R_push(seq_t v)
{
    if (R_nv == R_cv) {
        size_t nc = R_cv ? R_cv * 2 : 256;
        seq_t *p = mem_resize(seq_t, R_vals, nc);
        if (!p) abort();
        R_vals = p; R_cv = nc;
    }
    R_vals[R_nv] = v;
    if ((R_nv + 1) * 2 > R_tabsz) {
        size_t ns = R_tabsz ? R_tabsz * 2 : 1024;
        size_t *t = (size_t *)calloc(ns, sizeof(size_t));
        if (!t) abort();
        free(R_tab);
        R_tab = t; R_tabsz = ns;
        for (size_t i = 0; i < R_nv; ++i) R_tab_insert(i);
    }
    R_tab_insert(R_nv);
    R_nv++;
    if (R_nr == 0 || v > R_recs[R_nr - 1]) {
        if (R_nr == R_cr) {
            size_t nc = R_cr ? R_cr * 2 : 64;
            seq_t *p = mem_resize(seq_t, R_recs, nc);
            if (!p) abort();
            R_recs = p; R_cr = nc;
        }
        R_recs[R_nr++] = v;
    }
}

static void R_step(void)
{
    size_t i = R_nv;
    seq_t nm = (seq_t)i;
    if ((size_t)(idx_t)nm != i || (seq_t)(i) == (seq_t)0) { R_ovf = 1; return; }
    seq_t prev = R_vals[i - 1];
    seq_t best = 0;
    bool found = false;
    if (prev >= nm) {
        seq_t v = prev - nm;
        if (!R_has(v)) { best = v; found = true; }
    }
    if (prev % nm == (seq_t)0) {
        seq_t v = prev / nm;
        if (!R_has(v) && (!found || v < best)) { best = v; found = true; }
    }
    if (found) { R_push(best); return; }

    seq_t add = prev + nm;
    bool add_ov = add < prev;
    seq_t mul = prev * nm;
    bool mul_ov = (prev != (seq_t)0) && (mul / prev != nm);
    if (add_ov) { R_ovf = 1; return; }
    bool av = !R_has(add);
    bool mv = !mul_ov && !R_has(mul);
    if (av && mv) best = (mul < add) ? mul : add;
    else if (av) best = add;
    else if (mv) best = mul;
    else if (mul_ov) { R_ovf = 1; return; }
    else best = add;
    R_push(best);
}

seq_t a(idx_t n)
{
    if (R_nv == 0) {
        R_push((seq_t)0);
        R_push((seq_t)1);
    }
    while (R_nr < (size_t)n && !R_ovf) R_step();
    if (n >= 1 && (size_t)n <= R_nr) return R_recs[n - 1];
    return (seq_t)0;
}
