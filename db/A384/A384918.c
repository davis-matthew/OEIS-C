/*
 * A384918  If k is in the sequence, so is k*2^m + 3, for all m > 0, a(1) = 2,
 *          ordered.
 * https://oeis.org/A384918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A384918:
   a(1)=2; for each k in the sequence, all numbers k*2^m+3 (m>0) are in the sequence;
   the sequence is ordered increasingly. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

struct Node {
    seq_t val;
    seq_t base;
    idx_t m;
};

static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

static struct Node *heap       = NULL;
static size_t heap_cap          = 0;
static size_t heap_len          = 0;

static void swap_nodes(size_t i, size_t j)
{
    struct Node tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

static void heap_push(struct Node n)
{
    if (heap_len == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 16;
        struct Node *tmp = (struct Node *)realloc(heap, new_cap * sizeof(struct Node));
        if (!tmp) abort();
        heap = tmp;
        heap_cap = new_cap;
    }
    size_t i = heap_len++;
    heap[i] = n;
    /* bubble up */
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        swap_nodes(i, parent);
        i = parent;
    }
}

static struct Node heap_pop(void)
{
    if (heap_len == 0) abort();
    struct Node top = heap[0];
    heap[0] = heap[--heap_len];
    /* bubble down */
    size_t i = 0;
    while (1) {
        size_t left   = (i << 1) + 1;
        size_t right  = left + 1;
        size_t smallest = i;
        if (left < heap_len && heap[left].val < heap[smallest].val) smallest = left;
        if (right < heap_len && heap[right].val < heap[smallest].val) smallest = right;
        if (smallest == i) break;
        swap_nodes(i, smallest);
        i = smallest;
    }
    return top;
}

static void init_sequence(void)
{
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();
    }
    cache[0] = (seq_t)2;      /* a(1) = 2 */
    cache_len = 1;

    /* Initial candidate from base 2: 2*2 + 3 = 7 */
    struct Node n;
    n.val = (seq_t)2 * (seq_t)2 + (seq_t)3;
    n.base = (seq_t)2;
    n.m    = 1;
    heap_push(n);
}

static void extend_sequence(size_t upto)
{
    /* heap will be allocated on first push */
    while (cache_len < upto) {
        struct Node cur = heap_pop();
        seq_t val = cur.val;

        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = val;

        /* Next same base: k*2^(m+1)+3 = 2*val - 3 */
        seq_t next_same = (seq_t)2 * val - (seq_t)3;
        struct Node ns;
        ns.val = next_same;
        ns.base = cur.base;
        ns.m = cur.m + 1;
        heap_push(ns);

        /* New base: val*2 + 3 */
        seq_t next_new = (seq_t)2 * val + (seq_t)3;
        struct Node nnew;
        nnew.val = next_new;
        nnew.base = val;
        nnew.m = 1;
        heap_push(nnew);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (!cache) init_sequence();
    if ((size_t)n > cache_len) extend_sequence((size_t)n);

    return cache[(size_t)n - 1];
}
