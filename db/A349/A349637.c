/*
 * A349637  a(1) = 2 and a(n) is the smallest nonsquare positive integer not
 *          occurring earlier such that the intersection of the periodic parts
 *          of continued fractions for square roots of a(n) and a(n-1) is the
 *          empty set.
 * https://oeis.org/A349637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   period_intersection_empty
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This program implements the sequence A349637.
   a(1) = 2.
   For n > 1, a(n) is the smallest unused nonsquare integer such that
   the periodic part of the continued‑fraction expansion of √a(n)
   has no element in common with that of √a(n‑1).  The function
   builds the sequence on demand, memoizing all previously generated
   terms and reusing the computed continued‑fraction periods. */

/* Dependencies only from the standard library.
   The build supplies the typedefs seq_t and idx_t, so no need to
   define them here. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/period_intersection_empty.h"

/* ---------- integer square root for idx_t (uint64_t) ---------- */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= (__uint128_t)n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ---------- test for perfect square ---------- */
static bool is_square_idx(idx_t n)
{
    idx_t r = isqrt_u64(n);
    __uint128_t sq = (__uint128_t)r * r;
    return sq == (__uint128_t)n;
}

/* ---------- memory and caching ---------- */
static seq_t *seq      = NULL;   /* sequence values, 0‑based */
static size_t seq_len  = 0;      /* number of terms stored    */
static size_t seq_cap  = 0;      /* allocated capacity        */

static uint8_t *used    = NULL;   /* 0 = unused, 1 = used      */
static size_t used_cap   = 0;      /* allocated capacity        */

static idx_t *last_period = NULL; /* periodic part of √a(n‑1)   */
static size_t last_len    = 0;      /* its length                */

static idx_t min_unused   = 2;      /* smallest unused candidate */

/* Ensure used[] can index up to n (inclusive). */
static void ensure_used_cap(idx_t n)
{
    if ((idx_t)used_cap > n) return;
    size_t new_cap = used_cap ? used_cap : 16;
    while ((idx_t)new_cap <= n) new_cap <<= 1;
    uint8_t *tmp = (uint8_t*)realloc(used, new_cap * sizeof(uint8_t));
    if (!tmp && new_cap > 0) exit(1);
    if (tmp != used) used = tmp;
    /* zero the newly allocated region */
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
    used_cap = new_cap;
}

/* Ensure seq[] can hold at least n elements. */
static void ensure_seq_cap(size_t n)
{
    if (seq_cap >= n) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) exit(1);
    if (tmp != seq) seq = tmp;
    seq_cap = new_cap;
}

/* ---------- continued‑fraction period of sqrt(D) ---------- */
static void compute_period(idx_t D, idx_t **arr, size_t *len)
{
    idx_t a0 = isqrt_u64(D);
    idx_t m  = 0;
    idx_t d  = 1;
    idx_t a  = a0;
    size_t cap = 16;
    idx_t *p = (idx_t*)malloc(cap * sizeof(idx_t));
    size_t l = 0;

    while (true) {
        m = (idx_t)(((__uint128_t)d * a - m));
        d = (idx_t)((((__uint128_t)D - (__uint128_t)m * m) / d));
        a = (idx_t)((a0 + m) / d);

        if (l == cap) {
            cap <<= 1;
            p = (idx_t*)realloc(p, cap * sizeof(idx_t));
        }
        p[l++] = a;
        if (a == 2 * a0) break;
    }
    *arr = p;
    *len = l;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */

    /* generate terms up to n */
    while (seq_len < n) {
        /* --- first term, trivial --- */
        if (seq_len == 0) {
            seq_t val = (seq_t)2;
            ensure_seq_cap(seq_len + 1);
            seq[seq_len++] = val;

            ensure_used_cap((idx_t)val);
            used[(idx_t)val] = 1;

            idx_t *p = NULL; size_t pl = 0;
            compute_period((idx_t)2, &p, &pl);
            if (last_period) free(last_period);
            last_period = p; last_len = pl;

            if (min_unused == 2) {
                min_unused = 3;
                ensure_used_cap(min_unused);
                while (used[min_unused]) ++min_unused;
            }
            continue;
        }

        /* --- find next term --- */
        idx_t cand = min_unused;
        idx_t *chosen_period = NULL;
        size_t chosen_len = 0;

        while (true) {
            ensure_used_cap(cand);
            if (used[cand]) { ++cand; continue; }
            if (is_square_idx(cand)) { ++cand; continue; }

            idx_t *p = NULL; size_t pl = 0;
            compute_period(cand, &p, &pl);

            bool ok = period_intersection_empty(p, pl, last_period, last_len);
            if (ok) {
                chosen_period = p;
                chosen_len    = pl;
                break;                  /* candidate chosen */
            } else {
                free(p);                 /* discard non‑matching period */
            }
            ++cand;
        }

        /* --- record chosen term --- */
        seq_t val = (seq_t)cand;
        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = val;
        used[(idx_t)val] = 1;

        free(last_period);
        last_period = chosen_period;
        last_len    = chosen_len;

        /* --- update min_unused if necessary --- */
        if (cand == min_unused) {
            min_unused = cand + 1;
            ensure_used_cap(min_unused);
            while (used[min_unused]) ++min_unused;
        }
    }

    return seq[n - 1];
}
