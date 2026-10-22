/*
 * A344822  Numbers m with decimal expansion (d_1, ..., d_k) such that d_i = m
 *          * i mod 10 for i = 1..k.
 * https://oeis.org/A344822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) for A344822
 *
 * The property is that for m with decimal digits (d1,…,dk) we have
 *   di = (m * i) mod 10   for i = 1..k
 *
 * This sequence consists of all numbers obtained by
 *   a ∈ {0,…,9}, k satisfying a*(k‑1) ≡ 0 (mod 10)
 *   digits di = (a*i) mod 10
 *   k is the length of the number.
 *
 * Each a generates an infinite increasing subsequence by advancing k
 *   by the period:
 *     period(a) = 0   for a==0
 *     10            for a∈{1,3,7,9}
 *     5             for a∈{2,4,6,8}
 *     2             for a==5
 *
 * The global sequence is obtained by merging these nine subsequences
 * with a min‑heap ordered first by length (k) and then by the numeric
 * value.  This ordering guarantees that all overflow examples (for
 * builds where seq_t is 64‑bit) are placed after all numbers that
 * fit within 64 bits, so indices of terms that do fit remain correct.
 *
 * The code caches previously computed terms for efficient repeated
 * calls.  The implementation is fully portable to C11 and C++17.
 */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Node for the priority queue                                          */
/* ------------------------------------------------------------------ */
typedef struct {
    seq_t   val;   /* numeric value of the term                      */
    unsigned int a;   /* leading (and last) digit 0..9                 */
    idx_t   k;     /* length of the number (number of digits)       */
} Node;

/* ------------------------------------------------------------------ */
/* Global state: heap and cache                                         */
/* ------------------------------------------------------------------ */
static Node  *heap          = NULL;
static size_t heap_size     = 0;
static size_t heap_cap      = 0;

static seq_t *cache         = NULL;
static size_t cache_len     = 0;
static size_t cache_cap     = 0;

/* ------------------------------------------------------------------ */
/* Periods for each leading digit                                      */
/* ------------------------------------------------------------------ */
static const unsigned int period[10] = {
    0, 10, 5, 10, 5, 2, 5, 10, 5, 10
};

/* ------------------------------------------------------------------ */
/* Utility: compare two nodes (min‑heap)                                */
/* ------------------------------------------------------------------ */
static inline int node_less(const Node *x, const Node *y)
{
    if (x->k != y->k)
        return x->k < y->k;
    return x->val < y->val;
}

/* ------------------------------------------------------------------ */
/* Heap operations                                                     */
/* ------------------------------------------------------------------ */
static void heap_resize(void)
{
    size_t new_cap = (heap_cap == 0) ? 8 : heap_cap * 2;
    Node *tmp = (Node *)realloc(heap, new_cap * sizeof(Node));
    if (!tmp) abort(); /* allocation failure */
    heap = tmp;
    heap_cap = new_cap;
}

static void heap_push(const Node node)
{
    if (heap_size >= heap_cap) heap_resize();
    size_t i = heap_size++;
    heap[i] = node;
    /* sift up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (!node_less(&heap[i], &heap[p])) break;
        Node tmp = heap[i];
        heap[i] = heap[p];
        heap[p] = tmp;
        i = p;
    }
}

static Node heap_pop(void)
{
    Node root = heap[0];
    heap[0] = heap[--heap_size];
    /* sift down */
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_size && node_less(&heap[l], &heap[smallest]))
            smallest = l;
        if (r < heap_size && node_less(&heap[r], &heap[smallest]))
            smallest = r;
        if (smallest == i) break;
        Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return root;
}

/* ------------------------------------------------------------------ */
/* Ensure cache capacity                                               */
/* ------------------------------------------------------------------ */
static void cache_resize(size_t min_cap)
{
    if (cache_cap >= min_cap) return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Global init (called on first invocation)                           */
/* ------------------------------------------------------------------ */
static void init_structures(void)
{
    /* Initialize heap with all single‑digit numbers (a=0..9) */
    for (unsigned int a = 0; a <= 9; ++a) {
        Node node;
        node.val = (seq_t)a;
        node.a   = a;
        node.k   = 1;
        heap_push(node);
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_structures();
        initialized = 1;
    }

    /* Return cached value if already computed */
    if (n <= cache_len) return cache[n - 1];

    /* Generate terms until we have at least n values */
    while (cache_len < n) {
        /* Get next term from the heap */
        Node cur = heap_pop();
        /* Store it */
        cache_resize(cache_len + 1);
        cache[cache_len++] = cur.val;

        /* Prepare next term in the same subsequence, if applicable */
        unsigned int p = period[cur.a];
        if (p != 0) {
            idx_t new_k = cur.k + p;
            seq_t new_val = cur.val;
            /* Append the required new digits */
            for (idx_t i = cur.k + 1; i <= new_k; ++i) {
                seq_t digit = ((seq_t)cur.a * (seq_t)i) % ((seq_t)10);
                new_val = new_val * ((seq_t)10) + digit;
            }
            Node nxt = { new_val, cur.a, new_k };
            heap_push(nxt);
        }
    }

    return cache[n - 1];
}
