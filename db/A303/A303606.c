/*
 * A303606  Powers of composite squarefree numbers that are not squarefree.
 * https://oeis.org/A303606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 2796, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isCompositeSquarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A303606 – Powers of composite square‑free numbers
   that are not square‑free.  The function returns the n‑th element
   of the sequence for n ≥ 1.  The implementation merges the
   powers of all composite square‑free bases using a
   min‑heap, so terms are generated in sorted order and cached
   for arbitrary‑order access. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isCompositeSquarefree.h"

/* Heap node */
typedef struct {
    seq_t val;          /* current power of the base */
    uint64_t base;      /* the base m */
    uint8_t  exp;       /* current exponent k (k ≥ 2) */
} HeapNode;

/* Static heap data */
static HeapNode *heap = NULL;
static size_t heap_size = 0, heap_cap = 0;

/* Static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0, cache_size = 0;

/* Next base to consider (starts after the first composite square‑free) */
static uint64_t next_base = 6;

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Push a new node onto the min‑heap */
static void heap_push(seq_t val, uint64_t base, uint8_t exp)
{
    if (heap_size == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 16;
        HeapNode *new_heap = (HeapNode *)realloc(heap, new_cap * sizeof(HeapNode));
        heap = new_heap;
        heap_cap = new_cap;
    }
    size_t i = heap_size++;
    heap[i].val  = val;
    heap[i].base = base;
    heap[i].exp  = exp;

    /* sift up */
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val)
            break;
        HeapNode tmp = heap[i];
        heap[i] = heap[parent];
        heap[parent] = tmp;
        i = parent;
    }
}

/* ------------------------------------------------------------------ */
/* Pop the smallest node from the heap */
static HeapNode heap_pop_min(void)
{
    HeapNode ret = heap[0];
    heap_size--;
    if (heap_size == 0)
        return ret;

    heap[0] = heap[heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        if (l >= heap_size)
            break;
        size_t smallest = l;
        if (r < heap_size && heap[r].val < heap[l].val)
            smallest = r;
        if (heap[i].val <= heap[smallest].val)
            break;
        HeapNode tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return ret;
}

/* ------------------------------------------------------------------ */
/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)                     /* ignore non‑positive indices */
        return 0;

    /* Generate terms until we have at least n values cached */
    while (cache_size < (size_t)n) {

        /* Ensure the heap contains at least one element */
        if (heap_size == 0) {
            while (!isCompositeSquarefree(next_base))
                next_base++;
            seq_t sq = (seq_t)next_base * (seq_t)next_base;
            heap_push(sq, next_base, 2);
            next_base++;
        }

        /* Peek at the minimal candidate */
        HeapNode top = heap[0];

        /* Insert new bases whose squares are ≤ current minimal value */
        while (1) {
            uint64_t cand = next_base;
            while (!isCompositeSquarefree(cand))
                cand++;
            seq_t sq = (seq_t)cand * (seq_t)cand;
            if (sq > top.val)      /* stop when the square exceeds the current minimum */
                break;
            heap_push(sq, cand, 2);
            next_base = cand + 1;
        }

        /* Extract the next term to append */
        HeapNode cur = heap_pop_min();

        /* Store in cache */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_size++] = cur.val;

        /* Push the next power of the same base (if it does not overflow) */
        seq_t nxt = cur.val * (seq_t)cur.base;
        if (nxt >= cur.val)            /* guard against overflow; in unsigned arithmetic
                                           overflow makes nxt < cur.val */
            heap_push(nxt, cur.base, cur.exp + 1);
    }

    return cache[n - 1];
}
