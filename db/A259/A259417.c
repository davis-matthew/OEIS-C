/*
 * A259417  Even powers of the odd primes listed in increasing order.
 * https://oeis.org/A259417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 35, u64 n <= 473, u128 n <= 473,
 *            big any n
 * Helpers:   build_primes_upto, cmp_seq_t, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) – the sequence of even powers of odd primes, with a(1)=1.
   a(n) is the n‑th smallest integer of the form p^(2k) (p odd prime, k≥1).
   The sequence is built lazily and cached so that calls in arbitrary order
   are efficient. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/cmp_seq_t.h"

typedef struct {
    seq_t value;   /* current power p^(2k) */
    seq_t prime;   /* prime p */
} Node;

/* --- Min‑heap for Node, ordered by value --------------------------------- */
static size_t heap_size = 0;
static size_t heap_cap  = 0;
static Node *heap = NULL;

static inline int node_lt(const Node *a, const Node *b)
{
    return cmp_seq_t(&a->value, &b->value) < 0;
}

static void heap_push(Node node)
{
    if (heap_size == heap_cap) {
        heap_cap = (heap_cap > 0) ? heap_cap * 2 : 64;
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
    }
    size_t i = heap_size++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (!node_lt(&node, &heap[parent])) break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = node;
}

static void heap_pop(Node *out)
{
    *out = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    for (;;) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < heap_size && node_lt(&heap[left], &heap[smallest]))
            smallest = left;
        if (right < heap_size && node_lt(&heap[right], &heap[smallest]))
            smallest = right;
        if (smallest == i) break;
        Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

/* --- Sequence cache ----------------------------------------------------- */
static seq_t *cache  = NULL;   /* 1‑based: cache[0] = a(1) */
static idx_t cache_size = 0;
static idx_t cache_cap  = 0;

/* --- Prime list (odd primes) ------------------------------------------- */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* --- Static initialisation on first call -------------------------------- */
static void init_static(void)
{
    if (prime_list) return; /* already initialised */

    /* Generate odd primes up to a fixed limit.  200 000 is enough for
       n ≈ 10 000 as the sequence grows super‑polynomial. */
    size_t cnt;
    const size_t LIMIT = 200000;
    build_primes_upto(LIMIT, &prime_list, &cnt);

    /* prime_list[0] is 2 – skip it. */
    prime_cnt = cnt - 1;

    /* Initialise heap with p² for each odd prime. */
    heap_cap = prime_cnt;
    heap = (Node *)malloc(heap_cap * sizeof(Node));
    heap_size = 0;

    for (size_t i = 1; i < cnt; ++i) {          /* skip 2 */
        uint64_t p = prime_list[i];
        Node node;
        node.prime = (seq_t)p;
        seq_t pp = seq_mul((seq_t)p, (seq_t)p);   /* p² */
        node.value = pp;
        heap_push(node);
    }

    /* Initialise cache with a(1) = 1. */
    cache_cap = 1024;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache[0] = (seq_t)1;
    cache_size = 1;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */
    init_static();

    while (cache_size < n) {
        Node cur;
        heap_pop(&cur);

        /* Grow cache if necessary. */
        if (cache_size == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_size++] = cur.value;

        /* Push next even power of the same prime. */
        seq_t prime_sq = seq_mul(cur.prime, cur.prime);   /* p² */
        cur.value = seq_mul(cur.value, prime_sq);         /* p^(2k+2) */
        heap_push(cur);
    }

    return cache[n - 1];
}
