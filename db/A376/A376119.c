/*
 * A376119  Abundant numbers that are perfect powers.
 * https://oeis.org/A376119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   seq_mul, seq_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utilities provided by the build environment */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t a, seq_t b) */
#include "../utils/seq_pow.h"    /* seq_t seq_pow(seq_t base, int exp) */
#include "../utils/sigma_of.h"  /* seq_t sigma_of(idx_t n) */

/* ----------------------------------------------------------------------
   Abundant perfect powers
   ---------------------------------------------------------------------- */

/* Comment: computes the n-th term of OEIS sequence A376119
   (abundant numbers that are perfect powers). */

typedef struct {
    seq_t val;     /* current value of base^exp */
    seq_t base;    /* base of the power */
    unsigned exp;  /* current exponent (>=2) */
} HeapNode;

typedef struct {
    HeapNode *data; /* dynamic array of nodes */
    size_t   size;  /* number of elements in heap */
    size_t   cap;   /* allocated capacity */
} MinHeap;

/* ---------- heap helper functions ------------------------------------ */

static inline bool node_less(const HeapNode* a, const HeapNode* b)
{
    return a->val < b->val;
}

static inline void heap_swap(HeapNode* a, HeapNode* b)
{
    HeapNode t = *a; *a = *b; *b = t;
}

static void heap_push(MinHeap* h, HeapNode n)
{
    if (h->size == h->cap) {
        size_t newcap = h->cap ? h->cap * 2 : 16;
        h->data = (HeapNode*)realloc(h->data, newcap * sizeof(HeapNode));
        h->cap = newcap;
    }
    h->data[h->size++] = n;
    size_t i = h->size - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (node_less(&h->data[i], &h->data[p])) {
            heap_swap(&h->data[i], &h->data[p]);
            i = p;
        } else break;
    }
}

static HeapNode heap_pop(MinHeap* h)
{
    HeapNode top = h->data[0];
    --h->size;
    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        size_t i = 0;
        while (1) {
            size_t l = (i << 1) + 1;
            size_t r = l + 1;
            size_t best = i;
            if (l < h->size && node_less(&h->data[l], &h->data[best]))
                best = l;
            if (r < h->size && node_less(&h->data[r], &h->data[best]))
                best = r;
            if (best != i) {
                heap_swap(&h->data[i], &h->data[best]);
                i = best;
            } else break;
        }
    }
    return top;
}

/* ---------- abundant test ------------------------------------------- */

static inline bool is_abundant(idx_t n)
{
    seq_t sigma = sigma_of(n);          /* sum of all positive divisors */
    seq_t two_n = (seq_t)(2 * (uint64_t)n);
    return sigma > two_n;
}

/* ---------- main sequence function ----------------------------------- */

seq_t a(idx_t n)
{
    /* static data persistent between calls */
    static bool      initialized = false;
    static MinHeap   heap = {NULL,0,0};
    static seq_t*    results = NULL;
    static size_t    results_len = 0;
    static size_t    results_cap = 0;
    static seq_t     last_val = 0;
    static idx_t     max_requested = 0;

    /* initialization on first call */
    if (!initialized) {
        const idx_t BASE_LIMIT = 200000;     /* seems sufficient for n<=10000 */
        for (idx_t b = 2; b <= BASE_LIMIT; ++b) {
            seq_t val = seq_mul((seq_t)b, (seq_t)b);   /* b^2 */
            heap_push(&heap, (HeapNode){val, (seq_t)b, 2});
        }

        results_cap = 64;
        results     = (seq_t*)malloc(results_cap * sizeof(seq_t));
        results_len = 0;
        max_requested = 0;

        initialized = true;
    }

    /* expand the list until we have enough terms for the requested index */
    while (results_len < n) {
        HeapNode node = heap_pop(&heap);

        /* skip duplicates that appear with different bases/exponents */
        if (node.val != last_val) {
            /* test for abundance */
            if (is_abundant((idx_t)node.val)) {
                if (results_len == results_cap) {
                    results_cap <<= 1;
                    results = (seq_t*)realloc(results, results_cap * sizeof(seq_t));
                }
                results[results_len++] = node.val;
            }
            last_val = node.val;
        }

        /* push the next power of the same base */
        seq_t next_val = seq_mul(node.val, node.base);
        heap_push(&heap, (HeapNode){next_val, node.base, node.exp + 1});
    }

    max_requested = n;
    return results[n-1];
}
