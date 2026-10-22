/*
 * A337170  Squares visited by knight moves on a diagonally back and forth
 *          numbered board and moving to the lowest available unvisited square
 *          at every step.
 * https://oeis.org/A337170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, look
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 343, u32 n <= 343, u64 n <= 343,
 *            u128 n <= 343, big any n
 * Helpers:   board_number, hash64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Computes OEIS A337170 by simulating a knight
 * starting at (1,1) on an infinitely large board whose squares
 * are numbered anti‑diagonally.  At each step the knight moves
 * to the still‑unvisited square with the lowest board number
 * that is reachable by a single knight move.  This function
 * returns a(n) for n ≥ 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/board_number.h"
#include "../utils/hash64.h"

/* Simple open‑addressing hash table for 64‑bit keys,
 * used to remember visited board coordinates. */
struct hash_table {
    uint64_t *keys;   /* 0 denotes an empty slot */
    size_t cap;       /* power of two */
};

static void ht_init(struct hash_table *ht, size_t cap)
{
    ht->cap  = cap;
    ht->keys = (uint64_t *)calloc(cap, sizeof(uint64_t));
}

static void ht_free(struct hash_table *ht)
{
    free(ht->keys);
}

static bool ht_contains(const struct hash_table *ht, uint64_t key)
{
    size_t i = hash64(key, ht->cap);
    while (ht->keys[i]) {
        if (ht->keys[i] == key) return true;
        i = (i + 1) & (ht->cap - 1);
    }
    return false;
}

static void ht_insert(struct hash_table *ht, uint64_t key)
{
    size_t i = hash64(key, ht->cap);
    while (ht->keys[i]) {
        if (ht->keys[i] == key) return;  /* already present */
        i = (i + 1) & (ht->cap - 1);
    }
    ht->keys[i] = key;
}

/* Knight's 8 possible moves */
static const int64_t KNX[8] = {  1,  2, -1, -2,  1,  2, -1, -2 };
static const int64_t KNY[8] = {  2,  1,  2,  1, -2, -1, -2, -1 };

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined beyond offset, return 0 safely */

    /* Prepare hash table: load factor ≤ 0.5 for up to 2n+16 items. */
    size_t cap = 16;
    while (cap < 2 * (size_t)n + 16) cap <<= 1;

    struct hash_table ht;
    ht_init(&ht, cap);

    int64_t x = 1, y = 1;                 /* start at (1,1) */
    seq_t cur_val = board_number(x, y);   /* = 1 */
    if (n == 1) {
        ht_free(&ht);
        return cur_val;
    }

    ht_insert(&ht, ((uint64_t)(uint32_t)x << 32) | (uint32_t)y);

    for (idx_t step = 2; step <= n; ++step) {
        seq_t best_val = 0;
        int64_t best_x = 0, best_y = 0;
        bool found = false;

        for (int dir = 0; dir < 8; ++dir) {
            int64_t nx = x + KNX[dir];
            int64_t ny = y + KNY[dir];

            if (nx < 1 || ny < 1) continue;          /* outside board */

            uint64_t key = ((uint64_t)(uint32_t)nx << 32) | (uint32_t)ny;
            if (ht_contains(&ht, key)) continue;     /* already visited */

            seq_t val = board_number(nx, ny);
            if (!found || val < best_val) {
                best_val = val;
                best_x   = nx;
                best_y   = ny;
                found    = true;
            }
        }

        /* A move is guaranteed to exist for all steps. */
        cur_val = best_val;
        x = best_x;
        y = best_y;
        ht_insert(&ht, ((uint64_t)(uint32_t)x << 32) | (uint32_t)y);
    }

    ht_free(&ht);
    return cur_val;
}
