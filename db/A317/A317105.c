/*
 * A317105  Squares visited by a (1,3)-leaper on a spirally numbered board and
 *          moving to the lowest available unvisited square at each step,
 *          squares labelled >=0.
 * https://oeis.org/A317105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 3722, u32 n <= 3722, u64 n <= 3722,
 *            u128 n <= 3722, big any n
 * Helpers:   spiral_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation simulates a (1,3)-leaper moving on an infinite board whose
   squares are numbered by the standard spiral indexing.  It keeps a dynamic
   bitset of all visited indices and, at each step, chooses the reachable
   unvisited square with the smallest spiral index.  The sequence a(n) is
   the list of indices visited, starting with a(0)=0.  The implementation
   works for any unsigned integer type used for seq_t (including the
   arbitrary‑precision build).  It is O(n) for the first n terms. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/spiral_index.h"

/* Candidate moves of a (1,3)-leaper. */
static const int dx[8] = {  1,  1, -1, -1,  3,  3, -3, -3 };
static const int dy[8] = {  3, -3,  3, -3,  1, -1,  1, -1 };

/* Ensure the bitset can hold the given index.  Word size is 64 bits. */
static uint64_t *visited_bits = NULL;
static size_t visited_words = 0;

static void visited_ensure(size_t idx)
{
    size_t word = idx >> 6;
    if (word >= visited_words) {
        size_t new_words = word + 1;
        uint64_t *new_bits = (uint64_t *)realloc(visited_bits,
                                                 new_words * sizeof(uint64_t));
        if (!new_bits) abort();                  /* allocation failure */
        for (size_t i = visited_words; i < new_words; ++i)
            new_bits[i] = 0ULL;
        visited_bits = new_bits;
        visited_words = new_words;
    }
}

static bool visited_test(seq_t idx)
{
    size_t ix = (size_t)idx;
    visited_ensure(ix);
    size_t w = ix >> 6;
    size_t b = ix & 63;
    return ((visited_bits[w] >> b) & 1ULL) != 0;
}

static void visited_set(seq_t idx)
{
    size_t ix = (size_t)idx;
    visited_ensure(ix);
    size_t w = ix >> 6;
    size_t b = ix & 63;
    visited_bits[w] |= 1ULL << b;
}

/* Dynamic array holding the values of the sequence up to the current maximum. */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;        /* number of elements stored */
static size_t seq_cap = 0;        /* current capacity */

/* Current position of the leaper. */
static int64_t cur_x = 0;
static int64_t cur_y = 0;

/* --- Public function ------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* one‑time initialization */
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        seq_cap = 1;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_arr) abort();
        seq_arr[0] = (seq_t)0;
        seq_len = 1;
        cur_x = 0;
        cur_y = 0;
        visited_bits = (uint64_t *)malloc(sizeof(uint64_t));
        if (!visited_bits) abort();
        visited_bits[0] = 1ULL;          /* index 0 visited */
        visited_words = 1;
    }

    /* extend computed sequence up to n */
    while (seq_len <= n) {
        seq_t best_idx = (seq_t)~0ULL;   /* maximum possible value */
        int64_t best_x = 0, best_y = 0;

        for (int dir = 0; dir < 8; ++dir) {
            int64_t nx = cur_x + (int64_t)dx[dir];
            int64_t ny = cur_y + (int64_t)dy[dir];
            seq_t cand = spiral_index(nx, ny);
            if (!visited_test(cand) && cand < best_idx) {
                best_idx = cand;
                best_x = nx;
                best_y = ny;
            }
        }

        /* best move must exist */
        visited_set(best_idx);

        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap << 1;
            seq_t *new_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
            if (!new_arr) abort();
            seq_arr = new_arr;
            seq_cap = new_cap;
        }
        seq_arr[seq_len] = best_idx;
        ++seq_len;
        cur_x = best_x;
        cur_y = best_y;
    }

    return seq_arr[n];
}
