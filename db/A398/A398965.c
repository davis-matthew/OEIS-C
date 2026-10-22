/*
 * A398965  Squares visited by a "Carpenter" (Dabbaba + Knight) moving on a
 *          spirally numbered board always to the lowest available unvisited
 *          square.
 * https://oeis.org/A398965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, changed
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   num_to_pos, pos_to_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: simulate a “Carpenter” piece (Dabbaba + Knight) moving
 * on a square‑spirally numbered board.  From the current square the
 * piece chooses the lowest numbered square that is reachable by one of
 * the eight knight moves or one of the four 2‑step rook (Dabbaba) moves
 * and has not yet been visited.  The sequence a(n) is the square number
 * visited at step n (a(1)=1).  The implementation keeps a dynamic
 * boolean table of visited squares and extends the sequence on demand.
 * It works for any integer size of seq_t (fixed‑width or arbitrary
 * precision) as long as the numbers involved fit into the usable size
 * of the visited table (which is the case for n up to ~10 000). */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/num_to_pos.h"
#include "../utils/pos_to_num.h"

static seq_t *seq_cache   = NULL;
static size_t seq_cap     = 0;
static idx_t  seq_len      = 0;

static bool *visited      = NULL;
static size_t visited_cap = 0;

/* enlarge the visited table so that indices 0..need-1 are valid */
static void ensure_visited(size_t need)
{
    if (need <= visited_cap) return;
    size_t new_cap = visited_cap ? visited_cap * 2 : 64;
    while (new_cap <= need) new_cap <<= 1;
    bool *tmp = (bool *)realloc(visited, new_cap * sizeof(bool));
    if (!tmp) abort();
    memset(tmp + visited_cap, 0, (new_cap - visited_cap) * sizeof(bool));
    visited = tmp;
    visited_cap = new_cap;
}

/* enlarge the sequence array so that indices 0..need-1 are valid */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cap = new_cap;
}

/* 12 possible moves: the eight knight moves + the four Dabbaba moves */
static const int64_t dx[12] = {
    2,  1, -1, -2, -2, -1,  1,  2,
    2, -2,  0,  0
};
static const int64_t dy[12] = {
    1,  2,  2,  1, -1, -2, -2, -1,
    0,  0,  2, -2
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for zero */

    /* initialise (first call) */
    if (seq_len == 0) {
        ensure_seq_cap(1);
        seq_cache[0] = 1;
        seq_len = 1;

        ensure_visited(2);
        visited[1] = true;

        int64_t cur_x = 0, cur_y = 0;
        while (seq_len < n) {
            bool found = false;
            seq_t best = 0;
            for (int i = 0; i < 12; ++i) {
                int64_t nx = cur_x + dx[i];
                int64_t ny = cur_y + dy[i];
                seq_t cand = pos_to_num(nx, ny);
                ensure_visited((size_t)cand + 1);
                if (!visited[(size_t)cand]) {
                    if (!found || cand < best) {
                        best = cand;
                        found = true;
                    }
                }
            }
            if (!found) abort();                /* should never happen */
            ensure_seq_cap(seq_len + 1);
            seq_cache[seq_len] = best;
            seq_len++;
            visited[(size_t)best] = true;
            num_to_pos(best, &cur_x, &cur_y);
        }
    }
    /* extend the cache if more terms are requested */
    else if (seq_len < n) {
        int64_t cur_x, cur_y;
        num_to_pos(seq_cache[seq_len - 1], &cur_x, &cur_y);
        while (seq_len < n) {
            bool found = false;
            seq_t best = 0;
            for (int i = 0; i < 12; ++i) {
                int64_t nx = cur_x + dx[i];
                int64_t ny = cur_y + dy[i];
                seq_t cand = pos_to_num(nx, ny);
                ensure_visited((size_t)cand + 1);
                if (!visited[(size_t)cand]) {
                    if (!found || cand < best) {
                        best = cand;
                        found = true;
                    }
                }
            }
            if (!found) abort();
            ensure_seq_cap(seq_len + 1);
            seq_cache[seq_len] = best;
            seq_len++;
            visited[(size_t)best] = true;
            num_to_pos(best, &cur_x, &cur_y);
        }
    }
    return seq_cache[n - 1];
}
