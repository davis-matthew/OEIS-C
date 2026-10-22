/*
 * A322468  Numbers that are sums of consecutive tetrahedral numbers.
 * https://oeis.org/A322468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest integer that can be written as a sum
 *            of * consecutive tetrahedral numbers T(k)=k(k+1)(k+2)/6 with 1 ≤
 *            i ≤ j. * Offset: 1 – a(1) = 0. * The algorithm generates the
 *            sequence on demand using a min‑heap of streams, * each stream
 *            corresponding to a fixed starting index i and...
 * Range:     u8 n <= 10, u16 n <= 119, u32 n <= 603, u64 n <= 603,
 *            u128 n <= 603, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Tetrahedral number */
static inline seq_t tetrahedral(idx_t k)
{
    return (seq_t)k * ((seq_t)k + 1) * ((seq_t)k + 2) / 6;
}

/* A node of the min‑heap: one active stream */
struct node {
    seq_t sum; /* current sum of the stream */
    idx_t i;   /* fixed start index of the stream */
    idx_t j;   /* current end index of the stream (>= i) */
};

/* --------------------------------------------------------------------- */
/*  Static state – sequence cache and heap                               */
/* --------------------------------------------------------------------- */
static seq_t *results = NULL;    /* cached sequence values            */
static size_t  res_cap   = 0;     /* allocated capacity for results    */
static size_t  res_len   = 0;     /* number of values stored           */

static struct node *heap     = NULL;   /* min‑heap of active streams      */
static size_t       heap_cap = 0;       /* heap capacity                    */
static size_t       heap_len = 0;       /* number of elements in heap       */

static idx_t max_start = 0;   /* highest start index currently in heap */

/* --------------------------------------------------------------------- */
/*  Heap helpers                                                          */
/* --------------------------------------------------------------------- */
static void heap_push(struct node v)
{
    if (heap_len == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap << 1 : 8;
        struct node *tmp = (struct node *)realloc(heap, new_cap * sizeof(struct node));
        if (!tmp) abort();
        heap     = tmp;
        heap_cap = new_cap;
    }
    size_t idx = heap_len++;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].sum <= v.sum) break;
        heap[idx] = heap[parent];
        idx = parent;
    }
    heap[idx] = v;
}

static struct node heap_pop(void)
{
    struct node top = heap[0];
    heap_len--;
    struct node val = heap[heap_len];
    size_t idx = 0;
    while (1) {
        size_t left  = idx * 2 + 1;
        size_t right = left + 1;
        if (left >= heap_len) break;
        size_t child = left;
        if (right < heap_len && heap[right].sum < heap[left].sum)
            child = right;
        if (heap[child].sum >= val.sum) break;
        heap[idx] = heap[child];
        idx = child;
    }
    heap[idx] = val;
    return top;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety – the sequence is defined for n ≥ 1 */

    /* Initialise storage on first call */
    if (res_len == 0) {
        res_cap   = 16;
        results   = (seq_t *)malloc(res_cap * sizeof(seq_t));
        if (!results) abort();
        results[0] = (seq_t)0;   /* a(1) = 0 */
        res_len   = 1;
    }

    /* Add new start indices up to n, if not already present */
    while (max_start < n) {
        idx_t i = ++max_start;
        struct node nv;
        nv.i     = i;
        nv.j     = i;
        nv.sum   = tetrahedral(i);
        heap_push(nv);
    }

    /* Generate values until we have at least n terms */
    while (res_len < n) {
        struct node cur = heap_pop();

        /* Store only if not a duplicate of the previous value */
        if (res_len == 0 || cur.sum != results[res_len - 1]) {
            if (res_len == res_cap) {
                size_t new_cap = res_cap ? res_cap << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                results = tmp;
                res_cap = new_cap;
            }
            results[res_len++] = cur.sum;
        }

        /* Advance the stream and push the updated node back into the heap */
        cur.j++;
        cur.sum += tetrahedral(cur.j);
        heap_push(cur);
    }

    return results[n - 1];
}
