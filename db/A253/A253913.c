/*
 * A253913  Numbers of the form m^k + m, with m >= 0 and k > 1.
 * https://oeis.org/A253913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer of the form m^k + m, with m ≥
 *            0, k > 1, with duplicates removed and the sequence sorted
 * Range:     u8 n <= 13, u16 n <= 57, u32 n <= 1814, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   less_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence is computed via a min‑heap of candidate values; code is O(n log n). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/less_seq.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Heap node stores the current value m^k+m, the base m, and the current power m^(k-1). */
struct Node {
    seq_t val;   /* current value m^k + m */
    uint64_t m;  /* base m */
    seq_t m_pow; /* current power m^(k-1)  (so val = m_pow * m + m) */
};

struct Heap {
    struct Node *data;
    size_t size;
    size_t capacity;
};

static struct Heap heap = {NULL, 0, 0};
static uint64_t max_m_pushed = 1; /* last base m that has been pushed into the heap */

static void heap_swap(struct Node *a, struct Node *b)
{
    struct Node tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_sift_up(struct Heap *h, size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (!less_seq(h->data[i].val, h->data[parent].val))
            break;
        heap_swap(&h->data[i], &h->data[parent]);
        i = parent;
    }
}

static void heap_sift_down(struct Heap *h, size_t i)
{
    size_t n = h->size;
    while (true) {
        size_t left = (i << 1) + 1;
        if (left >= n) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < n && less_seq(h->data[right].val, h->data[left].val))
            smallest = right;
        if (!less_seq(h->data[smallest].val, h->data[i].val))
            break;
        heap_swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
}

static void heap_ensure_cap(size_t needed)
{
    if (heap.capacity < needed) {
        size_t new_cap = heap.capacity ? heap.capacity << 1 : 8;
        while (new_cap < needed) new_cap <<= 1;
        struct Node *new_data = (struct Node *)realloc(heap.data, new_cap * sizeof(struct Node));
        if (!new_data) abort();
        heap.data = new_data;
        heap.capacity = new_cap;
    }
}

static void heap_push(struct Node node)
{
    heap_ensure_cap(heap.size + 1);
    heap.data[heap.size++] = node;
    heap_sift_up(&heap, heap.size - 1);
}

static struct Node heap_pop(void)
{
    struct Node ret = heap.data[0];
    heap.size--;
    if (heap.size > 0) {
        heap.data[0] = heap.data[heap.size];
        heap_sift_down(&heap, 0);
    }
    return ret;
}

/* ---------- cache utilities ---------- */
static void cache_ensure(size_t bound)
{
    if (cache_cap < bound) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap < bound) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* ---------- sequence generation ---------- */
static void generate_terms(size_t n)
{
    if (cache_len >= n) return;
    cache_ensure(n);

    /* Initialise cache for the very first terms if not already done. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;          /* m = 0 gives 0. */
        cache_len = 1;
    }

    if (n >= 2 && cache_len < 2) {
        cache[1] = (seq_t)2;          /* m = 1 gives 2, no need to push it into heap. */
        cache_len = 2;
    }
    if (cache_len >= n) return;

    /* Insert new bases up to a safe upper bound. */
    size_t m_limit = n + 1;  /* ensures enough m’s to cover first n terms */
    if (m_limit > max_m_pushed) {
        for (uint64_t m = max_m_pushed + 1; m <= m_limit; ++m) {
            seq_t m_seq = (seq_t)m;
            seq_t m_pow = m_seq * m_seq;   /* start with k = 2: m^2 */
            seq_t val   = m_pow + m_seq;   /* m^2 + m */
            heap_push((struct Node){val, m, m_pow});
        }
        max_m_pushed = m_limit;
    }

    seq_t last_val = cache[cache_len - 1];
    while (cache_len < n) {
        struct Node cur = heap_pop();

        /* Skip if this value equals the last distinct value inserted. */
        if (cur.val != last_val) {
            cache[cache_len++] = cur.val;
            last_val = cur.val;
        }

        /* Compute next value for the same base m: k → k+1 */
        seq_t next_pow = cur.m_pow * (seq_t)cur.m;          /* m^(k+1) */
        seq_t next_val = next_pow + (seq_t)cur.m;           /* m^(k+1) + m */
        heap_push((struct Node){next_val, cur.m, next_pow});
    }
}

/* ---------- required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset: undefined, return 0 */
    generate_terms((size_t)n);
    return cache[(size_t)(n - 1)];
}
