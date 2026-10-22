/*
 * A389957  Even perfect powers plus 1.
 * https://oeis.org/A389957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A075090(n) + 1 where A075090(n) is the n‑th even perfect
 *            power (m^k, m even, k ≥ 2)
 * Range:     u8 none, u16 none, u32 n <= 987, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Node in the heap: current power (val) and the base that generates
   the next power. */
typedef struct {
    seq_t val;   /* current power m^k */
    seq_t base;  /* base m : next power will be val * base */
} heap_node_t;

/* Upper limit for even bases considered.  This value is ample to
   generate at least the first 10000 even perfect powers. */
#define MAX_BASE 50000

/* Number of even bases up to MAX_BASE. */
#define HEAP_CAP (MAX_BASE / 2)

/* Static heap and cache storage. */
static heap_node_t *heap = NULL;
static size_t heap_size = 0;
static bool heap_inited = false;

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap = 0;    /* allocated capacity */
static seq_t prev_val = 0;      /* last even perfect power emitted */

/* Helper functions for the binary min‑heap. */
static inline void swap_node(heap_node_t *a, heap_node_t *b)
{
    heap_node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heapify_down(size_t i)
{
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;

        if (l < heap_size && heap[l].val < heap[smallest].val)
            smallest = l;
        if (r < heap_size && heap[r].val < heap[smallest].val)
            smallest = r;

        if (smallest == i)
            break;
        swap_node(&heap[i], &heap[smallest]);
        i = smallest;
    }
}

static void heapify_up(size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val)
            break;
        swap_node(&heap[parent], &heap[i]);
        i = parent;
    }
}

/* Initialise the heap with one node per even base, starting at its square. */
static void init_heap(void)
{
    if (heap_inited)
        return;

    heap = (heap_node_t *)malloc(HEAP_CAP * sizeof(heap_node_t));
    if (!heap)
        return;  /* allocation failure – further computation may be impossible */

    size_t pos = 0;
    for (idx_t b = 2; b <= MAX_BASE; b += 2) {
        seq_t base_seq = (seq_t)b;
        heap[pos].val  = base_seq * base_seq;  /* m^2 */
        heap[pos].base = base_seq;
        pos++;
    }
    heap_size = pos;

    /* Turn the array into a heap. */
    for (size_t i = (heap_size >> 1); i > 0; --i)
        heapify_down(i - 1);

    heap_inited = true;
}

/* Ensure the cache has room for at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp) {
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n = 0 */

    /* Generate terms until we have at least n of them. */
    if (n > cache_size) {
        init_heap();                       /* build heap once */
        ensure_cache_capacity(n);

        while (cache_size < n) {
            /* Extract the smallest even perfect power. */
            heap_node_t top = heap[0];
            seq_t val = top.val;

            /* Store it as val + 1, skipping duplicates. */
            if (val != prev_val) {
                cache[cache_size++] = val + (seq_t)1;
                prev_val = val;
            }

            /* Replace root with the next power of the same base. */
            heap[0].val = val * top.base;  /* m^(k+1) */
            heapify_down(0);
        }
    }

    return cache[n - 1];
}
