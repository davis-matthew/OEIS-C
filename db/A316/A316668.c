/*
 * A316668  Squares visited by king moves on a diagonally numbered board and
 *          moving to the lowest available unvisited square at each step.
 * https://oeis.org/A316668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 252, u16 n <= 65533, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 83 terms through n = 9828 (2026-11-03)
 * Helpers:   mem, triangular, triangular_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence implemented by greedy king moves simulation. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/triangular.h"
#include "../utils/triangular_idx.h"
#include "../utils/mem.h"

/* Convert a board number to its coordinates (r,c), 1‑based */
static inline void number_to_coord(seq_t n, idx_t *r, idx_t *c)
{
    idx_t val = (idx_t)n;               /* assume val fits in idx_t */
    idx_t d = 0;
    while (triangular(d + 1) < val)
        ++d;
    idx_t t = triangular(d);
    *r = val - t;
    *c = d + 2 - *r;
}

/* ---------- static state ---------- */
static seq_t      *seq_data   = NULL;
static size_t     seq_cap    = 0;
static size_t     seq_len    = 0;

/* the squares visited, by number, in a byte array that grows (this was a
   hash table of a fixed 65537 slots, which looped for ever once full) */
static unsigned char *visited = NULL;
static size_t visited_cap = 0;

static void visited_insert(seq_t k)
{
    size_t i = (size_t)k;
    if (i >= visited_cap) {
        size_t c = visited_cap ? visited_cap : 1024;
        while (c <= i) c *= 2;
        unsigned char *t = (unsigned char *)realloc(visited, c);
        if (!t) abort();
        for (size_t j = visited_cap; j < c; ++j) t[j] = 0;
        visited = t;
        visited_cap = c;
    }
    visited[i] = 1;
}

static inline bool visited_contains(seq_t k)
{
    size_t i = (size_t)k;
    return i < visited_cap && visited[i];
}

/* initialize static state exactly once */
static void init_state(void)
{
    static bool done = false;
    if (done) return;

    seq_cap = 128;
    seq_data = mem_new(seq_t, seq_cap);
    seq_len = 0;


    /* start with square 1 */
    seq_t one = 1;
    seq_data[seq_len++] = one;
    visited_insert(one);

    done = true;
}

/* --------------------------------------------------------- */
/* Return the n-th term of the sequence (1‑based).            */
/* --------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_state();
    if (n == 0) return 0;          /* behaviour below offset not required */

    while (seq_len < (size_t)n) {
        seq_t current = seq_data[seq_len - 1];
        idx_t cur_r, cur_c;
        number_to_coord(current, &cur_r, &cur_c);

        seq_t best = 0;
        bool found = false;

        /* 8 king moves */
        const int dr[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
        const int dc[8] = { -1,  0,  1,-1, 1,-1, 0, 1 };

        for (int k = 0; k < 8; ++k) {
            int nr_i = (int)cur_r + dr[k];
            int nc_i = (int)cur_c + dc[k];
            if (nr_i < 1 || nc_i < 1) continue;
            idx_t nr = (idx_t)nr_i;
            idx_t nc = (idx_t)nc_i;
            idx_t diag = nr + nc - 2;
            seq_t cand = triangular_idx(diag) + (seq_t)nr;

            if (!visited_contains(cand)) {
                if (!found || cand < best) {
                    best = cand;
                    found = true;
                }
            }
        }

        /* at least one available square is guaranteed */
        if (!found) return 0;  /* safety guard */

        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq_data = mem_resize(seq_t, seq_data, seq_cap);
        }
        seq_data[seq_len++] = best;
        visited_insert(best);
    }

    return seq_data[n - 1];
}
