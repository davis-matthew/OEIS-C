/*
 * A244623  Odd prime powers that are not primes.
 * https://oeis.org/A244623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd prime powers that are not prime, with a(1) = 1 (1 is
 *            included as the first term)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 46, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper structures */
struct heap_node {
    seq_t val;   /* current value p^k (k ≥ 2) */
    size_t idx;  /* index into odd_primes array */
};

/* Heap operations */
static void heapify_down(struct heap_node *heap, size_t heap_size, size_t i)
{
    while (1) {
        size_t l = 2 * i + 1;
        size_t r = 2 * i + 2;
        size_t smallest = i;
        if (l < heap_size && heap[l].val < heap[smallest].val) smallest = l;
        if (r < heap_size && heap[r].val < heap[smallest].val) smallest = r;
        if (smallest == i) break;
        struct heap_node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

static void build_heap(struct heap_node *heap, size_t heap_size)
{
    if (heap_size <= 1) return;
    for (size_t i = (heap_size - 1) / 2; ; --i) {
        heapify_down(heap, heap_size, i);
        if (i == 0) break;
    }
}

/* Global state */
static bool initialized = false;
static uint32_t *odd_primes = NULL;
static size_t odd_prime_cnt = 0;
static struct heap_node *heap = NULL;
static size_t heap_size = 0;
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* One‑time initialization: generate odd primes up to a fixed limit,
   build the initial heap with their squares, and set up the cache. */
static void init_once(void)
{
    if (initialized) return;
    const idx_t PRIME_LIMIT = 200000;          /* enough for n ≤ 10000 */
    /* Sieve to collect odd primes */
    bool *is_composite = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    size_t odd_cap = 20000;
    odd_primes = (uint32_t*)malloc(odd_cap * sizeof(uint32_t));
    odd_prime_cnt = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (i > 2) {              /* odd prime */
                if (odd_prime_cnt == odd_cap) {
                    odd_cap *= 2;
                    odd_primes = (uint32_t*)realloc(odd_primes, odd_cap * sizeof(uint32_t));
                }
                odd_primes[odd_prime_cnt++] = (uint32_t)i;
            }
            if (i * i <= PRIME_LIMIT) {
                for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);

    /* Build the heap with each prime squared */
    heap_size = odd_prime_cnt;
    heap = (struct heap_node*)malloc(heap_size * sizeof(struct heap_node));
    for (size_t i = 0; i < heap_size; ++i) {
        seq_t sq = (seq_t)odd_primes[i] * odd_primes[i];
        heap[i].val = sq;
        heap[i].idx = i;
    }
    build_heap(heap, heap_size);

    /* Cache for sequence values (excluding the initial 1) */
    cache_len = 0;
    cache_cap = 0;
    cache = NULL;

    initialized = true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1; /* first term is 1 */
    init_once();

    idx_t desired = n - 2; /* zero‑based index into cache (a(2) → cache[0]) */

    while (cache_len <= desired) {
        /* smallest current value */
        seq_t cur = heap[0].val;

        /* store it */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        cache[cache_len++] = cur;

        /* advance the power for this prime */
        heap[0].val = heap[0].val * (seq_t)odd_primes[heap[0].idx];
        heapify_down(heap, heap_size, 0);
    }

    return cache[desired];
}
