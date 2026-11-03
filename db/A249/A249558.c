/*
 * A249558  a(a(n)) is a triangular number.
 * https://oeis.org/A249558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249558
 *   a(1) = 1, a(2) = 3, a(3) = 6, a(4) = 2, …
 *   Build the permutation by always choosing the smallest unused integer
 *   that does not violate the triangular-number rules, so that all
 *   triangular numbers appear in natural order.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Expand a bool array to hold at least idx elements (1‑based). */
static void ensure_bool(idx_t idx, bool **arr, size_t *cap)
{
    if ((size_t)idx < *cap) return;
    size_t new_cap = *cap ? *cap : 16;
    while (new_cap <= (size_t)idx) new_cap <<= 1;
    bool *new_arr = (bool *)realloc(*arr, new_cap * sizeof(bool));
    if (!new_arr) abort();
    for (size_t i = *cap; i < new_cap; ++i) new_arr[i] = false;
    *arr = new_arr;
    *cap = new_cap;
}

/* Return the k‑th triangular number T(k) = k*(k+1)/2 */
static inline seq_t triangular(idx_t k)
{
    return (seq_t)k * (k + 1) / 2;
}

/* Test if a value is triangular. Uses the property that 8*n+1 = square. */
static inline bool is_triangular(seq_t x)
{
    seq_t y = (seq_t)8 * x + 1;
    seq_t s = floor_sqrt(y);
    return s * s == y && (s & 1);
}

/* ----------  dynamic storage for the sequence  ---------- */

static seq_t *perm        = NULL;  /* perm[n] = a(n), indices start at 1   */
static size_t perm_cap    = 0;

static bool *used        = NULL;  /* used[v] = true if value v already used */
static size_t used_cap    = 0;

static bool *must_tri    = NULL;  /* must_tri[i] = true if a(i) must be triangular */
static size_t must_tri_cap = 0;

/* ----------  triangular numbers in natural order  ---------- */

static idx_t tri_index     = 1;          /* current triangular index k (1‑based) */
static seq_t next_tri_val  = 1;          /* T(tri_index) */

/* ----------  bookkeeping  ---------- */

static idx_t last_computed = 0;          /* highest index already computed */

/* Expand an array of seq_t to hold at least idx elements (1‑based). */
static void ensure_perm(idx_t idx)
{
    if ((size_t)idx < perm_cap) return;
    size_t new_cap = perm_cap ? perm_cap : 16;
    while (new_cap <= (size_t)idx) new_cap <<= 1;
    seq_t *new_arr = mem_resize(seq_t, perm, new_cap);
    if (!new_arr) abort();
    for (size_t i = perm_cap; i < new_cap; ++i) new_arr[i] = 0;
    perm = new_arr;
    perm_cap = new_cap;
}

/* ----------  the sequence function  ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n = 0 */

    /* Ensure storage for indices up to n. */
    ensure_perm(n);

    /* Compute values up to n if not already done. */
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        /* Make sure bookkeeping arrays can address index i. */
        ensure_bool(i, &used,       &used_cap);
        ensure_bool(i, &must_tri,   &must_tri_cap);

        /* Choose the smallest unused integer that keeps the rules. */
        for (idx_t cand_idx = 1; ; ++cand_idx)
        {
            /* Make sure bookkeeping arrays can address cand_idx. */
            ensure_bool(cand_idx, &used,     &used_cap);
            ensure_bool(cand_idx, &must_tri, &must_tri_cap);

            if (used[cand_idx]) continue;            /* already used */

            seq_t cand_val = (seq_t)cand_idx;

            /* Rule: if cand is triangular, it must be the next triangular number. */
            if (is_triangular(cand_val) && cand_val != next_tri_val)
                continue;

            /* Rule: if cand equals its own index, the value must be triangular. */
            if (cand_idx == i && !is_triangular(cand_val))
                continue;

            /* Rule: a(i) may be forced to be triangular. */
            if (must_tri[i] && !is_triangular(cand_val))
                continue;

            /* Rule: if cand ≤ i‑1, then a(cand) must already be triangular. */
            if (cand_idx <= i - 1 && !is_triangular(perm[cand_idx]))
                continue;

            /* Accept this candidate. */
            used[cand_idx] = true;
            perm[i] = cand_val;

            /* If the chosen value points to a future index, force it to be triangular. */
            if (cand_idx > i)
                must_tri[cand_idx] = true;

            /* Advance to the next triangular number if the chosen value is triangular. */
            if (is_triangular(cand_val))
            {
                ++tri_index;
                next_tri_val = triangular(tri_index);
            }

            break;   /* move on to compute the next index */
        }
    }

    last_computed = n;
    return perm[n];
}
