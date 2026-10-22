/*
 * A399265  Numbers having two factor pairs whose absolute pairwise
 *          differences form the legs of a primitive Pythagorean triple.
 * https://oeis.org/A399265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence consists of all integers N such that N =
 *            l1*l2*k*(k+1) where l1,l2 are legs of a primitive Pythagorean
 *            triple
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"

/* Euclidean GCD for uint64_t */

/* Node in priority queue: current value and its parameters */
typedef struct {
    seq_t val;
    uint64_t L;
    uint64_t k;
} Node;

/* Min‑heap for Node sorted by val */
static Node *heap = NULL;
static size_t heap_size = 0;
static size_t heap_cap  = 0;

static void heap_push(Node x)
{
    if (heap_size == heap_cap) {
        heap_cap = (heap_cap == 0) ? 16 : heap_cap * 2;
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
    }
    size_t i = heap_size++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= x.val) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i] = x;
}

static Node heap_pop(void)
{
    Node min = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        if (l >= heap_size) break;
        size_t s = l;
        if (r < heap_size && heap[r].val < heap[l].val) s = r;
        if (heap[s].val >= heap[i].val) break;
        Node tmp = heap[i];
        heap[i] = heap[s];
        heap[s] = tmp;
        i = s;
    }
    return min;
}

/* Cache of produced sequence values */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Initialisation flag */
static int initialized = 0;

/* Initialise the list of primitive triples and the heap */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = 1;

    /* Generate primitive triples with Euclid's formula */
    const int M = 200;   /* sufficient for the first 10 000 terms */
    for (int m = 2; m <= M; ++m) {
        for (int n = 1; n < m; ++n) {
            if (((m - n) & 1) == 0) continue;      /* opposite parity */
            if (gcd_u64(m, n) != 1) continue;      /* coprime */
            uint64_t a = (uint64_t)m * m - (uint64_t)n * n;
            uint64_t b = 2ULL * (uint64_t)m * (uint64_t)n;
            if (a == 0 || b == 0) continue;
            uint64_t L = a * b;                     /* product of legs */
            Node node;
            node.val = (seq_t)L * (seq_t)2;         /* k = 1 -> pronic = 2 */
            node.L   = L;
            node.k   = 1;
            heap_push(node);
        }
    }

    /* Initialise cache for output values */
    cache_cap = 4096;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_size = 0;
}

/* Return the n‑th term (1‑based) of the sequence */
seq_t a(idx_t n)
{
    init_sequence();

    /* produce terms up to index n */
    while (cache_size < (size_t)n) {
        Node cur = heap_pop();
        seq_t v = cur.val;

        /* deduplicate identical successive terms */
        if (cache_size == 0 || v != cache[cache_size - 1]) {
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = v;
        }

        /* next value from the same primitive triple (k+1) */
        uint64_t knext   = cur.k + 1;
        seq_t next_val = (seq_t)cur.L * (seq_t)knext * (seq_t)(knext + 1);
        Node next;
        next.val = next_val;
        next.L   = cur.L;
        next.k   = knext;
        heap_push(next);
    }

    return cache[n - 1];
}
