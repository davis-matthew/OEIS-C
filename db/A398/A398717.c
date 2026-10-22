/*
 * A398717  Squares visited by a "Priest" (Fers + Knight) moving on a spirally
 *          numbered board always to the lowest available unvisited square.
 * https://oeis.org/A398717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, changed
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 1051, u32 n <= 1051, u64 n <= 1051,
 *            u128 n <= 1051, big any n
 * Helpers:   coord_to_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A398717 (Squares visited by a "Priest" (Fers + Knight))
   Simulation: start at square 1 on a square‑spiral numbered board.
   At each step the priest may move like a knight or along a diagonal (fer).
   He always moves to the lowest‑numbered reachable unvisited square.
   The algorithm stores visited squares in a bitset, memoises the
   sequence values, and simulates the moves up to the requested index.
   This implementation is fully portable to C11 and C++17.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/coord_to_index.h"

static seq_t *cache = NULL;      /* memoised values a(1)..a(n) */
static size_t cache_cap = 0;     /* allocated cache capacity */
static size_t cache_len = 0;     /* number of values currently stored */

static uint64_t *visited_bits = NULL; /* bitset of visited board indices */
static size_t visited_words = 0;      /* number of uint64_t words allocated */

static int cur_x = 0, cur_y = 0;     /* current position */
static bool initialized = false;    /* has the simulation been started? */

/* Knight and fer move offsets (relative to current square) */
static const int knight_dx[8] = { 1, 1,-1,-1, 2, 2,-2,-2 };
static const int knight_dy[8] = { 2,-2, 2,-2, 1,-1, 1,-1 };
static const int fer_dx[4]   = { 1,-1, 1,-1 };
static const int fer_dy[4]   = { 1, 1,-1,-1 };

/* Ensure the cache has capacity at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < need) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Ensure the visited_bits bitset can hold at least index 'idx' (1‑based) */
static void ensure_visited_cap(seq_t idx)
{
    size_t bit_index = (size_t)(idx - 1);
    size_t word = bit_index >> 6;        /* 64 bits per word */
    if (word < visited_words) return;
    size_t new_words = visited_words ? visited_words * 2 : 1;
    while (new_words <= word) new_words <<= 1;
    visited_bits = (uint64_t *)realloc(visited_bits, new_words * sizeof(uint64_t));
    if (visited_bits && visited_words < new_words)
        memset(visited_bits + visited_words, 0,
               (new_words - visited_words) * sizeof(uint64_t));
    visited_words = new_words;
}

/* Mark board index 'idx' (1‑based) as visited */
static void set_visited(seq_t idx)
{
    ensure_visited_cap(idx);
    size_t bit_index = (size_t)(idx - 1);
    size_t word = bit_index >> 6;
    size_t bit  = bit_index & 63;
    visited_bits[word] |= ((uint64_t)1 << bit);
}

/* Test whether board index 'idx' has been visited */
static bool is_visited(seq_t idx)
{
    size_t bit_index = (size_t)(idx - 1);
    size_t word = bit_index >> 6;
    if (word >= visited_words) return false;
    size_t bit = bit_index & 63;
    return ((visited_bits[word] >> bit) & 1ULL) != 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;   /* n >= 1 */

    /* Initialize simulation on first call */
    if (!initialized) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache_cap = 1;
        cache_len = 1;
        cache[0] = 1;                /* a(1) = 1 */

        cur_x = 0; cur_y = 0;
        visited_words = 1;
        visited_bits = (uint64_t *)calloc(1, sizeof(uint64_t));
        set_visited(1);

        initialized = true;
    }

    if (N <= cache_len) return cache[N - 1];

    /* Extend cache up to N */
    for (size_t step = cache_len; step < N; ++step) {
        seq_t best_idx = 0;
        int best_x = 0, best_y = 0;
        bool first = true;

        /* Check knight moves */
        for (int k = 0; k < 8; ++k) {
            int nx = cur_x + knight_dx[k];
            int ny = cur_y + knight_dy[k];
            seq_t idx = coord_to_index(nx, ny);
            if (!is_visited(idx)) {
                if (first || idx < best_idx) {
                    best_idx = idx;
                    best_x = nx;
                    best_y = ny;
                    first = false;
                }
            }
        }

        /* Check fer (diagonal) moves */
        for (int f = 0; f < 4; ++f) {
            int nx = cur_x + fer_dx[f];
            int ny = cur_y + fer_dy[f];
            seq_t idx = coord_to_index(nx, ny);
            if (!is_visited(idx)) {
                if (first || idx < best_idx) {
                    best_idx = idx;
                    best_x = nx;
                    best_y = ny;
                    first = false;
                }
            }
        }

        /* Store the chosen square */
        ensure_cache_cap(step + 1);
        cache[step] = best_idx;
        cache_len = step + 1;

        set_visited(best_idx);
        cur_x = best_x;
        cur_y = best_y;
    }

    return cache[N - 1];
}
