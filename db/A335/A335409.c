/*
 * A335409  a(n) is the least k such that A335155(n) = A335393(k).
 * https://oeis.org/A335409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 160, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements the OEIS sequence A335409.
   a(n) is the least k such that A335155(n) = A335393(k),
   i.e. the minimal k whose binary representation encodes a
   sequence of operations (+5 for a 0-bit, *3 for a 1-bit) that produces
   the n‑th smallest distinct number reachable from the starting
   value 1.  The values are generated in increasing order by a
   min‑heap on (value, k) with tie‑break on k, and each distinct
   value records the first (hence minimal) k that produces it. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    seq_t  val;   /* value produced from this k */
    idx_t  k;     /* the binary index */
} node_t;

/* ----------------- Min‑heap of node_t ----------------- */
static node_t *heap = NULL;
static size_t heap_size = 0;
static size_t heap_cap  = 0;

static inline int node_cmp(const node_t *a, const node_t *b)
{
    if (a->val < b->val) return -1;
    if (a->val > b->val) return  1;
    if (a->k   < b->k  ) return -1;
    if (a->k   > b->k  ) return  1;
    return 0;
}

static void heap_init(void)
{
    if (!heap) {
        heap_cap = 16;
        heap = (node_t *)malloc(heap_cap * sizeof(node_t));
        heap_size = 0;
    }
}

static void heap_expand(void)
{
    size_t new_cap = heap_cap ? heap_cap * 2 : 16;
    node_t *tmp = (node_t *)realloc(heap, new_cap * sizeof(node_t));
    if (!tmp) abort();
    heap = tmp;
    heap_cap = new_cap;
}

static void heap_push(node_t node)
{
    if (heap_size >= heap_cap) heap_expand();
    size_t i = heap_size++;
    heap[i] = node;
    /* bubble up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (node_cmp(&heap[i], &heap[p]) >= 0) break;
        node_t tmp = heap[i]; heap[i] = heap[p]; heap[p] = tmp;
        i = p;
    }
}

static node_t heap_pop(void)
{
    node_t top = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (true) {
        size_t l = (i << 1) + 1;
        if (l >= heap_size) break;
        size_t r = l + 1;
        size_t smallest = l;
        if (r < heap_size && node_cmp(&heap[r], &heap[l]) < 0)
            smallest = r;
        if (node_cmp(&heap[smallest], &heap[i]) >= 0) break;
        node_t tmp = heap[i]; heap[i] = heap[smallest]; heap[smallest] = tmp;
        i = smallest;
    }
    return top;
}

/* ----------------- Arrays of results ----------------- */
static seq_t  *values = NULL;   /* distinct values in sorted order */
static idx_t  *ks     = NULL;   /* minimal k for each value */
static size_t  sz          = 0; /* number of distinct values stored */
static size_t  cap         = 0; /* allocated capacity for arrays */

static void results_init(void)
{
    if (!values) {
        cap = 16;
        values = (seq_t *)malloc(cap * sizeof(seq_t));
        ks     = (idx_t *)malloc(cap * sizeof(idx_t));
        sz     = 0;
    }
}

static void results_expand(void)
{
    size_t new_cap = cap ? cap * 2 : 16;
    seq_t *val_tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
    idx_t *k_tmp   = (idx_t *)realloc(ks,     new_cap * sizeof(idx_t));
    if (!val_tmp || !k_tmp) abort();
    values = val_tmp; ks = k_tmp; cap = new_cap;
}

/* ----------------- Main function ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for 0, return 0 */

    results_init();
    heap_init();

    /* Main loop: generate values until we have n distinct ones */
    while (sz < (size_t)n) {
        /* Ensure the heap is not empty – push initial node if needed */
        if (heap_size == 0 && sz == 0) {
            heap_push((node_t){ (seq_t)1, (idx_t)1 });
        }

        node_t cur = heap_pop();

        /* Skip duplicates – only the first (smallest k) is recorded */
        if (sz > 0 && values[sz - 1] == cur.val) {
            /* do not store, but still continue pushing children only
               for the first occurrence – hence children are already
               scheduled in earlier steps. */
            continue;
        }

        /* Record the new distinct value and its minimal k */
        if (sz >= cap) results_expand();
        values[sz] = cur.val;
        ks[sz]     = cur.k;
        sz++;

        /* Generate children of this node */
        heap_push((node_t){ cur.val + (seq_t)5, cur.k * 2     });
        heap_push((node_t){ cur.val * (seq_t)3, cur.k * 2 + 1 });
    }

    /* At this point ks[n-1] holds the minimal k for the n‑th smallest value */
    return ks[n - 1];
}
