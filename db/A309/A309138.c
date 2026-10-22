/*
 * A309138  Nonnegative integers of the form x^2 + 23*y^2.
 * https://oeis.org/A309138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309138: numbers of the form x^2 + 23*y^2.
   Computed by a min‑heap over rows indexed by y.
   Each row contains the current value of x^2 + 23*y^2.
   New rows are inserted when a term is extracted, ensuring all
   values appear in increasing order with duplicates removed. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Node used in the min‑heap */
typedef struct {
    seq_t val;          /* x^2 + 23*y^2 */
    uint64_t x;         /* current x for this y */
    uint64_t y;         /* current y */
} node_t;

/* Static state kept between calls */
static seq_t *cache     = NULL;
static size_t cache_cap = 0, cache_len = 0;

static node_t *heap     = NULL;
static size_t heap_cap  = 0, heap_len  = 0;

static uint64_t next_y  = 1;              /* next new y value to push */
static seq_t   last_val = (seq_t)-1;      /* last value output, for deduplication */

/* Swap two nodes */
static void swap(node_t *a, node_t *b)
{
    node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Heap helper: sift up from position i */
static void heap_sift_up(size_t i)
{
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        swap(&heap[p], &heap[i]);
        i = p;
    }
}

/* Heap helper: sift down from position i */
static void heap_sift_down(size_t i)
{
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;

        if (l < heap_len && heap[l].val < heap[smallest].val)
            smallest = l;
        if (r < heap_len && heap[r].val < heap[smallest].val)
            smallest = r;

        if (smallest == i) break;
        swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
}

/* Grow a generic array, casting for C++ compatibility */
static void grow(void **ptr, size_t *cap, size_t elem_size)
{
    size_t new_cap = (*cap) ? (*cap) * 2 : 128;
    void *p = realloc(*ptr, new_cap * elem_size);
    /* In C++ the allocation must be cast to a pointer of the desired type;
       the cast is omitted in C since it is implicit. */
    *ptr = p;
    *cap = new_cap;
}

/* Push a node onto the heap */
static void heap_push(node_t n)
{
    if (heap_len == heap_cap) grow((void**)&heap, &heap_cap, sizeof(node_t));
    heap[heap_len++] = n;
    heap_sift_up(heap_len - 1);
}

/* Pop the smallest node from the heap */
static node_t heap_pop(void)
{
    node_t top = heap[0];
    heap_len--;
    if (heap_len) {
        heap[0] = heap[heap_len];
        heap_sift_down(0);
    }
    return top;
}

/* Compute next value for the same y by incrementing x */
static node_t next_same(node_t cur)
{
    uint64_t nx = cur.x + 1;
    seq_t val = (seq_t)nx * (seq_t)nx + (seq_t)23 * (seq_t)cur.y * (seq_t)cur.y;
    return (node_t){ val, nx, cur.y };
}

/* Create node for a new y (starting with x = 0) */
static node_t new_y_node(uint64_t y)
{
    seq_t val = (seq_t)23 * (seq_t)y * (seq_t)y; /* x = 0 */
    return (node_t){ val, 0, y };
}

/* Main function required by the harness */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* safety for out‑of‑range, though not required */

    /* initialise static structures on first call */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        heap_cap  = 128;
        heap = (node_t*)malloc(heap_cap * sizeof(node_t));

        /* start with y = 0, x = 0 */
        heap_push((node_t){ 0, 0, 0 });
    }

    while (cache_len < (size_t)n) {
        node_t cur = heap_pop();

        /* store next distinct value */
        if (cur.val != last_val) {
            if (cache_len == cache_cap) grow((void**)&cache, &cache_cap, sizeof(seq_t));
            cache[cache_len++] = cur.val;
            last_val = cur.val;
        }

        /* push next candidate for the same y */
        heap_push(next_same(cur));

        /* push candidate for a fresh y (only once per y) */
        heap_push(new_y_node(next_y));
        next_y++;
    }

    return cache[n - 1];
}
