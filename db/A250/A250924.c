/*
 * A250924  Number of (n+1)X(5+1) 0..2 arrays with no 2X2 subblock having its
 *          minimum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A250924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: transfer‑matrix for 0‑2 (n+1)×6 arrays with the
   property that every 2×2 subblock has min(diagonal) ≥ min(antidiagonal). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* -------------------------------------------------------------------- */
/* Global data                                                     */
/* -------------------------------------------------------------------- */
static bool   _init_done = false;
static uint8_t _row_vals[729][6];


/* The adjacency list is stored as a single array _adj_to and a start
   index array _adj_start where the neighbors of state i are in
   _adj_to[_adj_start[i] .. _adj_start[i+1]-1]. */
static size_t _adj_start[730];
static size_t _adj_to[729*729];          /* maximum possible number of edges */

/* Dynamic programming arrays, one for the current state counts and
   one for the next.  They are defined globally so that they are
   re‑used between calls. */
static seq_t _dp_curr[729];
static seq_t _dp_next[729];

/* -------------------------------------------------------------------- */
/* Initialise the row value tables and the adjacency lists             */
/* -------------------------------------------------------------------- */
static void _init_global(void) {
    /* Pre‑compute the base‑3 digits for each of the 3^6 = 729 rows. */
    for (size_t r = 0; r < 729; ++r) {
        size_t tmp = r;
        for (int c = 5; c >= 0; --c) {
            _row_vals[r][c] = (uint8_t)(tmp % 3);
            tmp /= 3;
        }
    }

    /* First pass: count outgoing edges for each state to build the
       start indices. */
    size_t total_edges = 0;
    for (size_t i = 0; i < 729; ++i) {
        size_t cnt = 0;
        for (size_t j = 0; j < 729; ++j) {
            bool ok = true;
            for (int k = 0; k < 5; ++k) {
                uint8_t a = _row_vals[i][k];
                uint8_t b = _row_vals[i][k + 1];
                uint8_t c = _row_vals[j][k];
                uint8_t d = _row_vals[j][k + 1];
                uint8_t diag = a < d ? a : d;
                uint8_t anti = b < c ? b : c;
                if (diag < anti) { ok = false; break; }
            }
            if (ok) ++cnt;
        }
        _adj_start[i] = total_edges;
        total_edges += cnt;
    }
    _adj_start[729] = total_edges;       /* sentinel */

    /* Second pass: store the actual adjacency list. */
    size_t pos = 0;
    for (size_t i = 0; i < 729; ++i) {
        for (size_t j = 0; j < 729; ++j) {
            bool ok = true;
            for (int k = 0; k < 5; ++k) {
                uint8_t a = _row_vals[i][k];
                uint8_t b = _row_vals[i][k + 1];
                uint8_t c = _row_vals[j][k];
                uint8_t d = _row_vals[j][k + 1];
                uint8_t diag = a < d ? a : d;
                uint8_t anti = b < c ? b : c;
                if (diag < anti) { ok = false; break; }
            }
            if (ok) {
                _adj_to[pos++] = j;
            }
        }
    }
    _init_done = true;
}

/* -------------------------------------------------------------------- */
/* Public function                                                   */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (!_init_done) _init_global();

    /* Initialise dp_curr with all ones (single-row arrays). */
    for (size_t i = 0; i < 729; ++i) {
        _dp_curr[i] = (seq_t)1;
    }

    /* Perform n transitions: n edges correspond to n+1 rows. */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero the next array. */
        for (size_t i = 0; i < 729; ++i) {
            _dp_next[i] = (seq_t)0;
        }
        /* Accumulate transitions. */
        for (size_t prev = 0; prev < 729; ++prev) {
            seq_t val = _dp_curr[prev];
            if (val == (seq_t)0) continue;
            size_t start = _adj_start[prev];
            size_t end   = _adj_start[prev + 1];
            for (size_t p = start; p < end; ++p) {
                size_t next = _adj_to[p];
                _dp_next[next] += val;
            }
        }
        /* Move next counts to current for the next iteration. */
        for (size_t i = 0; i < 729; ++i) {
            _dp_curr[i] = _dp_next[i];
        }
    }

    /* Sum all final counts to obtain the total number of arrays. */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < 729; ++i) {
        result += _dp_curr[i];
    }
    return result;
}
