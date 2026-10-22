/*
 * A393100  A sequence constructed by greedily sampling the probability
 *          distribution given by 1/log_2(i+1) - 1/log_2(i+2) to minimize
 *          ratio discrepancy.
 * https://oeis.org/A393100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 948, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   p_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A393100: greedy generation of values according to
   p(k) = 1/log_2(k+1) - 1/log_2(k+2).  The code implements the
   algorithm described in the OEIS entry: at each step choose the
   integer k that minimises (c(k)+1)/p(k), where c(k) is the
   number of occurrences of k so far.  */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <float.h>
#include "../utils/p_of.h"

struct Node {
    idx_t k;
    idx_t count_prior;   /* count before the next occurrence */
    long double ratio;   /* (count_prior+1)/p(k) */
};

/* heap helpers ------------------------------------------------------ */
static struct Node *heap = NULL;
static size_t heap_cap = 0, heap_size = 0;

static void heap_expand(void) {
    size_t new_cap = heap_cap ? heap_cap << 1 : 16;
    heap = (struct Node *)realloc(heap, new_cap * sizeof(struct Node));
    heap_cap = new_cap;
}

static inline void heap_swap(size_t a, size_t b) {
    struct Node tmp = heap[a];
    heap[a] = heap[b];
    heap[b] = tmp;
}

static void heap_push(idx_t k, idx_t count_prior) {
    if (heap_size == heap_cap) heap_expand();
    heap[heap_size].k = k;
    heap[heap_size].count_prior = count_prior;
    heap[heap_size].ratio = ((long double)(count_prior + 1)) / p_of(k);
    size_t pos = heap_size++;
    /* sift up */
    while (pos) {
        size_t parent = (pos - 1) >> 1;
        if (heap[pos].ratio >= heap[parent].ratio) break;
        heap_swap(pos, parent);
        pos = parent;
    }
}

static void heap_pop(void) {
    if (heap_size == 0) return;
    heap[0] = heap[--heap_size];
    size_t pos = 0;
    while (1) {
        size_t left = (pos << 1) + 1;
        size_t right = left + 1;
        size_t smallest = pos;
        if (left < heap_size && heap[left].ratio < heap[smallest].ratio)
            smallest = left;
        if (right < heap_size && heap[right].ratio < heap[smallest].ratio)
            smallest = right;
        if (smallest == pos) break;
        heap_swap(pos, smallest);
        pos = smallest;
    }
}

static inline struct Node *heap_peek(void) {
    return heap_size ? &heap[0] : NULL;
}

/* memoisation arrays ----------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0, cache_len = 0;

static void cache_expand(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = needed > 16 ? needed << 1 : 16;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

static seq_t *counts = NULL;
static size_t counts_cap = 0;

static void counts_expand(size_t needed) {
    if (needed <= counts_cap) return;
    size_t new_cap = needed > 16 ? needed << 1 : 16;
    counts = (seq_t *)realloc(counts, new_cap * sizeof(seq_t));
    counts_cap = new_cap;
}

/* main sequence routine ------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 1 */
    /* ensure cache enough */
    if (n > cache_len) {
        /* extend auxiliary structures to handle all values up to n */
        cache_expand((size_t)n + 10);
        counts_expand((size_t)n + 10);
        static idx_t max_k = 0;
        while (cache_len < n) {
            /* new unseen candidate */
            idx_t next_k = max_k + 1;
            if (next_k >= counts_cap) counts_expand((size_t)next_k + 10);
            long double ratio_new_k = 1.0L / p_of(next_k);
            /* purge stale heap entries */
            struct Node *top = heap_peek();
            while (top && top->count_prior != (idx_t)counts[top->k]) {
                heap_pop();
                top = heap_peek();
            }
            long double best_existing = top ? top->ratio : LDBL_MAX;
            seq_t chosen;
            if (ratio_new_k < best_existing) {
                /* take the new k */
                counts[next_k] = 1;
                heap_push(next_k, 1);
                chosen = (seq_t)next_k;
                max_k = next_k;
            } else {
                /* take from heap */
                idx_t sel_k = top->k;
                heap_pop();
                /* ensure counts[sel_k] exists */
                if (sel_k >= counts_cap) counts_expand((size_t)sel_k + 10);
                counts[sel_k]++;
                heap_push(sel_k, (idx_t)counts[sel_k]);
                chosen = (seq_t)sel_k;
            }
            cache[cache_len++] = chosen;
        }
    }
    return cache[n - 1];
}
