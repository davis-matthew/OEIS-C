/*
 * A328706  Dirichlet g.f.: Product_{k>=2} (1 + k^(-s))^2.
 * https://oeis.org/A328706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A045778(n/d) * A045778(d)
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 7408 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where A045778(k) counts factorizations of k into distinct integers >1
   (unordered). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Recursive helper: count factorizations of rem into distinct factors >= minf */
static seq_t compute_g_rec(idx_t rem, idx_t minf) {
    if (rem == 1) return (seq_t)1;         /* empty factorization */
    if (rem < minf || rem < 2) return (seq_t)0;
    seq_t total = 0;
    for (idx_t d = 2; d <= rem / d; ++d) {
        if (rem % d == 0) {
            if (d >= minf) {
                total += compute_g_rec(rem / d, d + 1);
            }
            idx_t e = rem / d;
            if (e != d && e >= minf) {
                total += compute_g_rec(rem / e, e + 1);
            }
        }
    }
    /* include the factorization consisting of the number itself */
    if (rem >= minf && rem >= 2) total += (seq_t)1;
    return total;
}

/* ---------- helpers for A045778 ---------- */
static seq_t *g_cache = NULL;      /* cached values of g(n) */
static bool  *g_done  = NULL;      /* flag if g_cache[n] is valid */
static size_t g_cap   = 0;         /* current capacity of caches */

/* Ensure caches can hold index n */
static void ensure_g_cache(idx_t n) {
    if (n < g_cap) return;
    size_t new_cap = g_cap ? g_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *gc = mem_resize(seq_t, g_cache, new_cap);
    bool  *gd = (bool  *)realloc(g_done , new_cap * sizeof(bool));
    if (!gc || !gd) abort();
    for (size_t i = g_cap; i < new_cap; ++i) gd[i] = false;
    g_cache = gc;
    g_done  = gd;
    g_cap   = new_cap;
}

/* Public routine to obtain A045778(n). Uses caching for speed. */
static seq_t g_value(idx_t n) {
    if (n == 0) return (seq_t)0;           /* unused, offset is 1 */
    ensure_g_cache(n);
    if (g_done[n]) return g_cache[n];
    seq_t val = (n == 1) ? (seq_t)1 : compute_g_rec(n, 2);
    g_cache[n] = val;
    g_done[n]  = true;
    return val;
}

/* ---------- cache for a(n) ---------- */
static seq_t *a_cache = NULL;
static bool  *a_done  = NULL;
static size_t a_cap   = 0;

/* Ensure a(n) cache can hold index n */
static void ensure_a_cache(idx_t n) {
    if (n < a_cap) return;
    size_t new_cap = a_cap ? a_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *ac = mem_resize(seq_t, a_cache, new_cap);
    bool  *ad = (bool  *)realloc(a_done , new_cap * sizeof(bool));
    if (!ac || !ad) abort();
    for (size_t i = a_cap; i < new_cap; ++i) ad[i] = false;
    a_cache = ac;
    a_done  = ad;
    a_cap   = new_cap;
}

/* Main sequence routine */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;               /* offset is 1 */
    ensure_a_cache(n);
    if (a_done[n]) return a_cache[n];

    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            seq_t gi = g_value(i);
            seq_t gj = g_value(j);
            if (i == j) {
                sum += gi * gj;
            } else {
                sum += (seq_t)2 * gi * gj;
            }
        }
    }

    a_cache[n] = sum;
    a_done[n]  = true;
    return sum;
}
