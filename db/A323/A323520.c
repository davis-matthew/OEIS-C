/*
 * A323520  Numbers of the form p^(k^2) where p is prime and k >= 0.
 * https://oeis.org/A323520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   build_primes_upto, ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the sequence of numbers of the form p^(k²) where p is prime
 * and k ≥ 0, sorted in ascending order without duplicates (OEIS A323520).
 * a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 5, a(5) = 7, a(6) = 11, …
 */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

typedef struct {
    seq_t  val;          /* current value of this stream */
    size_t prime_index;  /* index into the global primes array */
    idx_t  k;            /* current exponent index (k ≥ 1) */
} node_t;

/* ---------- global state ---------- */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

static uint64_t *primes      = NULL;
static size_t    prime_cnt   = 0;
static size_t    prime_limit = 0;
static int       primes_inited = 0;

static node_t *heap   = NULL;
static size_t  heap_size = 0;
static size_t  heap_cap  = 0;

/* ---------- helper functions ---------- */

/* grow dynamic array */
static void grow_seq_arr(seq_t **arr, size_t *cap, size_t need)
{
    if (!*cap) {
        *arr = mem_new(seq_t, need);
        if (!*arr) abort();
        *cap = need;
    } else if (need > *cap) {
        seq_t *tmp = mem_resize(seq_t, *arr, need);
        if (!tmp) abort();
        *arr = tmp;
        *cap = need;
    }
}

/* heap operations */
static int node_cmp(const node_t *a, const node_t *b)
{
    return cmp_seq_t(&a->val, &b->val);
}

static void heap_swap(node_t *a, node_t *b)
{
    node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_heapify_up(size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (node_cmp(&heap[i], &heap[parent]) < 0) {
            heap_swap(&heap[i], &heap[parent]);
            i = parent;
        } else break;
    }
}

static void heap_heapify_down(size_t i)
{
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_size && node_cmp(&heap[right], &heap[left]) < 0)
            smallest = right;
        if (node_cmp(&heap[smallest], &heap[i]) < 0) {
            heap_swap(&heap[i], &heap[smallest]);
            i = smallest;
        } else break;
    }
}

static void heap_init(size_t cap)
{
    heap_cap = cap;
    heap = mem_new(node_t, cap);
    if (!heap) abort();
    heap_size = 0;
}

static void heap_push(node_t n)
{
    heap[heap_size++] = n;
    heap_heapify_up(heap_size - 1);
}

static node_t heap_pop(void)
{
    node_t ret = heap[0];
    heap[0] = heap[--heap_size];
    heap_heapify_down(0);
    return ret;
}

/* initialise primes once (enough for n up to ~10000) */
static void init_primes(void)
{
    if (primes_inited) return;
    prime_limit = 200000U;              /* covers the 10000th prime (~104729) */
    build_primes_upto(prime_limit, &primes, &prime_cnt);
    primes_inited = 1;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                    /* undefined for offset < 1 */

    /* initialise primes */
    if (!primes_inited) init_primes();

    /* ensure cache capacity */
    if (n > cache_cap) grow_seq_arr(&cache, &cache_cap, (size_t)n);

    /* initialise cache with 1 on first use */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* return if already computed */
    if (cache_size >= (size_t)n) return cache[(size_t)n - 1];

    /* initialise heap once */
    if (heap_size == 0) {
        heap_init(prime_cnt);
        for (size_t i = 0; i < prime_cnt; ++i) {
            node_t nd;
            nd.val = (seq_t)primes[i];   /* p^1 */
            nd.prime_index = i;
            nd.k = 1;
            heap_push(nd);
        }
    }

    /* generate entries until we have n of them */
    while (cache_size < (size_t)n) {
        node_t cur = heap_pop();
        /* avoid duplicate 1 or any unexpected repeats */
        if (cache_size == 0 || cmp_seq_t(&cache[cache_size - 1], &cur.val) != 0) {
            cache[cache_size++] = cur.val;
        }
        /* compute next power for this prime */
        cur.k += 1;
        idx_t exp = cur.k * cur.k;            /* k^2 */
        cur.val = ipow_idx((seq_t)primes[cur.prime_index], exp);
        heap_push(cur);
    }

    return cache[(size_t)n - 1];
}
