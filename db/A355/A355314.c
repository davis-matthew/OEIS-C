/*
 * A355314  Lexicographically earliest sequence of positive integers on a
 *          square spiral such that the difference between all orthogonally
 *          adjacent pairs of numbers is distinct.
 * https://oeis.org/A355314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lexicographically earliest sequence of values on a square
 *            spiral such that the absolute difference between every pair of
 *            orthogonally adjacent cells is unique. The sequence is produced
 *            by a breadth‑first construction of the spiral; a(0)=0, a(1)=0,
 *            and thereafter each new cell receives the...
 * Range:     u8 n <= 70, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- Helper structures ---- */

typedef struct {
    uint64_t key;
    seq_t    val;
    bool     used;
} KeyVal;

typedef struct {
    size_t   cap;      /* power of two */
    size_t   mask;     /* cap-1 */
    size_t   sz;       /* number of used entries */
    KeyVal  *table;
} Map;

typedef struct {
    size_t   bits;     /* total number of bits managed (power of two) */
    size_t   words;    /* number of uint64_t words */
    uint64_t *data;    /* bit array */
} DiffSet;

/* ---- Global static state ---- */

static Map      g_map       = {0,0,0,NULL};
static DiffSet  g_diff      = {0,0,NULL};
static seq_t  *g_seq        = NULL;   /* dynamic array of sequence values */
static size_t  g_seq_cap    = 0;      /* capacity of g_seq */
static size_t  g_seq_len    = 0;      /* number of values computed */
static int32_t g_x          = 0;      /* current spiral position x */
static int32_t g_y          = 0;      /* current spiral position y */
static int     g_dir        = 0;      /* direction (0:right,1:up,2:left,3:down) */
static int     g_step_len   = 1;      /* current leg length */
static int     g_step_rem   = 1;      /* steps remaining in current leg */
static int     g_leg_done   = 0;      /* number of legs completed */
static bool    g_initialized= false;    /* initialization flag */

/* ---- Utilities ---- */

/* Simple 64‑bit hash multiplier (Knuth's golden ratio) */
static inline size_t hash_uint64(uint64_t k) {
    return (size_t)((k * 11400714785074694791ULL) >> 1);
}

/* Key for coordinate pair (x,y) */
static inline uint64_t coord_key(int32_t x, int32_t y) {
    return ((uint64_t)(int64_t)x << 32) | (uint32_t)(int32_t)y;
}

/* Map operations -------------------------------------------------------- */

static void map_init(Map *m) {
    if (m->cap) return;
    m->cap   = 64;
    m->mask  = m->cap - 1;
    m->sz    = 0;
    m->table = (KeyVal*)calloc(m->cap, sizeof(KeyVal));
}

static void map_rehash(Map *m) {
    size_t new_cap = m->cap << 1;
    KeyVal *new_tbl = (KeyVal*)calloc(new_cap, sizeof(KeyVal));
    size_t new_mask = new_cap - 1;
    for (size_t i = 0; i < m->cap; ++i) {
        if (!m->table[i].used) continue;
        uint64_t k = m->table[i].key;
        size_t idx = hash_uint64(k) & new_mask;
        while (new_tbl[idx].used) idx = (idx + 1) & new_mask;
        new_tbl[idx].used = true;
        new_tbl[idx].key  = k;
        new_tbl[idx].val  = m->table[i].val;
    }
    free(m->table);
    m->table = new_tbl;
    m->cap   = new_cap;
    m->mask  = new_mask;
}

static void map_set(Map *m, uint64_t key, seq_t val) {
    map_init(m);
    if (m->sz * 2 > m->cap) map_rehash(m);
    size_t idx = hash_uint64(key) & m->mask;
    while (m->table[idx].used) {
        if (m->table[idx].key == key) {
            m->table[idx].val = val;
            return;
        }
        idx = (idx + 1) & m->mask;
    }
    m->table[idx].used = true;
    m->table[idx].key  = key;
    m->table[idx].val  = val;
    ++m->sz;
}

static seq_t* map_find(Map *m, uint64_t key) {
    if (!m->cap) return NULL;
    size_t idx = hash_uint64(key) & m->mask;
    while (m->table[idx].used) {
        if (m->table[idx].key == key) return &m->table[idx].val;
        idx = (idx + 1) & m->mask;
    }
    return NULL;
}

/* DiffSet operations ----------------------------------------------------- */

static void diff_init(DiffSet *d) {
    if (d->bits) return;
    d->bits  = 128;
    d->words = d->bits >> 6;
    d->data  = (uint64_t*)calloc(d->words, sizeof(uint64_t));
}

static void diff_expand(DiffSet *d, uint64_t need) {
    if (need < d->bits) return;
    size_t new_bits  = d->bits;
    while (new_bits <= need) new_bits <<= 1;
    size_t new_words = new_bits >> 6;
    uint64_t *new_data = (uint64_t*)calloc(new_words, sizeof(uint64_t));
    memcpy(new_data, d->data, d->words * sizeof(uint64_t));
    free(d->data);
    d->data  = new_data;
    d->bits  = new_bits;
    d->words = new_words;
}

static inline bool diff_test(const DiffSet *d, uint64_t v) {
    size_t idx = v >> 6;
    return (d->data[idx] >> (v & 63)) & 1ULL;
}

static inline void diff_set(DiffSet *d, uint64_t v) {
    diff_expand(d, v);
    size_t idx = v >> 6;
    d->data[idx] |= 1ULL << (v & 63);
}

/* Sequence array management ---------------------------------------------- */

static void seq_ensure(size_t needed) {
    if (g_seq_len >= needed) return;
    if (g_seq_cap == 0) g_seq_cap = 64;
    while (g_seq_cap < needed) g_seq_cap <<= 1;
    g_seq = (seq_t*)realloc(g_seq, g_seq_cap * sizeof(seq_t));
}

static void seq_push(seq_t v) {
    seq_ensure(g_seq_len + 1);
    g_seq[g_seq_len++] = v;
}

/* Spiral progression ----------------------------------------------------- */

static const int dx[4] = {1,0,-1,0};
static const int dy[4] = {0,1,0,-1};

static void advance_position(void) {
    if (g_step_rem == 0) {
        g_dir = (g_dir + 1) & 3;
        ++g_leg_done;
        if ((g_leg_done & 1) == 0) ++g_step_len;
        g_step_rem = g_step_len;
    }
    g_x += dx[g_dir];
    g_y += dy[g_dir];
    --g_step_rem;
}

/* Candidate selection ---------------------------------------------------- */

static seq_t choose_value(void) {
    /* list of neighboring values with their keys */
    seq_t  neigh[4];
    int    ncnt = 0;
    uint64_t neigh_key[4];
    int     neigh_used[4] = {0};

    /* gather existing neighbors */
    for (int d = 0; d < 4; ++d) {
        int32_t nx = g_x + dx[d];
        int32_t ny = g_y + dy[d];
        uint64_t k = coord_key(nx, ny);
        seq_t *p = map_find(&g_map, k);
        if (p) {
            neigh_key[ncnt] = k;
            neigh[ ncnt ]  = *p;
            neigh_used[ ncnt ] = 1;
            ++ncnt;
        }
    }

    /* search minimal candidate value */
    for (seq_t cand = 0;; ++cand) {
        bool ok = true;
        seq_t tmp_diffs[4];
        int tmp_cnt = 0;

        for (int i = 0; i < ncnt; ++i) {
            seq_t a = cand, b = neigh[i];
            seq_t d = (a >= b) ? (a - b) : (b - a);

            uint64_t di = (uint64_t)d;
            if (diff_test(&g_diff, di)) { ok = false; break; }
            /* duplicate among new diffs? */
            for (int j = 0; j < tmp_cnt; ++j)
                if (tmp_diffs[j] == d) { ok = false; break; }
            if (!ok) break;
            tmp_diffs[tmp_cnt++] = d;
        }

        if (ok) return cand;
    }
}

/* Main computation loop ---------------------------------------------- */

static void compute_up_to(idx_t n) {
    if (!g_initialized) {
        /* initialise structures */
        map_init(&g_map);
        diff_init(&g_diff);
        g_seq_cap = 64;
        g_seq = (seq_t*)malloc(g_seq_cap * sizeof(seq_t));
        g_seq_len = 0;

        /* first position (0,0) with value 0 */
        g_x = 0; g_y = 0;
        map_set(&g_map, coord_key(g_x, g_y), 0);
        seq_push(0);

        /* prepare to advance */
        g_dir = 0;
        g_step_len = 1;
        g_step_rem = 1;   /* first move will be right */
        g_leg_done = 0;
        g_initialized = true;
    }

    while (g_seq_len <= (size_t)n) {
        advance_position();                     /* new coordinate */

        seq_t val = choose_value();             /* find suitable value */

        /* record new value and update used differences */
        uint64_t key = coord_key(g_x, g_y);
        map_set(&g_map, key, val);
        seq_push(val);

        /* update difference set with new pairs */
        for (int d = 0; d < 4; ++d) {
            int32_t nx = g_x + dx[d];
            int32_t ny = g_y + dy[d];
            uint64_t k = coord_key(nx, ny);
            seq_t *p = map_find(&g_map, k);
            if (p) {
                seq_t a = val, b = *p;
                seq_t dval = (a >= b) ? (a - b) : (b - a);
                diff_set(&g_diff, (uint64_t)dval);
            }
        }
    }
}

/* ------------------------------------------------------------------- */
/* Public entry point required by the testing harness */
seq_t a(idx_t n) {
    compute_up_to(n);
    return g_seq[(size_t)n];
}
