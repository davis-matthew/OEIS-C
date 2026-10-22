/*
 * A338701  Sums of two odd-indexed primes.
 * https://oeis.org/A338701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   build_primes_upto_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A338701:  Sums of two odd‑indexed primes.
   Computes a(n) = the n‑th smallest distinct sum p_i + p_j,  
   where p_i and p_j are primes in the ordinary list whose 1‑based
   position is odd.  The sequence is generated lazily and cached
   so that calls to a() are O(1) after the first O(n log n) build.
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto_local.h"

/* ---------- data structures for heap -------------------------------- */
typedef struct {
    seq_t sum;
    idx_t i;
    idx_t j;
} heap_node_t;

static heap_node_t *heap_nodes = NULL;
static size_t heap_size = 0;
static size_t heap_cap = 0;

/* ---- heap utilities (min‑heap on sum) ------------------------------- */
static inline void heap_swap(heap_node_t *a, heap_node_t *b)
{
    heap_node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_push_node(heap_node_t node)
{
    if (heap_size == heap_cap) {
        heap_cap = (heap_cap == 0) ? 16 : heap_cap * 2;
        heap_nodes = (heap_node_t *)realloc(heap_nodes, heap_cap * sizeof(heap_node_t));
    }
    heap_nodes[heap_size++] = node;
    size_t pos = heap_size - 1;
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap_nodes[pos].sum < heap_nodes[parent].sum) {
            heap_swap(&heap_nodes[pos], &heap_nodes[parent]);
            pos = parent;
        } else
            break;
    }
}

static heap_node_t heap_pop_node(void)
{
    heap_node_t ret = heap_nodes[0];
    heap_nodes[0] = heap_nodes[--heap_size];
    size_t pos = 0;
    while (true) {
        size_t left = (pos << 1) + 1;
        size_t right = left + 1;
        if (left >= heap_size) break;
        size_t smallest = left;
        if (right < heap_size && heap_nodes[right].sum < heap_nodes[left].sum)
            smallest = right;
        if (heap_nodes[smallest].sum < heap_nodes[pos].sum) {
            heap_swap(&heap_nodes[smallest], &heap_nodes[pos]);
            pos = smallest;
        } else
            break;
    }
    return ret;
}

/* ---------- global data for sequence generation --------------------- */
static uint64_t *odd_primes = NULL;
static size_t odd_cnt = 0;

static seq_t *sums = NULL;
static size_t sums_len = 0;
static size_t sums_cap = 0;

/* ---- ensure sums array capacity ------------------------------------ */
static void ensure_sums_capacity(size_t needed)
{
    if (needed <= sums_cap) return;
    sums_cap = needed ? needed * 2 : 16;
    sums = (seq_t *)realloc(sums, sums_cap * sizeof(seq_t));
}

/* ---------- lazy initialization ------------------------------------- */
static bool initialized = false;

static void init_sequence(void)
{
    /* Prime generation limit – ample for up to 10 000 terms. */
    const idx_t LIMIT = 300000;   /* primes up to 300 k are enough */

    uint64_t *all_primes = NULL;
    size_t all_cnt = 0;

    build_primes_upto_local(LIMIT, &all_primes, &all_cnt);

    odd_cnt = (all_cnt + 1) / 2;
    odd_primes = (uint64_t *)malloc(odd_cnt * sizeof(uint64_t));
    size_t k = 0;
    for (size_t i = 0; i < all_cnt; i += 2)   /* 0‑based indices 0,2,4,… are odd‑indexed */
        odd_primes[k++] = all_primes[i];

    free(all_primes);

    /* initialise min‑heap with sums (p_i + p_i) for all odd primes */
    heap_cap = odd_cnt;
    heap_nodes = (heap_node_t *)malloc(heap_cap * sizeof(heap_node_t));
    heap_size = 0;
    for (size_t i = 0; i < odd_cnt; ++i) {
        heap_node_t node;
        node.sum = (seq_t)odd_primes[i] + (seq_t)odd_primes[i];
        node.i = (idx_t)i;
        node.j = (idx_t)i;
        heap_push_node(node);
    }

    sums_len = 0;
    sums_cap = 0;
    sums = NULL;

    initialized = true;
}

/* ---------- required public function -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, but 0 is unused */

    if (!initialized)
        init_sequence();

    /* Generate sums until we have at least n distinct values. */
    while (sums_len < (size_t)n) {
        heap_node_t node = heap_pop_node();

        /* store distinct sums only */
        if (sums_len == 0 || node.sum != sums[sums_len - 1]) {
            ensure_sums_capacity(sums_len + 1);
            sums[sums_len++] = node.sum;
        }

        /* advance j for the current i */
        if (node.j + 1 < (idx_t)odd_cnt) {
            heap_node_t next;
            next.sum = (seq_t)odd_primes[node.i] + (seq_t)odd_primes[node.j + 1];
            next.i = node.i;
            next.j = node.j + 1;
            heap_push_node(next);
        }
    }

    return sums[n - 1];
}
