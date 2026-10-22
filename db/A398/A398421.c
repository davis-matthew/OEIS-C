/*
 * A398421  Odd numbers of the form p^2*q, where p and q are distinct odd
 *          primes.
 * https://oeis.org/A398421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1454, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A398421: odd numbers of the form p^2 * q with p and q odd primes, p ≠ q.
   Implementation uses a min‑heap to generate the terms in increasing order.
   The function is memoised; it is safe for arbitrary order of calls. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----------------------------------------------------------- */
/* Prime generation (odd primes up to a fixed limit)             */
#define PRIME_LIMIT 2000000          /* sufficient for n ≤ 10000 */

static uint64_t *g_primes = NULL;    /* array of odd primes */
static size_t    g_prime_cnt = 0;    /* number of primes stored */

/* initialise the prime array */
static void init_primes(void)
{
    if (g_primes != NULL)
        return;                       /* already done */

    bool *is_comp = (bool *)malloc(PRIME_LIMIT + 1);
    if (!is_comp)
        return;                       /* out of memory */

    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        is_comp[i] = false;
    is_comp[0] = is_comp[1] = true;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = true;

    /* count odd primes */
    g_prime_cnt = 0;
    for (size_t i = 3; i <= PRIME_LIMIT; i += 2)
        if (!is_comp[i])
            ++g_prime_cnt;

    g_primes = (uint64_t *)malloc(g_prime_cnt * sizeof(uint64_t));
    if (!g_primes) {
        free(is_comp);
        return;                       /* out of memory */
    }

    size_t idx = 0;
    for (size_t i = 3; i <= PRIME_LIMIT; i += 2)
        if (!is_comp[i])
            g_primes[idx++] = i;

    free(is_comp);
}

/* ----------------------------------------------------------- */
/* Heap node for p^2 * q values                                 */
typedef struct {
    seq_t  val;      /* current value p^2 * q */
    size_t p_idx;    /* index of the prime that is squared      */
    size_t q_idx;    /* index of the prime used as q           */
} heap_node_t;

/* simple binary heap (min‑heap) operations */
static void heap_swap(heap_node_t *a, heap_node_t *b)
{
    heap_node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_sift_up(heap_node_t *h, size_t n, size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (h[parent].val <= h[i].val)
            break;
        heap_swap(&h[parent], &h[i]);
        i = parent;
    }
}

static void heap_sift_down(heap_node_t *h, size_t n, size_t i)
{
    while (1) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < n && h[left].val < h[smallest].val)
            smallest = left;
        if (right < n && h[right].val < h[smallest].val)
            smallest = right;
        if (smallest == i)
            break;
        heap_swap(&h[i], &h[smallest]);
        i = smallest;
    }
}

static void heap_push(heap_node_t *h, size_t *size, size_t cap, heap_node_t node)
{
    if (*size == cap)
        return;                       /* should not happen */
    h[*size] = node;
    heap_sift_up(h, *size + 1, *size);
    ++(*size);
}

static heap_node_t heap_pop(heap_node_t *h, size_t *size)
{
    heap_node_t top = h[0];
    h[0] = h[*size - 1];
    --(*size);
    heap_sift_down(h, *size, 0);
    return top;
}

/* ----------------------------------------------------------- */
/* Sequence cache                                               */
static seq_t *g_cache     = NULL;   /* computed terms */
static size_t g_cache_cap = 0;
static idx_t  g_cache_len = 0;     /* number of values stored */

/* grow an array to at least 'need' elements */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap)
        return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 4;
    while (new_cap < need)
        new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* ----------------------------------------------------------- */
/* Heap to produce the sequence terms                          */
static heap_node_t *g_heap   = NULL;
static size_t      g_heap_sz = 0;
static size_t      g_heap_cap = 0;

/* initialise the heap with the first q for each prime p */
static void init_heap(void)
{
    if (g_heap != NULL)
        return;                       /* already done */

    init_primes();

    g_heap_cap = g_prime_cnt;
    g_heap = (heap_node_t *)malloc(g_heap_cap * sizeof(heap_node_t));
    if (!g_heap)
        return;                       /* out of memory */
    g_heap_sz = 0;

    for (size_t p_idx = 0; p_idx < g_prime_cnt; ++p_idx) {
        size_t q_idx = 0;
        if (q_idx == p_idx) /* skip equal primes */
            ++q_idx;
        if (q_idx >= g_prime_cnt)
            continue;

        seq_t vp   = (seq_t)g_primes[p_idx];
        seq_t prod = vp * vp * (seq_t)g_primes[q_idx];
        heap_node_t node = { prod, p_idx, q_idx };
        heap_push(g_heap, &g_heap_sz, g_heap_cap, node);
    }
}

/* next q index after current, skipping equality with p */
static size_t next_q_index(size_t p_idx, size_t current_q)
{
    size_t q = current_q + 1;
    while (q < g_prime_cnt && q == p_idx)
        ++q;
    return q;
}

/* generate the next term and append to cache */
static void generate_next(void)
{
    heap_node_t entry = heap_pop(g_heap, &g_heap_sz);

    grow_seq(&g_cache, &g_cache_cap, g_cache_len + 1);
    g_cache[g_cache_len++] = entry.val;

    size_t q_next = next_q_index(entry.p_idx, entry.q_idx);
    if (q_next < g_prime_cnt) {
        seq_t vp   = (seq_t)g_primes[entry.p_idx];
        seq_t prod = vp * vp * (seq_t)g_primes[q_next];
        heap_node_t node = { prod, entry.p_idx, q_next };
        heap_push(g_heap, &g_heap_sz, g_heap_cap, node);
    }
}

/* ----------------------------------------------------------- */
/* Public function: returns the n‑th term (n≥1)                */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined, but handled gracefully */
        return 0;

    init_heap();

    while (g_cache_len < n)
        generate_next();

    return g_cache[n - 1];
}
