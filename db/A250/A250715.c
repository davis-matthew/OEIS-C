/*
 * A250715  Numbers of the form 7^x + y^7 with x, y >= 0.
 * https://oeis.org/A250715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers of the form 7^x + y^7 with x,y ≥ 0, sorted
 *            ascending and duplicates removed
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/pow_uint.h"

/* ---------- binary min‑heap of (value,x,y) ---------- */
static struct Node {
    seq_t val;
    idx_t x;
    idx_t y;
} *heap = NULL;
static size_t heap_cap = 0;
static size_t heap_len = 0;

static void heap_grow(size_t needed) {
    if (heap_cap >= needed) return;
    size_t n = heap_cap ? heap_cap * 2 : 16;
    if (n < needed) n = needed;
    heap = (struct Node *)realloc(heap, n * sizeof(struct Node));
    heap_cap = n;
}

static void heap_push(struct Node node) {
    heap_grow(heap_len + 1);
    size_t i = heap_len++;
    heap[i] = node;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        struct Node tmp = heap[p]; heap[p] = heap[i]; heap[i] = tmp;
        i = p;
    }
}

static struct Node heap_pop(void) {
    struct Node ret = heap[0];
    heap[0] = heap[--heap_len];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= heap_len) break;
        size_t r = l + 1;
        size_t j = (r < heap_len && heap[r].val < heap[l].val) ? r : l;
        if (heap[i].val <= heap[j].val) break;
        struct Node tmp = heap[i]; heap[i] = heap[j]; heap[j] = tmp;
        i = j;
    }
    return ret;
}

/* ---------- sequence cache ---------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void cache_grow(size_t needed) {
    if (cache_cap >= needed) return;
    size_t n = cache_cap ? cache_cap * 2 : 16;
    if (n < needed) n = needed;
    cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    cache_cap = n;
}

/* ---------- powers of 7 ---------- */
static seq_t *pow7_arr = NULL;
static size_t pow7_len = 0;
static size_t pow7_cap = 0;
static bool pow7_over = false;

static void pow7_grow(size_t needed) {
    if (pow7_cap >= needed) return;
    size_t n = pow7_cap ? pow7_cap * 2 : 8;
    while (n < needed) n <<= 1;
    pow7_arr = (seq_t *)realloc(pow7_arr, n * sizeof(seq_t));
    pow7_cap = n;
}

static void pow7_array_init(void) {
    pow7_grow(8);
    pow7_len = 1;
    pow7_arr[0] = (seq_t)1;
}

static void pow7_extend(idx_t x) {
    if (pow7_over || x < pow7_len) return;
    pow7_grow((size_t)x + 1);
    size_t i;
    for (i = pow7_len; i <= (size_t)x; ++i) {
        if (pow7_over) break;
        seq_t prev = pow7_arr[i - 1];
        seq_t cur  = prev * (seq_t)7;
        if (cur < prev) {          /* unsigned overflow detected */
            pow7_over = true;
            break;
        }
        pow7_arr[i] = cur;
    }
    pow7_len = i;                  /* first overflow index, or x+1 if none */
}

/* ---------- powers of y^7 ---------- */
static seq_t *pow7th_arr = NULL;
static size_t pow7th_len = 0;
static size_t pow7th_cap = 0;
static bool pow7th_over = false;

static void pow7th_grow(size_t needed) {
    if (pow7th_cap >= needed) return;
    size_t n = pow7th_cap ? pow7th_cap * 2 : 8;
    while (n < needed) n <<= 1;
    pow7th_arr = (seq_t *)realloc(pow7th_arr, n * sizeof(seq_t));
    pow7th_cap = n;
}

static void pow7th_array_init(void) {
    pow7th_grow(8);
    pow7th_len = 1;
    pow7th_arr[0] = (seq_t)0;      /* 0^7 = 0 */
}

static void pow7th_extend(idx_t y) {
    if (pow7th_over || y < pow7th_len) return;
    pow7th_grow((size_t)y + 1);
    size_t i;
    for (i = pow7th_len; i <= (size_t)y; ++i) {
        if (pow7th_over) break;
        seq_t v = pow_uint((uint64_t)i, (idx_t)7);
        if (i > 0 && v == (seq_t)0) {      /* unsigned overflow detected */
            pow7th_over = true;
            break;
        }
        pow7th_arr[i] = v;
    }
    pow7th_len = i;
}

/* ---------- visited‐pairs set (open addressing) ---------- */
static uint64_t *visit_table = NULL;
static size_t visit_cap   = 0;
static size_t visit_used  = 0;
static const uint64_t VISIT_SENTINEL = UINT64_MAX;

static void visit_grow(size_t needed) {
    if (visit_cap >= needed) return;
    size_t n = 8;
    while (n < needed) n <<= 1;
    uint64_t *newtbl = (uint64_t *)malloc(n * sizeof(uint64_t));
    memset(newtbl, 0xFF, n * sizeof(uint64_t));          /* initialise to sentinel */
    for (size_t i = 0; i < visit_cap; ++i) {
        uint64_t key = visit_table[i];
        if (key == VISIT_SENTINEL) continue;
        size_t h = key & (n - 1);
        while (newtbl[h] != VISIT_SENTINEL) h = (h + 1) & (n - 1);
        newtbl[h] = key;
    }
    free(visit_table);
    visit_table = newtbl;
    visit_cap   = n;
}

static bool visit_contains(uint64_t key) {
    if (visit_cap == 0) return false;
    size_t h = key & (visit_cap - 1);
    while (1) {
        uint64_t k = visit_table[h];
        if (k == VISIT_SENTINEL) return false;
        if (k == key) return true;
        h = (h + 1) & (visit_cap - 1);
    }
}

static void visit_insert(uint64_t key) {
    if (visit_contains(key)) return;
    visit_used++;
    if (visit_used * 3 > visit_cap * 2) visit_grow(visit_cap * 2);
    size_t h = key & (visit_cap - 1);
    while (visit_table[h] != VISIT_SENTINEL) h = (h + 1) & (visit_cap - 1);
    visit_table[h] = key;
}

/* ---------- enqueue a valid new pair ---------- */
static void try_push_pair(idx_t x, idx_t y) {
    uint64_t key = ((uint64_t)x << 32) | (uint64_t)y;
    if (visit_contains(key)) return;
    visit_insert(key);

    pow7_extend(x);
    pow7th_extend(y);

    if (x >= pow7_len || y >= pow7th_len) return;

    seq_t v = pow7_arr[x] + pow7th_arr[y];
    /* unsigned overflow check */
    if (v < pow7_arr[x] || v < pow7th_arr[y]) return;

    heap_push((struct Node){v, x, y});
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, but safe return */
    static bool init = false;
    if (!init) {
        init = true;
        cache_grow(16);
        pow7_array_init();
        pow7th_array_init();
        visit_grow(16);
        visit_insert(0);            /* (0,0) visited */
        heap_grow(16);
        heap_push((struct Node){(seq_t)1, 0, 0});   /* 7^0 + 0^7 = 1 */
    }
    while (cache_len < n) {
        struct Node node = heap_pop();
        if (cache_len == 0 || node.val != cache[cache_len - 1]) {
            cache_grow(cache_len + 1);
            cache[cache_len++] = node.val;
        }
        try_push_pair(node.x + 1, node.y);
        try_push_pair(node.x, node.y + 1);
    }
    return cache[n - 1];
}
