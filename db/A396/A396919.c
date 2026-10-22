/*
 * A396919  Numbers of the form 4*p or 2*p^2, where p is an odd prime.
 * https://oeis.org/A396919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   next_prime, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as described: the numbers 4*p or 2*p^2 where p is an odd prime.
   The function builds the sequence on demand, memoising terms in a static
   cache.  Prime generation is performed lazily with next_prime().
   Heap (binary min‑heap) is used to merge the two ascending subsequences
   without sorting the whole list each time. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/seq_cmp.h"     /* int seq_cmp(const void *, const void *); */

/* Binary min‑heap for seq_t values.  cmp_seq_t is defined in seq_cmp.h. */
typedef struct {
    seq_t *data;    /* array of heap elements */
    size_t size;    /* number of elements in the heap */
    size_t capacity;/* allocated capacity */
} seq_heap;

/* Ensure heap has enough space for one more element. */
static void heap_ensure_capacity(seq_heap *h)
{
    if (h->size == h->capacity) {
        size_t new_cap = h->capacity ? h->capacity * 2 : 8;
        seq_t *new_data = (seq_t *)realloc(h->data, new_cap * sizeof(seq_t));
        h->data = new_data;
        h->capacity = new_cap;
    }
}

/* Push a value onto the heap. */
static void heap_push(seq_heap *h, seq_t val)
{
    heap_ensure_capacity(h);
    size_t i = h->size++;
    h->data[i] = val;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (seq_cmp(&h->data[p], &h->data[i]) <= 0) break;
        /* swap */
        seq_t tmp = h->data[i];
        h->data[i] = h->data[p];
        h->data[p] = tmp;
        i = p;
    }
}

/* Pop the minimum element from the heap.  Assumes non‑empty. */
static seq_t heap_pop(seq_heap *h)
{
    seq_t top = h->data[0];
    h->size--;
    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        size_t i = 0;
        while (1) {
            size_t left = (i << 1) + 1;
            size_t right = left + 1;
            size_t smallest = i;
            if (left < h->size &&
                seq_cmp(&h->data[left], &h->data[smallest]) < 0)
                smallest = left;
            if (right < h->size &&
                seq_cmp(&h->data[right], &h->data[smallest]) < 0)
                smallest = right;
            if (smallest == i) break;
            seq_t tmp = h->data[i];
            h->data[i] = h->data[smallest];
            h->data[smallest] = tmp;
            i = smallest;
        }
    }
    return top;
}

/* Main sequence function.  Memoises results in a static array. */
seq_t a(idx_t n)
{
    /* Static cache for computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* State for prime generation and heap. */
    static uint64_t last_prime = 0; /* 0 indicates no prime yet used */
    static seq_heap heap = {NULL, 0, 0};

    /* Grow cache when needed. */
    if (n > cache_size) {
        /* Allocate heap if first run. */
        if (heap.data == NULL)
            heap.data = (seq_t *)calloc(8, sizeof(seq_t));

        /* Generate primes and push their 4*p and 2*p^2 terms into the heap
           until we have at least n values in the cache. */
        while (cache_size < (size_t)n) {
            uint64_t p;
            if (last_prime == 0)
                p = 3;            /* first odd prime */
            else
                p = next_prime(last_prime + 1);
            last_prime = p;

            /* 4*p */
            seq_t term1 = (seq_t)p;
            term1 *= 4;

            /* 2*p^2 */
            seq_t pp = (seq_t)p;
            pp *= p;            /* p^2 */
            seq_t term2 = pp * 2;

            heap_push(&heap, term1);
            heap_push(&heap, term2);

            /* Pop the smallest elements into the cache as long as we need more terms. */
            while (cache_size < (size_t)n && heap.size > 0) {
                seq_t val = heap_pop(&heap);
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache = new_mem;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = val;
            }
        }
    }
    return cache[(size_t)n - 1];
}
