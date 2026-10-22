/*
 * A317915  Squares visited by a (2,5)-leaper on a spirally numbered board and
 *          moving to the lowest available unvisited square at each step,
 *          squares labelled >=0.
 * https://oeis.org/A317915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7573, u32 n <= 7573, u64 n <= 7573,
 *            u128 n <= 7573, big any n
 * Helpers:   spiral_label
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the spiral number of the n‑th square visited
   by a (2,5)-leaper that always moves to the lowest numbered
   unvisited neighbour.
   The function simulates the leaper step by step, keeping an
   on‑demand visited bitset and caching results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/spiral_label.h"

/* Global state for caching. */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static uint64_t *bitset = NULL;
static size_t bitset_words = 0;

static int64_t cur_x = 0, cur_y = 0;

/* Ensure the sequence cache can hold at least 'need' entries. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure the bitset can hold at least 'words' 64‑bit words. */
static void ensure_bitset_words(size_t words)
{
    if (words <= bitset_words) return;
    size_t new_words = bitset_words ? bitset_words << 1 : 4;
    while (new_words < words) new_words <<= 1;
    uint64_t *new_bits = (uint64_t *)calloc(new_words, sizeof(uint64_t));
    if (bitset) {
        memcpy(new_bits, bitset, bitset_words * sizeof(uint64_t));
        free(bitset);
    }
    bitset = new_bits;
    bitset_words = new_words;
}

/* Check whether index 'idx' has been visited. */
static inline bool visited(uint64_t idx)
{
    size_t w = idx >> 6;
    return (w < bitset_words) && ((bitset[w] >> (idx & 63)) & 1ULL);
}

/* Mark index 'idx' as visited. */
static inline void mark_visited(uint64_t idx)
{
    size_t w = idx >> 6;
    ensure_bitset_words(w + 1);
    bitset[w] |= 1ULL << (idx & 63);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Lazy initialization. */
    static bool init = false;
    if (!init) {
        ensure_seq_cap(4);
        seq_cache[0] = (seq_t)0;
        seq_len = 1;
        cur_x = 0;
        cur_y = 0;
        ensure_bitset_words(1);
        bitset[0] = 1ULL;              /* mark 0 as visited */
        init = true;
    }

    /* Extend the cache up to index n. */
    while (seq_len <= (size_t)n) {
        static const int dx[8] = { 2,  2, -2, -2,  5,  5, -5, -5 };
        static const int dy[8] = { 5, -5,  5, -5,  2, -2,  2, -2 };

        uint64_t best = UINT64_MAX;
        int best_k = -1;

        for (int k = 0; k < 8; ++k) {
            int64_t nx = cur_x + dx[k];
            int64_t ny = cur_y + dy[k];
            uint64_t idx = (uint64_t)spiral_label(nx, ny);
            if (!visited(idx) && idx < best) {
                best = idx;
                best_k = k;
            }
        }

        /* There is always at least one unvisited neighbour. */
        cur_x += dx[best_k];
        cur_y += dy[best_k];
        mark_visited(best);
        ensure_seq_cap(seq_len + 1);
        seq_cache[seq_len] = (seq_t)best;
        seq_len++;
    }

    return seq_cache[(size_t)n];
}
