/*
 * A361442  Infinite triangle T(n, k), n, k >= 0, read and filled by rows the
 *          greedy way with distinct integers such that for any n, k >= 0,
 *          T(n, k) + T(n+1, k) + T(n+1, k+1) = 0; each term is minimal in
 *          absolute value and in case of a tie, preference is given to the
 *          positive value.
 * https://oeis.org/A361442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   compute the integer sequence A361442 by building the infinite
 *            triangle T(n,k) greedily. The sequence a(n) is T(n,0). For each
 *            n, choose the smallest absolute value not already used; if a tie
 *            occurs, prefer the positive value. Once T(n,0) is fixed, the
 *            rest of the row is determined by the relation...
 * Range:     i8 n <= 26, i16 n <= 103, i32 n <= 523, i64 n <= 1828,
 *            i128 n <= 7501, big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_to_str.h"

/* Simple hash for seq_t values using decimal representation. */
static size_t seq_hash(seq_t v)
{
    char buf[2048];
    size_t len;
    seq_to_str(v, buf, &len);
    uint64_t h = 14695981039346656037ULL; /* FNV‑1a 64‑bit */
    for (size_t i = 0; i < len; ++i) {
        h ^= (uint8_t)buf[i];
        h *= 1099511628211ULL;
    }
    return (size_t)h;
}

/* Triangle storage (flattened) */
static seq_t *tri = NULL;
static idx_t  tri_len = 0;
static size_t tri_cap = 0;

static void ensure_tri_cap(idx_t n)
{
    if (tri_cap < n) {
        size_t new_cap = tri_cap ? tri_cap : 8;
        while (new_cap < (size_t)n) new_cap <<= 1;
        tri = (seq_t *)realloc(tri, new_cap * sizeof(seq_t));
        tri_cap = new_cap;
    }
}

/* Hash table of indices of used values */
static idx_t *used_tbl = NULL;
static size_t used_cap = 0;
static size_t used_used = 0;

static void init_used_tbl(size_t cap)
{
    used_tbl = (idx_t *)malloc(cap * sizeof(idx_t));
    for (size_t i = 0; i < cap; ++i) used_tbl[i] = (idx_t)-1;
    used_cap = cap;
    used_used = 0;
}

static void grow_used_tbl(void)
{
    size_t new_cap = used_cap ? used_cap * 2 : 8;
    idx_t *new_tbl = (idx_t *)malloc(new_cap * sizeof(idx_t));
    for (size_t i = 0; i < new_cap; ++i) new_tbl[i] = (idx_t)-1;

    for (size_t i = 0; i < used_cap; ++i) {
        idx_t idx = used_tbl[i];
        if (idx != (idx_t)-1) {
            size_t h = seq_hash(tri[idx]) & (new_cap - 1);
            while (new_tbl[h] != (idx_t)-1) h = (h + 1) & (new_cap - 1);
            new_tbl[h] = idx;
        }
    }
    free(used_tbl);
    used_tbl = new_tbl;
    used_cap = new_cap;
}

static int used_has(seq_t v)
{
    if (used_cap == 0) return 0;
    size_t h = seq_hash(v) & (used_cap - 1);
    while (used_tbl[h] != (idx_t)-1) {
        if (tri[used_tbl[h]] == v) return 1;
        h = (h + 1) & (used_cap - 1);
    }
    return 0;
}

static void used_insert(seq_t v, idx_t idx)
{
    if (used_used * 2 >= used_cap) grow_used_tbl();

    size_t h = seq_hash(v) & (used_cap - 1);
    while (used_tbl[h] != (idx_t)-1) h = (h + 1) & (used_cap - 1);
    used_tbl[h] = idx;
    used_used++;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    static int first = 1;
    if (first) {
        first = 0;
        init_used_tbl(8);
        ensure_tri_cap(8);
        tri[0] = 0;
        tri_len = 1;
        used_insert(0, 0);
    }

    static idx_t max_row = 1;          /* number of rows already present */

    while (tri_len <= n) {
        idx_t row = max_row;           /* next row to generate (0‑based) */
        idx_t row_len = row + 1;
        idx_t start_prev = (row == 0) ? 0
                          : ((row - 1) * row) / 2;   /* first index of row‑1 */

        /* Find the smallest absolute value that yields a valid row */
        seq_t cand = 0;
        int found = 0;
        for (idx_t abs = 0; !found; ++abs) {
            if (abs == 0) cand = 0;
            else cand = (seq_t)abs;

            if (!used_has(cand)) {
                seq_t val = cand;
                int ok = 1;
                for (idx_t k = 1; k <= row; ++k) {
                    idx_t idx_prev = start_prev + (k - 1);
                    seq_t tmp = - (tri[idx_prev] + val);
                    if (used_has(tmp)) { ok = 0; break; }
                    val = tmp;
                }
                if (ok) { found = 1; break; }
            }

            if (abs > 0) {
                cand = -(seq_t)abs;
                if (!used_has(cand)) {
                    seq_t val = cand;
                    int ok = 1;
                    for (idx_t k = 1; k <= row; ++k) {
                        idx_t idx_prev = start_prev + (k - 1);
                        seq_t tmp = - (tri[idx_prev] + val);
                        if (used_has(tmp)) { ok = 0; break; }
                        val = tmp;
                    }
                    if (ok) { found = 1; break; }
                }
            }
        }

        /* Append the new row to the triangle */
        ensure_tri_cap(tri_len + row_len);
        idx_t row_start = tri_len;
        tri[row_start] = cand;
        used_insert(cand, row_start);
        seq_t val = cand;
        for (idx_t k = 1; k <= row; ++k) {
            idx_t idx_prev = start_prev + (k - 1);
            val = - (tri[idx_prev] + val);
            tri[row_start + k] = val;
            used_insert(val, row_start + k);
        }
        tri_len += row_len;
        max_row++;
    }

    return tri[n];
}
