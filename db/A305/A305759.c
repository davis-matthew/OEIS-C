/*
 * A305759  Numbers that can be factored as a product of numbers of the form
 *          2^k+1 (A000051).
 * https://oeis.org/A305759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the OEIS sequence A305759: numbers that can be factored as a
   product of terms of the form 2^k + 1, in ascending order.
   The sequence contains 1 as the first term. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* --- Hash set for deduplication ------------------------------------- */
typedef struct {
    seq_t *tab;   /* keys */
    uint8_t *used;/* 0 empty, 1 occupied */
    size_t size;  /* power of two */
    size_t count;
} hash_set_t;

static uint64_t hash64(seq_t key)
{
    uint64_t k = (uint64_t)key;            /* works for all supported seq_t */
    /* splitmix64 avalanche */
    k += 0x9e3779b97f4a7c15ULL;
    k = (k ^ (k >> 30)) * 0xbf58476d1ce4e5b9ULL;
    k = (k ^ (k >> 27)) * 0x94d049bb133111ebULL;
    return k;
}

static void hash_init(hash_set_t *h, size_t init_size)
{
    h->size = init_size;
    h->count = 0;
    h->tab = (seq_t *)malloc(init_size * sizeof(seq_t));
    h->used = (uint8_t *)calloc(init_size, sizeof(uint8_t));
}

static bool hash_contains(const hash_set_t *h, seq_t key)
{
    size_t idx = hash64(key) & (h->size - 1);
    while (h->used[idx]) {
        if (h->tab[idx] == key) return true;
        idx = (idx + 1) & (h->size - 1);
    }
    return false;
}

static bool hash_insert(hash_set_t *h, seq_t key)
{
    if (hash_contains(h, key)) return false;
    size_t idx = hash64(key) & (h->size - 1);
    while (h->used[idx]) idx = (idx + 1) & (h->size - 1);
    h->used[idx] = 1;
    h->tab[idx] = key;
    ++h->count;
    return true;
}

/* --- Min‑heap of candidates ---------------------------------------- */
static seq_t *heap = NULL;
static size_t heap_cap = 0;
static size_t heap_sz = 0;

static void heap_init(void)
{
    heap_cap = 1024;
    heap = (seq_t *)malloc(heap_cap * sizeof(seq_t));
    heap_sz = 0;
}

static void heap_grow(size_t need)
{
    while (heap_cap <= need) heap_cap <<= 1;
    heap = (seq_t *)realloc(heap, heap_cap * sizeof(seq_t));
}

static void heap_push(seq_t val)
{
    if (heap_sz == heap_cap) heap_grow(heap_sz + 1);
    size_t i = heap_sz++;
    heap[i] = val;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p] <= heap[i]) break;
        seq_t tmp = heap[p]; heap[p] = heap[i]; heap[i] = tmp;
        i = p;
    }
}

static seq_t heap_pop(void)
{
    seq_t res = heap[0];
    heap[0] = heap[--heap_sz];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) | 1;
        if (l >= heap_sz) break;
        size_t r = l + 1;
        size_t j = (r < heap_sz && heap[r] < heap[l]) ? r : l;
        if (heap[i] <= heap[j]) break;
        seq_t tmp = heap[i]; heap[i] = heap[j]; heap[j] = tmp;
        i = j;
    }
    return res;
}

/* --- Sequence generation ------------------------------------------- */
#define MAX_EXP 30                /* covers 2^k+1 up to 2^30+1 */
#define INITIAL_SEQ_CAP 4096
#define HASH_INIT_SIZE (1ULL << 16)

static seq_t *seq_vals = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

static seq_t base_vals[MAX_EXP + 1];
static size_t base_cnt = 0;

static hash_set_t visited;
static bool init_done = false;

static void generate_sequence_to(idx_t target)
{
    if (!init_done) {
        /* initial base numbers 2^k + 1 */
        for (idx_t k = 0; k <= MAX_EXP; ++k) {
            seq_t p = seq_pow((seq_t)2, (idx_t)k);
            p = p + (seq_t)1;
            base_vals[base_cnt++] = p;
        }
        heap_init();
        hash_init(&visited, HASH_INIT_SIZE);
        hash_insert(&visited, (seq_t)1);
        heap_push((seq_t)1);
        seq_cap = INITIAL_SEQ_CAP;
        seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        init_done = true;
    }

    while (seq_len < target) {
        if (heap_sz == 0) break; /* should not happen */
        seq_t cur = heap_pop();

        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
        }
        seq_vals[seq_len++] = cur;

        for (size_t i = 0; i < base_cnt; ++i) {
            seq_t prod = seq_mul(cur, base_vals[i]);
            if (hash_insert(&visited, prod)) heap_push(prod);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 */
    if (seq_len < n) generate_sequence_to(n);
    return seq_vals[n - 1];
}
