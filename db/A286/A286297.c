/*
 * A286297  Irregular triangle read by rows: successive rows have lengths
 *          1,3,5,7,..., and are filled in across rows with the smallest
 *          nonnegative number such that there is no repeat in any row,
 *          column, or diagonal of slope +-1.
 * https://oeis.org/A286297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9876, u16 n <= 9876, u32 n <= 9876, u64 n <= 9876,
 *            u128 n <= 9876, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A286297 (irregular triangle).  The triangle is built
   iteratively: each entry is the smallest non‑negative integer that
   does not duplicate a value in its row, vertical column, or either
   ±1 diagonal.  This routine maintains the triangle state in static
   storage so that calls in any order work correctly. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---- Bit‑set for a line --------------------------------------------- */
typedef struct {
    uint64_t *data;   /* 64‑bit blocks */
    size_t   nblocks; /* allocated blocks */
} Bits;

static inline void bits_set(Bits *b, unsigned long long v)
{
    size_t block = v >> 6;
    size_t bit   = (size_t)(v & 63ULL);
    if (block >= b->nblocks) {
        size_t new_blocks = block + 1;
        uint64_t *new_data = (uint64_t *)realloc(b->data, new_blocks * sizeof(uint64_t));
        if (!new_data) abort();
        for (size_t i = b->nblocks; i < new_blocks; ++i) new_data[i] = 0;
        b->data = new_data;
        b->nblocks = new_blocks;
    }
    b->data[block] |= (uint64_t)1 << bit;
}

static inline bool bits_used(const Bits *b, unsigned long long v)
{
    if (!b) return false;
    size_t block = v >> 6;
    size_t bit   = (size_t)(v & 63ULL);
    return block < b->nblocks && ((b->data[block] >> bit) & 1ULL);
}

/* ---- Simple hash map for lines that use possibly negative keys -------- */
typedef struct {
    int  key;
    Bits *bits;      /* NULL until first value inserted */
} KV;

typedef struct {
    KV *data;
    size_t count;
    size_t capacity;
} Map;

static Map* map_create(void)
{
    Map *m = (Map *)malloc(sizeof(Map));
    if (!m) abort();
    m->capacity = 16;
    m->count = 0;
    m->data = (KV *)malloc(m->capacity * sizeof(KV));
    if (!m->data) abort();
    return m;
}

static KV* map_find(Map *m, int key)
{
    for (size_t i = 0; i < m->count; ++i)
        if (m->data[i].key == key) return &m->data[i];
    return NULL;
}

static KV* map_insert(Map *m, int key)
{
    if (m->count == m->capacity) {
        m->capacity <<= 1;
        m->data = (KV *)realloc(m->data, m->capacity * sizeof(KV));
        if (!m->data) abort();
    }
    KV *kv = &m->data[m->count++];
    kv->key = key;
    kv->bits = NULL;
    return kv;
}

static bool map_used(Map *m, int key, unsigned long long v)
{
    KV *kv = map_find(m, key);
    return kv && bits_used(kv->bits, v);
}

static void map_set(Map *m, int key, unsigned long long v)
{
    KV *kv = map_find(m, key);
    if (!kv) kv = map_insert(m, key);
    if (!kv->bits) {
        kv->bits = (Bits *)malloc(sizeof(Bits));
        if (!kv->bits) abort();
        kv->bits->data = NULL;
        kv->bits->nblocks = 0;
    }
    bits_set(kv->bits, v);
}

/* ---- Row and column bitsets ------------------------------------------ */
static Bits **row_bits = NULL;
static size_t row_cap = 0;

static Bits **col_bits = NULL;
static size_t col_cap = 0;

static void ensure_row(size_t r)
{
    if (r >= row_cap) {
        size_t new_cap = r + 1;
        row_bits = (Bits **)realloc(row_bits, new_cap * sizeof(Bits *));
        if (!row_bits) abort();
        for (size_t i = row_cap; i < new_cap; ++i) row_bits[i] = NULL;
        row_cap = new_cap;
    }
    if (!row_bits[r]) {
        row_bits[r] = (Bits *)malloc(sizeof(Bits));
        if (!row_bits[r]) abort();
        row_bits[r]->data = NULL;
        row_bits[r]->nblocks = 0;
    }
}

static void ensure_col(size_t c)
{
    if (c >= col_cap) {
        size_t new_cap = c + 1;
        col_bits = (Bits **)realloc(col_bits, new_cap * sizeof(Bits *));
        if (!col_bits) abort();
        for (size_t i = col_cap; i < new_cap; ++i) col_bits[i] = NULL;
        col_cap = new_cap;
    }
    if (!col_bits[c]) {
        col_bits[c] = (Bits *)malloc(sizeof(Bits));
        if (!col_bits[c]) abort();
        col_bits[c]->data = NULL;
        col_bits[c]->nblocks = 0;
    }
}

/* ---- Sequence cache ----------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* ---- Current position in the triangle ------------------------------ */
static size_t next_row = 0;
static size_t next_col = 0;

/* ---- Vertical and diagonal line maps ------------------------------ */
static Map *vert_map = NULL;
static Map *diag_map = NULL;

/* ---- Main routine ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* One‑time initialization of maps */
    static bool initialized = false;
    if (!initialized) {
        vert_map = map_create();   /* vertical columns */
        diag_map = map_create();   /* slope ±1 diagonals */
        initialized = true;
    }

    /* Build the triangle up to the requested index */
    while (cache_len <= (size_t)n) {
        /* Ensure bitsets for the current position exist */
        ensure_row(next_row);
        ensure_col(next_col);

        /* Keys for the vertical and diagonal lines */
        int vcol = (int)next_col - (int)next_row;        /* vertical */
        int dkey = (int)next_col - 2 * (int)next_row;    /* slope  +1 diag */

        /* Smallest unused value for this cell */
        unsigned long long val = 0;
        for (;;) {
            bool used = bits_used(row_bits[next_row], val) ||
                        bits_used(col_bits[next_col], val) ||
                        map_used(vert_map, vcol, val) ||
                        map_used(diag_map, dkey, val);
            if (!used) break;
            ++val;
        }

        /* Record the value */
        seq_t val_seq = (seq_t)val;
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? 2 * cache_cap : 16;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!cache) abort();
            cache_cap = new_cap;
        }
        cache[cache_len++] = val_seq;

        /* Update all relevant lines */
        bits_set(row_bits[next_row], val);
        bits_set(col_bits[next_col], val);
        map_set(vert_map, vcol, val);
        map_set(diag_map, dkey, val);

        /* Advance to the next position */
        ++next_col;
        if (next_col == (size_t)(2 * next_row + 1)) {
            ++next_row;
            next_col = 0;
        }
    }

    return cache[(size_t)n];
}
