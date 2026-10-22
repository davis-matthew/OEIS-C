/*
 * A334012  a(n) is the least integer that can be expressed as the sum of one
 *          or more consecutive nonzero octagonal numbers in exactly n ways.
 * https://oeis.org/A334012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   octagonal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute sums of consecutive octagonal numbers
   and count representations to obtain a(n) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/octagonal.h"

/* Node used in the min‑heap – each node represents one running sum
   of consecutive octagonal numbers. */
struct Node {
    seq_t sum;          /* current sum of the run */
    idx_t k;            /* starting index of the run (1‑based) */
    idx_t m;            /* length of the run so far (≥1) */
};

/* Simple binary min‑heap for struct Node */
struct Heap {
    struct Node *data;
    size_t     size;
    size_t     cap;
};

static void heap_init(struct Heap *h)
{
    h->data = NULL;
    h->size = 0;
    h->cap  = 0;
}

static bool heap_empty(const struct Heap *h)
{
    return h->size == 0;
}

static void heap_push(struct Heap *h, struct Node n)
{
    if (h->size == h->cap) {
        size_t new_cap = (h->cap == 0) ? 8 : h->cap * 2;
        struct Node *tmp = (struct Node *)realloc(h->data, sizeof(struct Node) * new_cap);
        if (!tmp) return; /* allocation failure – let caller handle */
        h->data = tmp;
        h->cap  = new_cap;
    }
    size_t i = h->size++;
    h->data[i] = n;
    /* bubble up */
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (h->data[parent].sum <= h->data[i].sum) break;
        struct Node tmp = h->data[parent];
        h->data[parent] = h->data[i];
        h->data[i]      = tmp;
        i = parent;
    }
}

static struct Node heap_pop(struct Heap *h)
{
    struct Node root = h->data[0];
    h->size--;
    if (h->size == 0) return root;
    h->data[0] = h->data[h->size];
    /* bubble down */
    size_t i = 0;
    while (1) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < h->size && h->data[left].sum < h->data[smallest].sum)
            smallest = left;
        if (right < h->size && h->data[right].sum < h->data[smallest].sum)
            smallest = right;
        if (smallest == i) break;
        struct Node tmp = h->data[i];
        h->data[i] = h->data[smallest];
        h->data[smallest] = tmp;
        i = smallest;
    }
    return root;
}

/* Static state used by the algorithm */
static struct Heap    pq            = {NULL, 0, 0};
static idx_t          next_k        = 1;   /* next starting index to insert */
static seq_t         *cache         = NULL;
static size_t          cache_size    = 0;
static idx_t          max_computed_n = 0;   /* largest n for which cache[] has a value */
static size_t          found_count   = 0;   /* how many a(n) values we have found */
static bool            initialized   = false;
static seq_t           last_val;
static bool            has_last      = false;
static idx_t           current_count = 0;

/* Ensure that a(n) values for all 1 ≤ i ≤ target are computed. */
static void ensure(idx_t target /* target n */)
{
    if (!initialized) {
        /* initialise cache */
        cache_size = 2;  /* index 0 unused, we need room for a(1) */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return; /* allocation error – caller will fail if cache is NULL */
        for (size_t i = 0; i < cache_size; ++i) cache[i] = 0;
        heap_init(&pq);
        next_k = 1;
        has_last = false;
        initialized = true;
    }

    if (max_computed_n >= target) return;   /* nothing to do */

    /* grow cache if needed */
    if (cache_size <= target) {
        size_t new_size = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return; /* allocation error */
        cache = tmp;
        for (size_t i = cache_size; i < new_size; ++i) cache[i] = 0;
        cache_size = new_size;
    }

    while (found_count < target) {
        /* Insert new starting positions whose first octagonal number
           is not larger than the current smallest sum in the heap. */
        while (1) {
            seq_t first = octagonal(next_k);
            if (heap_empty(&pq) || first <= pq.data[0].sum) {
                struct Node node = { first, next_k, 1 };
                heap_push(&pq, node);
                ++next_k;
            } else {
                break;
            }
        }

        if (heap_empty(&pq)) break;   /* no more sums available */

        struct Node cur = heap_pop(&pq);
        seq_t val = cur.sum;

        if (!has_last) {
            last_val = val;
            current_count = 1;
            has_last = true;
        } else if (val == last_val) {
            ++current_count;
        } else {
            /* finalize the previous sum */
            if ((size_t)current_count <= target && cache[current_count] == 0) {
                cache[current_count] = last_val;
                ++found_count;
            }
            last_val = val;
            current_count = 1;
        }

        /* push next sum for this starting index */
        idx_t new_m = cur.m + 1;
        seq_t new_sum = cur.sum + octagonal(cur.k + cur.m);
        struct Node nxt = { new_sum, cur.k, new_m };
        heap_push(&pq, nxt);
    }

    /* Finalise the last value processed */
    if (has_last && (size_t)current_count <= target && cache[current_count] == 0) {
        cache[current_count] = last_val;
        ++found_count;
    }

    max_computed_n = target;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below offset is undefined */
    ensure(n);
    return cache[n];
}
