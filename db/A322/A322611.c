/*
 * A322611  Numbers that are sums (of a nonempty sequence) of consecutive
 *          centered square numbers.
 * https://oeis.org/A322611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   centered_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A322611: numbers that are sums of one or more consecutive centered
 * square numbers.  The sequence is produced by repeatedly extracting the
 * smallest currently‑possible sum from a min‑heap and extending the
 * corresponding continuation.  Each new starting index is added to the heap
 * when its initial centered square is less than or equal to the current
 * minimum.  This yields the sums in ascending order with duplicates omitted. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>  /* for size_t */
#include "../utils/centered_value.h"

/* --- Elementary helpers --------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Heap for active sequences ------------------------------------------------- */

typedef struct {
    seq_t sum;   /* current sum for this sequence            */
    idx_t m;     /* starting index (1‑based)                 */
    idx_t len;   /* current length of the sequence           */
} HeapNode;

static HeapNode *heap      = NULL;
static size_t    heap_len  = 0,
                 heap_cap  = 0;

static void heap_ensure(void)
{
    if (heap_len >= heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 16;
        heap = (HeapNode *)realloc(heap, new_cap * sizeof(HeapNode));
        heap_cap = new_cap;
    }
}

static inline void heap_swap(size_t i, size_t j)
{
    HeapNode tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

static void heap_push(HeapNode node)
{
    heap_ensure();
    heap[heap_len++] = node;
    size_t i = heap_len - 1;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].sum <= heap[i].sum)
            break;
        heap_swap(i, parent);
        i = parent;
    }
}

static HeapNode heap_pop(void)
{
    HeapNode min = heap[0];
    heap[0] = heap[--heap_len];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_len && heap[l].sum < heap[smallest].sum)
            smallest = l;
        if (r < heap_len && heap[r].sum < heap[smallest].sum)
            smallest = r;
        if (smallest == i)
            break;
        heap_swap(i, smallest);
        i = smallest;
    }
    return min;
}

static inline void push_start(idx_t m)
{
    HeapNode node;
    node.m   = m;
    node.len = 1;
    node.sum = centered_value(m);
    heap_push(node);
}

/* -------------------------------------------------------------------------- */
/* Cache for generated terms ------------------------------------------------ */

static seq_t *cache   = NULL;
static size_t cache_len = 0,
              cache_cap = 0;

static void cache_ensure(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* -------------------------------------------------------------------------- */
/* Main sequence function --------------------------------------------------- */

static bool initialized = false;
static idx_t next_start = 1;   /* next starting index to consider adding */

seq_t a(idx_t n)
{
    if (n < 1)   /* offset 1 is defined; below is undefined */
        return 0;

    if (!initialized) {
        initialized = true;
        heap_len = heap_cap = 0;
        heap = NULL;
        cache_len = cache_cap = 0;
        cache = NULL;
        push_start(1);          /* start with m = 1 */
        next_start = 2;         /* next new start */
    }

    /* Generate terms until we reach the requested index. */
    while (cache_len < (size_t)n) {
        /* Smallest current sum for any sequence */
        HeapNode cur = heap_pop();

        /* Store the sum if it is new. */
        if (cache_len == 0 || cur.sum != cache[cache_len - 1]) {
            cache_ensure();
            cache[cache_len++] = cur.sum;
        }

        /* Extend the same sequence and push the new sum. */
        ++cur.len;
        /* Add the next centered square number: index m+len-1 */
        cur.sum += centered_value(cur.m + cur.len - 1);
        heap_push(cur);

        /* Add new starting positions whose initial sum would be <= current min. */
        while (1) {
            seq_t start_sum = centered_value(next_start);
            if (heap_len == 0 || start_sum <= heap[0].sum) {
                push_start(next_start);
                ++next_start;
            } else
                break;
        }
    }

    return cache[n - 1];
}
