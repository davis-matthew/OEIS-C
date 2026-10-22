/*
 * A373689  Perfect powers such that the three numbers before it and the three
 *          numbers after it are squarefree.
 * https://oeis.org/A373689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: perfect powers such that the three numbers before it and the
   three numbers after it are squarefree.  The implementation
   generates all perfect powers in increasing order and tests the six
   neighbours for squarefreeness.  Results are cached so that calls in
   any order are answered in O(1) after the initial generation. */

/* Include the square‑free test provided by the framework */
#include "../utils/is_squarefree.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------------------------------- heap of power nodes ---------------------------------- */
typedef struct {
    seq_t val;        /* current power value */
    uint64_t base;    /* base of the power */
} PowerNode;

typedef struct {
    PowerNode *data;  /* heap array */
    size_t sz;        /* number of elements */
    size_t cap;       /* capacity */
} PowerHeap;

static void heap_init(PowerHeap *h, size_t cap)
{
    h->data = (PowerNode *)malloc(cap * sizeof(PowerNode));
    h->sz   = 0;
    h->cap  = cap;
}

static void heap_free(PowerHeap *h)
{
    free(h->data);
    h->data = NULL;
    h->sz   = h->cap = 0;
}

static void heap_push(PowerHeap *h, PowerNode n)
{
    if (h->sz == h->cap) {
        h->cap = h->cap ? h->cap * 2 : 16;
        h->data = (PowerNode *)realloc(h->data, h->cap * sizeof(PowerNode));
    }
    size_t i = h->sz++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h->data[p].val <= n.val) break;
        h->data[i] = h->data[p];
        i = p;
    }
    h->data[i] = n;
}

static PowerNode heap_pop(PowerHeap *h)
{
    PowerNode top = h->data[0];
    PowerNode last = h->data[--h->sz];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        if (l >= h->sz) break;
        size_t s = l;
        if (r < h->sz && h->data[r].val < h->data[l].val) s = r;
        if (h->data[s].val >= last.val) break;
        h->data[i] = h->data[s];
        i = s;
    }
    h->data[i] = last;
    return top;
}

/* ---------------------------------- static state for sequence generation ---------------------------------- */
#define BASE_LIMIT 100000UL               /* highest base considered for perfect powers */

static PowerHeap power_heap;
static bool heap_inited = false;

static seq_t *seq_cache = NULL;
static idx_t cache_len = 0, cache_cap = 0;
static bool cache_inited = false;
static seq_t last_val = 0;
static bool last_set   = false;

/* initialise the min‑heap with base^2 for every base up to BASE_LIMIT */
static void init_heap(void)
{
    if (heap_inited) return;
    heap_inited = true;
    heap_init(&power_heap, BASE_LIMIT);
    for (uint64_t b = 2; b <= BASE_LIMIT; ++b) {
        PowerNode n;
        n.base = b;
        n.val  = (seq_t)b * (seq_t)b;   /* start with exponent 2 */
        heap_push(&power_heap, n);
    }
}

static void ensure_cache(idx_t need)
{
    if (!cache_inited) {
        cache_inited = true;
        cache_cap   = 64;
        seq_cache   = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len   = 0;
    }
    while (cache_cap < need) {
        cache_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, cache_cap * sizeof(seq_t));
    }
}

/* generate terms up to index n (1‑based) */
static void generate_upto(idx_t n)
{
    init_heap();
    ensure_cache(n + 1);          /* one extra slot for 1‑based indexing */

    const int ofs[6] = {-3, -2, -1, 1, 2, 3};

    while (cache_len < n) {
        PowerNode node = heap_pop(&power_heap);
        seq_t v = node.val;

        /* skip duplicates produced by different (base,exp) pairs */
        if (last_set && v == last_val) {
            /* nothing to do */
        } else {
            bool ok = true;
            for (int k = 0; k < 6 && ok; ++k) {
                seq_t nb = v + ofs[k];
                if (nb < (seq_t)1) { ok = false; break; }
                if (!is_squarefree(nb)) ok = false;
            }
            if (ok) {
                seq_cache[cache_len++] = v;
            }
        }
        last_val = v;
        last_set = true;

        /* push next power for the same base */
        node.val = v * (seq_t)node.base;   /* multiply by base again */
        heap_push(&power_heap, node);
    }
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */
    generate_upto(n);
    return seq_cache[n - 1];             /* convert 1‑based to 0‑based index */
}
