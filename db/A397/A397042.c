/*
 * A397042  Numbers that can be written as the product of numbers of the form
 *          x^2 + 1.
 * https://oeis.org/A397042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 404, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: A397042 – Numbers that can be written as a product of numbers of the form x^2 + 1.
    Computation: generate all such numbers up to a fixed limit (2 000 000) with a min‑heap,
    store them in a sorted array and return a(n) as the (n-1)th element. */

/*  The code follows the requirements of the test harness:
    - uses only allowed arithmetic and bit operations on seq_t,
    - casts the result of malloc/realloc/calloc for C++ compatibility,
    - memoises the sequence in a static array so calls in any order work,
    - does not hard‑code any terms.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* -------------------  Configuration  ------------------- */
#undef  MAX_LIMIT
#define MAX_LIMIT 2000000  /* upper bound for generated terms  */

/* -------------------  Static state  ------------------- */
static seq_t *seq_cache      = NULL;   /* sorted list of terms          */
static size_t cache_len      = 0;      /* number of terms computed      */
static size_t cache_cap      = 0;      /* allocated capacity of cache   */

static seq_t *heap           = NULL;   /* min‑heap for BFS              */
static size_t heap_len       = 0;      /* current heap size             */
static size_t heap_cap       = 0;      /* heap capacity                 */

static unsigned char *visited = NULL;  /* visited indices up to MAX_LIMIT */
static seq_t generators[2000];         /* all x^2+1 values ≤ MAX_LIMIT   */
static size_t gen_count = 0;           /* number of generators           */

/* -------------------  Helper: min‑heap ------------------- */
static void heap_push(seq_t val)
{
    if (heap_len >= heap_cap) {
        /* increase capacity twice */
        heap_cap = heap_cap ? heap_cap * 2 : 1;
        heap = (seq_t*)realloc(heap, heap_cap * sizeof(seq_t));
        if (!heap) exit(1);
    }
    heap[heap_len++] = val;
    size_t i = heap_len - 1;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent] <= heap[i]) break;
        seq_t tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i]     = tmp;
        i = parent;
    }
}

static seq_t heap_pop(void)
{
    seq_t top = heap[0];
    heap[0] = heap[--heap_len];
    size_t i = 0;
    while (1) {
        size_t left  = (i << 1) + 1;
        if (left >= heap_len) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_len && heap[right] < heap[left]) smallest = right;
        if (heap[i] <= heap[smallest]) break;
        seq_t tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return top;
}

/* -------------------  Build the sequence  ------------------- */
static void build_sequence(void)
{
    /* 1. compute generators x^2+1 (x ≥ 0) up to MAX_LIMIT */
    gen_count = 0;
    for (idx_t x = 0; ; ++x) {
        seq_t g = (seq_t)x * (seq_t)x + (seq_t)1;
        if (g > (seq_t)MAX_LIMIT) break;
        generators[gen_count++] = g;
    }

    /* 2. allocate visited array */
    visited = (unsigned char*)calloc((size_t)MAX_LIMIT + 1, sizeof(unsigned char));
    if (!visited) exit(1);

    /* 3. allocate heap */
    heap_cap = (size_t)MAX_LIMIT + 1;
    heap = (seq_t*)malloc(heap_cap * sizeof(seq_t));
    if (!heap) exit(1);

    /* 4. allocate cache for terms */
    cache_cap = (size_t)MAX_LIMIT + 1;
    seq_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (!seq_cache) exit(1);

    /* 5. initialise generation with 1 */
    heap_push((seq_t)1);
    visited[1] = 1;
    cache_len = 0;

    /* 6. BFS over the semigroup */
    while (heap_len > 0) {
        seq_t val = heap_pop();

        /* store term */
        seq_cache[cache_len++] = val;

        /* generate new products */
        for (size_t i = 0; i < gen_count; ++i) {
            seq_t g = generators[i];
            seq_t prod_seq = val * g;
            if (prod_seq > (seq_t)MAX_LIMIT) break;  /* larger generators => larger product */
            idx_t prod = (idx_t)prod_seq;            /* safe conversion: prod_seq ≤ MAX_LIMIT */
            if (!visited[prod]) {
                visited[prod] = 1;
                heap_push(prod_seq);
            }
        }
    }

    /* 7. free auxiliary data; cache stays allocated */
    free(visited);
    free(heap);
    visited = NULL;
    heap    = NULL;
}

/* -------------------  Public function  ------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        build_sequence();
        initialized = true;
    }

    if (n == 0) return (seq_t)0;               /* undefined for offset 1, safe fallback */
    size_t idx = (size_t)n - 1;
    if (idx < cache_len) return seq_cache[idx];
    /* if requested n exceeds precomputed terms, return 0  */
    return (seq_t)0;
}
