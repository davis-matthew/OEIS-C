/*
 * A316668  Squares visited by king moves on a diagonally numbered board and
 *          moving to the lowest available unvisited square at each step.
 * https://oeis.org/A316668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 252, u16 n <= 9828, u32 n <= 9828, u64 n <= 9828,
 *            u128 n <= 9828, big any n
 * Helpers:   triangular_seq, triangular_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence implemented by greedy king moves simulation. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/triangular_seq.h"
#include "../utils/triangular_u64.h"

/* Convert a board number to its coordinates (r,c), 1‑based */
static inline void number_to_coord(seq_t n, idx_t *r, idx_t *c)
{
    idx_t val = (idx_t)n;               /* assume val fits in idx_t */
    idx_t d = 0;
    while (triangular_u64(d + 1) < val)
        ++d;
    idx_t t = triangular_u64(d);
    *r = val - t;
    *c = d + 2 - *r;
}

/* ---------- static state ---------- */
static seq_t      *seq_data   = NULL;
static size_t     seq_cap    = 0;
static size_t     seq_len    = 0;

static seq_t      *visited_keys = NULL; /* hash table keys  */
static bool       *visited_seen = NULL; /* occupancy flag   */
static const size_t visited_cap  = 65537; /* prime > 2^16     */
static size_t     visited_cnt = 0;

/* hash: simple modulo */
static inline size_t hash_seq(seq_t k)
{
    return (size_t)k % visited_cap;
}

/* insert key into hash table */
static void visited_insert(seq_t k)
{
    size_t h = hash_seq(k);
    while (visited_seen[h]) {
        if (visited_keys[h] == k) return;
        h = (h + 1) % visited_cap;
    }
    visited_seen[h] = true;
    visited_keys[h] = k;
    ++visited_cnt;
}

/* containment test */
static inline bool visited_contains(seq_t k)
{
    size_t h = hash_seq(k);
    while (visited_seen[h]) {
        if (visited_keys[h] == k) return true;
        h = (h + 1) % visited_cap;
    }
    return false;
}

/* initialize static state exactly once */
static void init_state(void)
{
    static bool done = false;
    if (done) return;

    seq_cap = 128;
    seq_data = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;

    visited_keys = (seq_t *)malloc(visited_cap * sizeof(seq_t));
    visited_seen = (bool *)calloc(visited_cap, sizeof(bool));
    visited_cnt = 0;

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
            seq_t cand = triangular_seq(diag) + (seq_t)nr;

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
            seq_data = (seq_t *)realloc(seq_data, seq_cap * sizeof(seq_t));
        }
        seq_data[seq_len++] = best;
        visited_insert(best);
    }

    return seq_data[n - 1];
}
