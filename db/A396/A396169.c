/*
 * A396169  Consider the square spiral with its cells numbered starting at 0.
 *          Seven players take turns and place knights at the smallest
 *          unoccupied cell not attacked by an existing knight of a different
 *          player. Sequence lists squares occupied by the third player.
 * https://oeis.org/A396169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This program simulates the 7‑player knight placement on a square
 *            spiral (OEIS A396169). It returns the n‑th cell number occupied
 *            by the third player, where n ≥ 1
 * Range:     u8 n <= 17, u16 n <= 4848, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   raw_key_from_xy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/raw_key_from_xy.h"

/* ---------- Integer square root (floor) for 64‑bit values ---------- */
static uint64_t isqrt64(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;  /* highest power of 4 ≤ x */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ---------- Hash table to map (x,y) pairs to a player number ---------- */
#define HASH_CAPACITY (1 << 17)          /* 131072 slots, > 7*10000 */
static uint64_t hash_keys[HASH_CAPACITY];
static unsigned char hash_vals[HASH_CAPACITY];
#define EMPTY_KEY 0xFFFFFFFFFFFFFFFFULL

static void hash_init(void)
{
    for (size_t i = 0; i < HASH_CAPACITY; ++i)
        hash_keys[i] = EMPTY_KEY;
}

static inline unsigned char hash_find(uint64_t key)
{
    size_t idx = (key * 0x9e3779b97f4a7c15ULL) & (HASH_CAPACITY - 1);
    while (1) {
        uint64_t k = hash_keys[idx];
        if (k == EMPTY_KEY) return 0;
        if (k == key) return hash_vals[idx];
        idx = (idx + 1) & (HASH_CAPACITY - 1);
    }
}

static inline void hash_insert(uint64_t key, unsigned char val)
{
    size_t idx = (key * 0x9e3779b97f4a7c15ULL) & (HASH_CAPACITY - 1);
    while (1) {
        uint64_t k = hash_keys[idx];
        if (k == EMPTY_KEY) {
            hash_keys[idx] = key;
            hash_vals[idx] = val;
            return;
        }
        if (k == key) {
            hash_vals[idx] = val;
            return;
        }
        idx = (idx + 1) & (HASH_CAPACITY - 1);
    }
}

/* ---------- Knight attack detection ---------- */
static const int8_t knight_dx[8] = { 1, 1, -1, -1,  2,  2, -2, -2 };
static const int8_t knight_dy[8] = { 2,-2,  2, -2,  1, -1,  1, -1 };

static bool attacked_by_other(unsigned char player, int64_t x, int64_t y)
{
    for (int i = 0; i < 8; ++i) {
        int64_t nx = x + knight_dx[i];
        int64_t ny = y + knight_dy[i];
        uint64_t key = raw_key_from_xy(nx, ny);
        unsigned char p2 = hash_find(key);
        if (p2 && p2 != player) return true;
    }
    return false;
}

/* ---------- Convert spiral index to (x,y) coordinates ---------- */
static void index_to_coord(idx_t n, int64_t *out_x, int64_t *out_y)
{
    if (n == 0) {
        *out_x = 0;
        *out_y = 0;
        return;
    }
    uint64_t k = (isqrt64(n - 1) + 1) >> 1;   /* ring number */
    uint64_t side = 2 * k;                    /* side length */
    uint64_t start = (2 * k - 1);
    start = start * start;                    /* first index in this ring */
    uint64_t t = n - start;                   /* 0‑based offset in ring */

    if (t < side) {                           /* right side (going up) */
        *out_x = k;
        *out_y = - (int64_t)(k - 1) + (int64_t)t;
    } else if (t < 2 * side) {                /* top side (going left) */
        t -= side;
        *out_x = (int64_t)(k - 1) - (int64_t)t;
        *out_y = k;
    } else if (t < 3 * side) {                /* left side (going down) */
        t -= 2 * side;
        *out_x = -(int64_t)k;
        *out_y = (int64_t)(k - 1) - (int64_t)t;
    } else {                                  /* bottom side (going right) */
        t -= 3 * side;
        *out_x = -(int64_t)k + 1 + (int64_t)t;
        *out_y = -(int64_t)k;
    }
}

/* ---------- Sequence storage ---------- */
static seq_t *seq_buf = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;  /* number of stored terms for player 3 */

/* ---------- Simulation state ---------- */
static uint64_t cur_pos[8];   /* cur_pos[p] = next candidate index for player p (1‑based) */
static uint64_t turn_counter = 1;  /* current turn number (1‑based) */

/* ---------- Initialization ---------- */
static void init_state(void)
{
    if (seq_buf) return;          /* already initialized */
    seq_cap = 65536;              /* initial capacity */
    seq_buf = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
    for (int p = 1; p <= 7; ++p) cur_pos[p] = 0;
    hash_init();
    turn_counter = 1;
    seq_len = 0;
}

/* ---------- Ensure seq_buf has enough capacity ---------- */
static void ensure_seq_capacity(void)
{
    if (seq_len < seq_cap) return;
    size_t new_cap = seq_cap * 2;
    seq_t *new_buf = (seq_t *)realloc(seq_buf, sizeof(seq_t) * new_cap);
    if (!new_buf) exit(1);      /* out of memory */
    seq_buf = new_buf;
    seq_cap = new_cap;
}

/* ---------- Main requested function ---------- */
seq_t a(idx_t n)
{
    init_state();

    /* Keep generating turns until we have at least n terms for player 3 */
    while (seq_len < (size_t)n) {
        unsigned char player = (unsigned char)(((turn_counter - 1) % 7) + 1);

        /* Find the smallest valid index for this player */
        uint64_t idx = cur_pos[player];
        while (1) {
            int64_t x, y;
            index_to_coord(idx, &x, &y);
            uint64_t key = raw_key_from_xy(x, y);

            unsigned char occ = hash_find(key);
            if (occ) {                 /* occupied */
                idx++;
                continue;
            }

            if (attacked_by_other(player, x, y)) {  /* attacked */
                idx++;
                continue;
            }

            /* valid position found */
            break;
        }
        cur_pos[player] = idx;  /* remember candidate for next search */

        /* Place the knight */
        int64_t x, y;
        index_to_coord(idx, &x, &y);
        uint64_t key = raw_key_from_xy(x, y);
        hash_insert(key, player);

        if (player == 3) {
            ensure_seq_capacity();
            seq_buf[seq_len++] = (seq_t)idx;
        }

        /* Update other players' candidates if they are now invalid */
        for (int other = 1; other <= 7; ++other) {
            if (other == player) continue;
            uint64_t cur = cur_pos[other];
            while (1) {
                int64_t ox, oy;
                index_to_coord(cur, &ox, &oy);
                uint64_t okey = raw_key_from_xy(ox, oy);
                unsigned char occ = hash_find(okey);
                if (occ) { cur++; continue; }
                if (attacked_by_other(other, ox, oy)) { cur++; continue; }
                break;
            }
            cur_pos[other] = cur;
        }

        turn_counter++;
    }

    return seq_buf[(size_t)n - 1];
}
