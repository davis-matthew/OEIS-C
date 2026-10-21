/*
 * A275117  Direction where Langton's ant is looking after n moves: 1 if
 *          looking in starting direction, 2 if looking 90 degrees clockwise
 *          from starting direction, 3 if looking 90 degrees counterclockwise
 *          from starting direction, or 4 if looking in direction opposite to
 *          starting direction.
 * https://oeis.org/A275117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = direction of Langton's ant after n moves mapped to 1 if
 *            looking in the starting direction, 2 if 90° clockwise from the
 *            starting direction, 3 if 90° counter‑clockwise from the starting
 *            direction, 4 if in the opposite direction
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   dir_to_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/dir_to_value.h"

/* Cell colour storage */
typedef struct {
    uint64_t key;   /* 64‑bit hash of (x,y) */
    uint8_t  color; /* 0 = white, 1 = black */
    uint8_t  used;
} Entry;

/* Hash table for coloured cells */
static size_t          cell_table_size = 0;
static Entry*          cell_table      = NULL;
static size_t          cell_used       = 0;

/* Grow hash table to new size */
static void grow_cell_table(void);

/* 64‑bit splitmix64 hash */
static inline size_t hash64(uint64_t k)
{
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccdULL;
    k ^= k >> 33;
    k *= 0xc4ceb9fe1a85ec53ULL;
    k ^= k >> 33;
    return (size_t)k;
}

/* Ensure load factor < 0.5 */
static void grow_cell_table(void)
{
    size_t new_size = cell_table_size ? cell_table_size * 2 : 16;
    Entry* new_table = (Entry*)calloc(new_size, sizeof(Entry));
    if (!new_table) exit(EXIT_FAILURE);

    size_t mask = new_size - 1;
    for (size_t i = 0; i < cell_table_size; ++i) {
        if (cell_table[i].used) {
            size_t idx = hash64(cell_table[i].key) & mask;
            while (new_table[idx].used) idx = (idx + 1) & mask;
            new_table[idx] = cell_table[i];
        }
    }
    free(cell_table);
    cell_table = new_table;
    cell_table_size = new_size;
}

/* Retrieve or create the entry for (x,y) */
static Entry* get_entry(int32_t x, int32_t y)
{
    uint64_t key = ((uint64_t)(uint32_t)x << 32) | (uint32_t)y;
    size_t idx = hash64(key) & (cell_table_size - 1);
    while (true) {
        if (!cell_table[idx].used) {
            if (cell_used * 2 >= cell_table_size) {
                grow_cell_table();
                idx = hash64(key) & (cell_table_size - 1);
                continue;
            }
            cell_table[idx].used  = 1;
            cell_table[idx].key   = key;
            cell_table[idx].color = 0;
            ++cell_used;
            return &cell_table[idx];
        }
        if (cell_table[idx].key == key) return &cell_table[idx];
        idx = (idx + 1) & (cell_table_size - 1);
    }
}

/* Memoisation buffers */
static seq_t*    dir_seq  = NULL;
static idx_t     dir_cap  = 0;
static idx_t     max_pre  = 0;   /* highest index computed */

/* Current ant state */
static int32_t cur_x = 0, cur_y = 0;
static int     cur_dir = 0;      /* 0 = North */

/* External function required by the test harness */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        /* initialise memoisation array */
        dir_cap = 1;
        dir_seq = (seq_t*)malloc(dir_cap * sizeof(seq_t));
        if (!dir_seq) exit(EXIT_FAILURE);
        dir_seq[0] = dir_to_value(0);   /* a(0) = 1 */

        /* initialise hash table for cell colours */
        cell_table_size = 16;          /* must be power of two */
        cell_table      = (Entry*)calloc(cell_table_size, sizeof(Entry));
        if (!cell_table) exit(EXIT_FAILURE);

        /* other state */
        cur_x = cur_y = 0;
        cur_dir = 0;
        max_pre = 0;
        initialized = true;
    }

    if (n <= max_pre)
        return dir_seq[(size_t)n];

    /* Simulate steps from max_pre+1 up to n */
    for (idx_t step = max_pre + 1; step <= n; ++step) {
        Entry* e = get_entry(cur_x, cur_y);

        /* Turn based on current colour */
        if (e->color == 0)          /* white -> turn right */
            cur_dir = (cur_dir + 1) & 3;
        else                        /* black -> turn left */
            cur_dir = (cur_dir + 3) & 3;

        /* Flip colour */
        e->color ^= 1;

        /* Move forward */
        switch (cur_dir) {
            case 0: cur_y -= 1; break; /* North */
            case 1: cur_x += 1; break; /* East */
            case 2: cur_y += 1; break; /* South */
            case 3: cur_x -= 1; break; /* West */
        }

        /* Grow sequence array if needed */
        if ((idx_t)step >= dir_cap) {
            idx_t new_cap = dir_cap ? dir_cap * 2 : 1;
            while (new_cap <= step) new_cap <<= 1;
            seq_t* tmp = (seq_t*)realloc(dir_seq, (size_t)new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            dir_seq = tmp;
            dir_cap = new_cap;
        }

        dir_seq[(size_t)step] = dir_to_value(cur_dir);
    }

    max_pre = n;
    return dir_seq[(size_t)n];
}
