/*
 * A358503  Positions inventory sequence: for stage k >= 2 we record where all
 *          the numbers from the two previous stages have appeared, starting
 *          with a(0) = 0, a(1) = 0.
 * https://oeis.org/A358503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Positions inventory sequence (OEIS A358503)
 * a(0)=0, a(1)=0.
 * For k≥2, the k‑th row consists of the absolute indices of all
 * occurrences of each number that appears in the two preceding rows,
 * sorted by numerical value of the number and then by index.
 * The function a(idx_t n) returns the n‑th element of this infinite sequence.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Simple hash map from seq_t (value in the sequence) to a dynamic    */
/* array of idx_t (positions where that value occurs).                 */
/* ------------------------------------------------------------------ */
typedef struct Entry {
    seq_t  key;      /* value present in the sequence */
    idx_t *pos;      /* array of positions where key occurs */
    size_t len;      /* number of positions stored */
    size_t cap;      /* capacity of pos array */
} Entry;

typedef struct {
    Entry *table;    /* array of entries, size always power of 2 */
    size_t sz;       /* number of used entries */
    size_t cap;      /* capacity (power of two) */
} HashMap;

static const seq_t EMPTY_KEY = (seq_t)(~(seq_t)0);

/* hash64: splitmix64 for 64‑bit keys */
static size_t hash64(seq_t x)
{
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return (size_t)(x ^ (x >> 31));
}

static void hashmap_init(HashMap *hm, size_t init_cap)
{
    hm->cap = init_cap;
    hm->sz = 0;
    hm->table = (Entry *)calloc(hm->cap, sizeof(Entry));
    for (size_t i = 0; i < hm->cap; ++i)
        hm->table[i].key = EMPTY_KEY;
}

static void hashmap_grow(HashMap *hm)
{
    size_t new_cap = hm->cap << 1;
    Entry *new_tbl = (Entry *)calloc(new_cap, sizeof(Entry));
    for (size_t i = 0; i < new_cap; ++i)
        new_tbl[i].key = EMPTY_KEY;

    for (size_t i = 0; i < hm->cap; ++i) {
        if (hm->table[i].key != EMPTY_KEY) {
            size_t h = hash64(hm->table[i].key) & (new_cap - 1);
            while (new_tbl[h].key != EMPTY_KEY) h = (h + 1) & (new_cap - 1);
            new_tbl[h] = hm->table[i];
        }
    }
    free(hm->table);
    hm->table = new_tbl;
    hm->cap = new_cap;
}

static Entry *hashmap_find_entry(HashMap *hm, seq_t key)
{
    size_t h = hash64(key) & (hm->cap - 1);
    while (hm->table[h].key != EMPTY_KEY) {
        if (hm->table[h].key == key) return &hm->table[h];
        h = (h + 1) & (hm->cap - 1);
    }
    return NULL;
}

static Entry *hashmap_get_entry(HashMap *hm, seq_t key)
{
    Entry *e = hashmap_find_entry(hm, key);
    if (e) return e;

    if ((hm->sz + 1) * 2 > hm->cap) hashmap_grow(hm);

    size_t h = hash64(key) & (hm->cap - 1);
    while (hm->table[h].key != EMPTY_KEY) h = (h + 1) & (hm->cap - 1);
    e = &hm->table[h];
    e->key = key;
    e->pos = NULL;
    e->len = 0;
    e->cap = 0;
    hm->sz++;
    return e;
}

static void hashmap_insert_idx(Entry *e, idx_t pos)
{
    if (e->len == e->cap) {
        size_t new_cap = e->cap ? e->cap << 1 : 4;
        e->pos = (idx_t *)realloc(e->pos, new_cap * sizeof(idx_t));
        e->cap = new_cap;
    }
    e->pos[e->len++] = pos;
}

/* -------------------------------------------------------------- */
/* Dynamic array holding the sequence itself                     */
/* -------------------------------------------------------------- */
static seq_t *SEQ = NULL;
static size_t SEQ_CAP = 0;
static size_t SEQ_LEN = 0;

static void seq_ensure(size_t needed)
{
    if (needed <= SEQ_CAP) return;
    size_t new_cap = SEQ_CAP ? SEQ_CAP << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    SEQ = (seq_t *)realloc(SEQ, new_cap * sizeof(seq_t));
    SEQ_CAP = new_cap;
}

/* -------------------------------------------------------------- */
/* Row metadata: absolute start index and length per row          */
/* -------------------------------------------------------------- */
static idx_t *ROW_START = NULL;
static idx_t *ROW_LEN   = NULL;
static size_t ROW_CAP   = 0;
static size_t ROW_CNT   = 0;

static void rows_ensure(size_t needed)
{
    if (needed <= ROW_CAP) return;
    size_t new_cap = ROW_CAP ? ROW_CAP << 1 : 32;
    while (new_cap < needed) new_cap <<= 1;
    ROW_START = (idx_t *)realloc(ROW_START, new_cap * sizeof(idx_t));
    ROW_LEN   = (idx_t *)realloc(ROW_LEN,   new_cap * sizeof(idx_t));
    ROW_CAP   = new_cap;
}

/* -------------------------------------------------------------- */
/* Comparator for qsort of seq_t values                          */
/* -------------------------------------------------------------- */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* -------------------------------------------------------------- */
/* Helper: lower bound on an array of idx_t                      */
/* -------------------------------------------------------------- */
static size_t lower_bound_idx(const idx_t *arr, size_t len, idx_t target)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

/* -------------------------------------------------------------- */
/* Main sequence function                                         */
/* -------------------------------------------------------------- */
static HashMap NUM_MAP;    /* map from value to (global) positions array */

seq_t a(idx_t n)
{
    /* first call – initialise basic structures */
    if (SEQ == NULL) {
        seq_ensure(2);
        SEQ[0] = SEQ[1] = (seq_t)0;
        SEQ_LEN = 2;

        rows_ensure(2);
        ROW_START[0] = 0; ROW_LEN[0] = 1;
        ROW_START[1] = 1; ROW_LEN[1] = 1;
        ROW_CNT = 2;

        hashmap_init(&NUM_MAP, 1024);

        /* register the two occurrences of 0 */
        Entry *e0 = hashmap_get_entry(&NUM_MAP, (seq_t)0);
        hashmap_insert_idx(e0, 0);
        hashmap_insert_idx(e0, 1);
    }

    /* generate rows until we have enough elements */
    while (SEQ_LEN <= (size_t)n) {
        /* indices of the two previous rows */
        idx_t start_prevprev = ROW_START[ROW_CNT-2];
        idx_t len_prevprev   = ROW_LEN[ROW_CNT-2];
        idx_t start_prev     = ROW_START[ROW_CNT-1];
        idx_t len_prev       = ROW_LEN[ROW_CNT-1];

        /* collect all numbers from the two previous rows */
        size_t uniq_size = len_prevprev + len_prev;
        seq_t *temp = (seq_t *)malloc(uniq_size * sizeof(seq_t));
        size_t ui = 0;
        for (idx_t i = 0; i < len_prevprev; ++i) temp[ui++] = SEQ[start_prevprev + i];
        for (idx_t i = 0; i < len_prev;     ++i) temp[ui++] = SEQ[start_prev + i];

        /* sort and deduplicate */
        qsort(temp, uniq_size, sizeof(seq_t), cmp_seq_t);
        size_t uniq_cnt = 0;
        for (size_t i = 0; i < uniq_size; ++i) {
            if (i == 0 || temp[i] != temp[i-1]) temp[uniq_cnt++] = temp[i];
        }

        /* compute length of the new row */
        size_t new_row_len = 0;
        for (size_t i = 0; i < uniq_cnt; ++i) {
            Entry *e = hashmap_find_entry(&NUM_MAP, temp[i]);
            if (!e) continue;
            /* first predecessor row */
            idx_t end1 = start_prevprev + len_prevprev - 1;
            size_t pos = lower_bound_idx(e->pos, e->len, start_prevprev);
            while (pos < e->len && e->pos[pos] <= end1) { ++new_row_len; ++pos; }
            /* second predecessor row */
            idx_t end2 = start_prev + len_prev - 1;
            pos = lower_bound_idx(e->pos, e->len, start_prev);
            while (pos < e->len && e->pos[pos] <= end2) { ++new_row_len; ++pos; }
        }

        /* ensure capacity */
        seq_ensure(SEQ_LEN + new_row_len);
        rows_ensure(ROW_CNT + 1);

        idx_t new_start = SEQ_LEN;

        /* append the new row values and register their positions */
        for (size_t i = 0; i < uniq_cnt; ++i) {
            Entry *e = hashmap_find_entry(&NUM_MAP, temp[i]);
            if (!e) continue;

            /* first predecessor row */
            idx_t end1 = start_prevprev + len_prevprev - 1;
            size_t pos = lower_bound_idx(e->pos, e->len, start_prevprev);
            while (pos < e->len && e->pos[pos] <= end1) {
                seq_t val = (seq_t)e->pos[pos];
                SEQ[SEQ_LEN++] = val;
                Entry *e2 = hashmap_get_entry(&NUM_MAP, val);
                hashmap_insert_idx(e2, SEQ_LEN - 1);
                ++pos;
            }

            /* second predecessor row */
            idx_t end2 = start_prev + len_prev - 1;
            pos = lower_bound_idx(e->pos, e->len, start_prev);
            while (pos < e->len && e->pos[pos] <= end2) {
                seq_t val = (seq_t)e->pos[pos];
                SEQ[SEQ_LEN++] = val;
                Entry *e2 = hashmap_get_entry(&NUM_MAP, val);
                hashmap_insert_idx(e2, SEQ_LEN - 1);
                ++pos;
            }
        }

        ROW_START[ROW_CNT] = new_start;
        ROW_LEN[ROW_CNT]   = (idx_t)new_row_len;
        ROW_CNT++;

        free(temp);
    }

    return SEQ[(size_t)n];
}
