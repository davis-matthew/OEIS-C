/*
 * A361154  Consider the square grid with cells {(x,y), x, y >= 0}; label the
 *          cells by downwards antidiagonals with nonnegative integers so that
 *          cells which are a knight's move apart have different labels;
 *          always choose smallest possible label.
 * https://oeis.org/A361154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the lexicographically earliest labeling of the
 *            first‑quadrant integer lattice (x,y ≥ 0) by non‑negative
 *            integers, reading points by increasing antidiagonal (downwards),
 *            with the constraint that any two points at a knight's move
 *            distance must receive different labels. The algorithm builds...
 * Range:     u8 n <= 9541, u16 n <= 9541, u32 n <= 9541, u64 n <= 9541,
 *            u128 n <= 9541, big any n
 * Helpers:   _triangular_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The conversion from an antidiagonal index to lattice coordinates
    uses the following identity:
        d      = x + y                            (antidiagonal number)
        start  = d*(d+1)/2                         (first index on that diagonal)
        x      = n - start
        y      = d - x
    This is precisely the enumeration mentioned in the OEIS entry. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/_triangular_root.h"

/* Cache of computed labels – grows only forwards. */
static seq_t *label_cache = NULL;
static size_t  cache_cap  = 0;   /* allocated capacity */
static idx_t   cache_len  = 0;   /* number of labels already stored */

/* Knight offsets. */
static const int KNIGHT_DX[8] = {  1,  1, -1, -1,  2,  2, -2, -2 };
static const int KNIGHT_DY[8] = {  2, -2,  2, -2,  1, -1,  1, -1 };

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Grow cache if necessary. */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 128;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(label_cache, new_cap * sizeof(seq_t));
        label_cache = tmp;
        cache_cap   = new_cap;
    }

    /* Compute labels up to n, using previously computed values. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Coordinates of cell i (antidiagonal numbering). */
        idx_t d      = _triangular_root(i);
        idx_t start  = d * (d + 1) / 2;
        idx_t x      = i - start;
        idx_t y      = d - x;

        /* Record which colors (0‑4) are already used by knight neighbours. */
        bool used[5] = { false, false, false, false, false };

        /* Check all eight knight moves. */
        for (int k = 0; k < 8; ++k) {
            long long xn = (long long)x + KNIGHT_DX[k];
            long long yn = (long long)y + KNIGHT_DY[k];
            if (xn < 0 || yn < 0) continue;          /* stay in first quadrant */

            /* Convert neighbour coordinates back to index. */
            idx_t d2   = (idx_t)(xn + yn);
            idx_t start2 = d2 * (d2 + 1) / 2;
            idx_t idx2 = start2 + (idx_t)xn;         /* neighbour index */

            /* Only neighbours already computed are relevant. */
            if (idx2 < i) {                          /* i is the current cell */
                seq_t v = label_cache[idx2];
                if (v <= 4) used[(size_t)v] = true;
            }
        }

        /* Smallest non‑negative label not used by any neighbour. */
        seq_t lbl = 0;
        while (lbl <= 4 && used[(size_t)lbl]) ++lbl;
        label_cache[i] = lbl;

        /* Update the length after each new entry. */
        cache_len = i + 1;
    }

    return label_cache[n];
}
