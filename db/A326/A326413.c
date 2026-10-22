/*
 * A326413  Successive squares visited by a knight on the single-digit square
 *          spiral, with ties resolved towards the left.
 * https://oeis.org/A326413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1217, u16 n <= 1217, u32 n <= 1217, u64 n <= 1217,
 *            u128 n <= 1217, big any n
 * Helpers:   coord_key, hash_key
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simulates a knight moving on the digit‑spiral to compute a(n). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/coord_key.h"
#include "../utils/hash_key.h"

/* ------------------------------------------------------------------ */
/*  Hash map for coordinates -> value (seq_t digit or unused)         */
/* ------------------------------------------------------------------ */
#define EMPTY_KEY ((int64_t)0x8000000000000000ULL)
#define LOAD_FACTOR 75  /* percent, use 75 => cap * 3/4 */

typedef struct { int64_t key; seq_t val; } KV;

static KV *kv_tbl = NULL;
static size_t kv_cap = 0, kv_len = 0;

static void kv_init(void)
{
    kv_cap = 64;
    kv_len = 0;
    kv_tbl = (KV *)calloc(kv_cap, sizeof(KV));
    for (size_t i = 0; i < kv_cap; ++i) kv_tbl[i].key = EMPTY_KEY;
}

static void kv_rehash(void)
{
    size_t new_cap = kv_cap << 1;
    KV *new_tbl = (KV *)calloc(new_cap, sizeof(KV));
    for (size_t i = 0; i < new_cap; ++i) new_tbl[i].key = EMPTY_KEY;

    for (size_t i = 0; i < kv_cap; ++i) {
        if (kv_tbl[i].key == EMPTY_KEY) continue;
        uint64_t h = hash_key(kv_tbl[i].key) & (new_cap - 1);
        while (new_tbl[h].key != EMPTY_KEY) h = (h + 1) & (new_cap - 1);
        new_tbl[h] = kv_tbl[i];
    }
    free(kv_tbl);
    kv_tbl = new_tbl;
    kv_cap = new_cap;
}

static void kv_insert(int64_t key, seq_t val)
{
    if ((kv_len + 1) > (kv_cap * LOAD_FACTOR) / 100) kv_rehash();
    uint64_t h = hash_key(key) & (kv_cap - 1);
    while (kv_tbl[h].key != EMPTY_KEY) h = (h + 1) & (kv_cap - 1);
    kv_tbl[h].key = key;
    kv_tbl[h].val  = val;
    ++kv_len;
}

static bool kv_find(int64_t key, seq_t *out)
{
    uint64_t h = hash_key(key) & (kv_cap - 1);
    while (kv_tbl[h].key != EMPTY_KEY) {
        if (kv_tbl[h].key == key) {
            *out = kv_tbl[h].val;
            return true;
        }
        h = (h + 1) & (kv_cap - 1);
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Digit spiral generator                                          */
/* ------------------------------------------------------------------ */
static int64_t gen_x = 0, gen_y = 0;
static int gen_dir = 0;              /* 0:right,1:up,2:left,3:down */
static uint64_t gen_step = 1;
static uint64_t gen_done = 0;
static uint64_t next_num = 0;
static uint8_t digits_buf[20];
static uint8_t digits_len = 0, digits_pos = 0;

static uint8_t next_digit(void)
{
    if (digits_pos == digits_len) {
        uint64_t m = next_num++;
        digits_len = 0;
        if (m == 0) { digits_buf[0] = 0; digits_len = 1; }
        else {
            uint8_t tmp[20];
            int idx = 0;
            while (m > 0) { tmp[idx++] = (uint8_t)(m % 10); m /= 10; }
            for (int i = 0; i < idx; ++i) digits_buf[i] = tmp[idx - 1 - i];
            digits_len = idx;
        }
        digits_pos = 0;
    }
    return digits_buf[digits_pos++];
}

static void gen_next_cell(void)
{
    gen_x += (int64_t)( (gen_dir==0)?1:(gen_dir==2?-1:0) );
    gen_y += (int64_t)( (gen_dir==1)?1:(gen_dir==3?-1:0) );
    ++gen_done;
    if (gen_done == gen_step) {
        gen_done = 0;
        gen_dir = (gen_dir + 1) & 3;
        if (gen_dir == 0 || gen_dir == 2) ++gen_step;
    }
    seq_t d = (seq_t)next_digit();
    kv_insert(coord_key(gen_x, gen_y), d);
}

/* ------------------------------------------------------------------ */
/*  Initialization                                                */
/* ------------------------------------------------------------------ */
static bool spiral_initialized = false;
static KV *visited_tbl = NULL;
static size_t visited_cap = 0, visited_len = 0;

static void visited_init(void)
{
    visited_cap = 64;
    visited_len = 0;
    visited_tbl = (KV *)calloc(visited_cap, sizeof(KV));
    for (size_t i = 0; i < visited_cap; ++i) visited_tbl[i].key = EMPTY_KEY;
}

static void visited_insert(int64_t key)
{
    if ((visited_len + 1) > (visited_cap * LOAD_FACTOR) / 100) {
        size_t new_cap = visited_cap << 1;
        KV *new_tbl = (KV *)calloc(new_cap, sizeof(KV));
        for (size_t i = 0; i < new_cap; ++i) new_tbl[i].key = EMPTY_KEY;
        for (size_t i = 0; i < visited_cap; ++i)
            if (visited_tbl[i].key != EMPTY_KEY) {
                uint64_t h = hash_key(visited_tbl[i].key) & (new_cap - 1);
                while (new_tbl[h].key != EMPTY_KEY) h = (h + 1) & (new_cap - 1);
                new_tbl[h] = visited_tbl[i];
            }
        free(visited_tbl);
        visited_tbl = new_tbl;
        visited_cap = new_cap;
    }
    uint64_t h = hash_key(key) & (visited_cap - 1);
    while (visited_tbl[h].key != EMPTY_KEY) h = (h + 1) & (visited_cap - 1);
    visited_tbl[h].key = key;
    ++visited_len;
}

static bool visited_find(int64_t key)
{
    uint64_t h = hash_key(key) & (visited_cap - 1);
    while (visited_tbl[h].key != EMPTY_KEY) {
        if (visited_tbl[h].key == key) return true;
        h = (h + 1) & (visited_cap - 1);
    }
    return false;
}

static void init_spiral(void)
{
    if (spiral_initialized) return;
    kv_init();
    /* first cell (0,0) with digit 0 */
    seq_t d0 = (seq_t)next_digit();
    kv_insert(coord_key(0,0), d0);
    /* set generator for subsequent cells */
    gen_x = 0; gen_y = 0;
    gen_dir = 0; gen_step = 1; gen_done = 0;
    spiral_initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, safe fallback */
    init_spiral();

    if (visited_tbl == NULL) visited_init();

    int64_t cx = 0, cy = 0;
    int64_t start_key = coord_key(cx, cy);
    seq_t ans;
    if (!kv_find(start_key, &ans)) ans = 0;           /* safety */
    visited_insert(start_key);

    if (n == 1) return ans;

    static const int knight_dx[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    static const int knight_dy[8] = { 1, 2,  2,  1, -1, -2, -2, -1 };

    for (idx_t step = 2; step <= n; ++step) {
        int64_t best_x = 0, best_y = 0;
        seq_t best_d = 10;
        uint64_t best_dist = (uint64_t)(-1);
        int64_t best_key = 0;

        for (int m = 0; m < 8; ++m) {
            int64_t nx = cx + knight_dx[m];
            int64_t ny = cy + knight_dy[m];
            int64_t key = coord_key(nx, ny);
            if (visited_find(key)) continue;

            seq_t d;
            if (!kv_find(key, &d)) {
                /* generate until coordinate appears */
                while (!kv_find(key, &d)) gen_next_cell();
            }
            uint64_t dist = (uint64_t)(nx*nx + ny*ny);   /* distance from start */

            bool better = false;
            if (d < best_d) better = true;
            else if (d == best_d) {
                if (dist < best_dist) better = true;
                else if (dist == best_dist && nx < best_x) better = true;
            }
            if (better) {
                best_d = d;
                best_key = key;
                best_x = nx; best_y = ny;
                best_dist = dist;
            }
        }
        /* move the knight */
        cx = best_x; cy = best_y;
        visited_insert(best_key);
        ans = best_d;
    }
    /* free visited map to avoid memory leak on repeated calls */
    free(visited_tbl);
    visited_tbl = NULL;
    visited_cap = visited_len = 0;
    return ans;
}
